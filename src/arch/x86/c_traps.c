/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#include <config.h>
#include <model/statedata.h>
#include <arch/machine/fpu.h>
#include <arch/fastpath/fastpath.h>
#include <arch/kernel/traps.h>
#include <machine/debug.h>
#include <arch/object/vcpu.h>
#include <api/syscall.h>
#include <arch/api/vmenter.h>

#include <benchmark/benchmark_track.h>
#include <benchmark/benchmark_utilisation.h>

void VISIBLE
c_nested_interrupt(int irq)
{
    /* This is not a real entry point, so we do not grab locks or
     * run c_entry/exit_hooks, since this occurs only if we're already
     * running inside the kernel. Just record the irq and return */
    assert(ARCH_NODE_STATE(x86KSPendingInterrupt) == int_invalid);
    ARCH_NODE_STATE(x86KSPendingInterrupt) = irq;
}

void VISIBLE NORETURN
c_handle_interrupt(int irq, int syscall)
{
    NODE_LOCK_IF(irq != int_remote_call_ipi);

    c_entry_hook();

    if (irq == int_unimpl_dev) {
        handleUnimplementedDevice();
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.path = Entry_UnimplementedDevice;
        ksKernelEntry.word = irq;
#endif
    } else if (irq == int_page_fault) {
        /* Error code is in Error. Pull out bit 5, which is whether it was instruction or data */
        vm_fault_type_t type = (NODE_STATE(ksCurThread)->tcbArch.tcbContext.registers[Error] >> 4u) & 1u;
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.path = Entry_VMFault;
        ksKernelEntry.word = type;
#endif
        handleVMFaultEvent(type);
#ifdef CONFIG_HARDWARE_DEBUG_API
    } else if (irq == int_debug || irq == int_software_break_request) {
        /* Debug exception */
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.path = Entry_DebugFault;
        ksKernelEntry.word = NODE_STATE(ksCurThread)->tcbArch.tcbContext.registers[FaultIP];
#endif
        handleUserLevelDebugException(irq);
#endif /* CONFIG_HARDWARE_DEBUG_API */
    } else if (irq < int_irq_min) {
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.path = Entry_UserLevelFault;
        ksKernelEntry.word = irq;
#endif
        handleUserLevelFault(irq, NODE_STATE(ksCurThread)->tcbArch.tcbContext.registers[Error]);
    } else if (likely(irq < int_trap_min)) {
        ARCH_NODE_STATE(x86KScurInterrupt) = irq;
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.path = Entry_Interrupt;
        ksKernelEntry.word = irq;
#endif
        handleInterruptEntry();
        /* check for other pending interrupts */
        receivePendingIRQ();
    } else if (irq == int_spurious) {
        /* fall through to restore_user_context and do nothing */
    } else {
        /* Interpret a trap as an unknown syscall */
        /* Adjust FaultIP to point to trapping INT
         * instruction by subtracting 2 */
        int sys_num;
        NODE_STATE(ksCurThread)->tcbArch.tcbContext.registers[FaultIP] -= 2;
        /* trap number is MSBs of the syscall number and the LSBS of EAX */
        sys_num = (irq << 24) | (syscall & 0x00ffffff);
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.path = Entry_UnknownSyscall;
        ksKernelEntry.word = sys_num;
#endif
        handleUnknownSyscall(sys_num);
    }
    restore_user_context();
    UNREACHABLE();
}

void NORETURN
slowpath(syscall_t syscall)
{

#ifdef CONFIG_VTX
    if (syscall == SysVMEnter) {
        vcpu_update_state_sysvmenter(NODE_STATE(ksCurThread)->tcbArch.vcpu);
        if (NODE_STATE(ksCurThread)->tcbBoundNotification && notification_ptr_get_state(NODE_STATE(ksCurThread)->tcbBoundNotification) == NtfnState_Active) {
            completeSignal(NODE_STATE(ksCurThread)->tcbBoundNotification, NODE_STATE(ksCurThread));
            setRegister(NODE_STATE(ksCurThread), msgInfoRegister, SEL4_VMENTER_RESULT_NOTIF);
            /* Any guest state that we should return is in the same
             * register position as sent to us, so we can just return
             * and let the user pick up the values they put in */
            restore_user_context();
        } else {
            setThreadState(NODE_STATE(ksCurThread), ThreadState_RunningVM);
            restore_user_context();
        }
    }
#endif
    /* check for undefined syscall */
    if (unlikely(syscall < SYSCALL_MIN || syscall > SYSCALL_MAX)) {
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.path = Entry_UnknownSyscall;
        /* ksKernelEntry.word word is already set to syscall */
#endif /* TRACK_KERNEL_ENTRIES */
        handleUnknownSyscall(syscall);
    } else {
#ifdef TRACK_KERNEL_ENTRIES
        ksKernelEntry.is_fastpath = 0;
#endif /* TRACK KERNEL ENTRIES */
        handleSyscall(syscall);
    }

    restore_user_context();
    UNREACHABLE();
}

void VISIBLE NORETURN
c_handle_syscall(word_t cptr, word_t msgInfo, syscall_t syscall)
{
    NODE_LOCK;

    c_entry_hook();

#ifdef TRACK_KERNEL_ENTRIES
    benchmark_debug_syscall_start(cptr, msgInfo, syscall);
    ksKernelEntry.is_fastpath = 1;
#endif /* TRACK_KERNEL_ENTRIES */

    if (config_set(CONFIG_SYSENTER)) {
        /* increment NextIP to skip sysenter */
        NODE_STATE(ksCurThread)->tcbArch.tcbContext.registers[NextIP] += 2;
    } else {
        /* set FaultIP */
        setRegister(NODE_STATE(ksCurThread), FaultIP, getRegister(NODE_STATE(ksCurThread), NextIP) - 2);
    }

#ifdef CONFIG_FASTPATH
    if (syscall == (syscall_t)SysCall) {
        fastpath_call(cptr, msgInfo);
        UNREACHABLE();
    } else if (syscall == (syscall_t)SysReplyRecv) {
        fastpath_reply_recv(cptr, msgInfo);
        UNREACHABLE();
    }
#endif /* CONFIG_FASTPATH */
    slowpath(syscall);
    UNREACHABLE();
}

#ifdef CONFIG_VTX
void VISIBLE NORETURN c_handle_vmexit(void)
{
    c_entry_hook();
    /* NODE_LOCK will get called in handleVmexit */
    handleVmexit();
    /* When we switched out of VMX mode the FS and GS registers were clobbered
     * and set to potentially undefined values. If we are going to switch back
     * to VMX mode then this is fine, but if we are switching to user mode we
     * need to make sure we reload the correct values of FS and GS. Unfortunately
     * our cached values in x86KSCurrent[FG]SBase now mismatch what is in the
     * hardware. To force a reload to happen we set the cached value to something
     * that is guaranteed to not be the target threads value, ensuring both
     * the cache and the hardware get updated */
    tcb_t *cur_thread = NODE_STATE(ksCurThread);
    if (thread_state_ptr_get_tsType(&cur_thread->tcbState) != ThreadState_RunningVM) {
        ARCH_NODE_STATE(x86KSCurrentGSBase) = -(word_t)1;
        ARCH_NODE_STATE(x86KSCurrentFSBase) = -(word_t)1;
    }
    restore_user_context();
    UNREACHABLE();
}
#endif
