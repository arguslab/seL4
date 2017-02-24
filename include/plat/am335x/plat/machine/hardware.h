/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __PLAT_MACHINE_HARDWARE_H
#define __PLAT_MACHINE_HARDWARE_H

#include <basic_types.h>
#include <arch/linker.h>
#include <plat/machine.h>
#include <plat/machine/devices.h>
#include <plat/machine/interrupt.h>

#define physBase          0x80000000
#define kernelBase        0xf0000000

static const kernel_frame_t BOOT_RODATA kernel_devices[] = {
    {
        /*  DM Timer 0 */
        DMTIMER0_PADDR,
        DMTIMER0_PPTR,
        true  /* armExecuteNever */
    },
    /*  INTC */
    {
        INTC_PADDR,
        INTC_PPTR,
        true  /* armExecuteNever */
    },
    {
        /*  WDT1 */
        WDT1_PADDR,
        WDT1_PPTR,
        true  /* armExecuteNever */
    },
    {
        /*  CMPER */
        CMPER_PADDR,
        CMPER_PPTR,
        true  /* armExecuteNever */
#ifdef CONFIG_PRINTING
    },
    {
        /*  UART */
        UART0_PADDR,
        UART0_PPTR,
        true  /* armExecuteNever */
#endif
    }
};

/* Available physical memory regions on platform (RAM minus kernel image). */
/* NOTE: Regions are not allowed to be adjacent! */

/* pointer to end of kernel image */
/* need a fake array to get the pointer from the linker script */
//TODO is this really needed?
extern char ki_end[1];

static const p_region_t BOOT_RODATA avail_p_regs[] = {
    /* 128 MiB of memory minus kernel image at its beginning */
    { /* .start = */ (pptr_t)ki_end - (kernelBase - physBase), /* .end = */ 0x88000000 }
};

static const p_region_t BOOT_RODATA dev_p_regs[] = {
    /* SoC devices: */
    { /* .start = */ UART0_PADDR,     /* .end = */ UART0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART1_PADDR,     /* .end = */ UART1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART2_PADDR,     /* .end = */ UART2_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART3_PADDR,     /* .end = */ UART3_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART4_PADDR,     /* .end = */ UART4_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART5_PADDR,     /* .end = */ UART5_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER0_PADDR,  /* .end = */ DMTIMER0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER1_PADDR,  /* .end = */ DMTIMER1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER2_PADDR,  /* .end = */ DMTIMER2_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER3_PADDR,  /* .end = */ DMTIMER3_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER4_PADDR,  /* .end = */ DMTIMER4_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER5_PADDR,  /* .end = */ DMTIMER5_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER6_PADDR,  /* .end = */ DMTIMER6_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ DMTIMER7_PADDR,  /* .end = */ DMTIMER7_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ WDT0_PADDR,      /* .end = */ WDT0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ WDT1_PADDR,      /* .end = */ WDT1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ GPIO0_PADDR,     /* .end = */ GPIO0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ GPIO1_PADDR,     /* .end = */ GPIO1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ GPIO2_PADDR,     /* .end = */ GPIO2_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ GPIO3_PADDR,     /* .end = */ GPIO3_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ USBSS_PADDR,     /* .end = */ USBSS_PADDR + (20 << PAGE_BITS)},
    { /* .start = */ SPI0_PADDR,      /* .end = */ SPI0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ SPI1_PADDR,      /* .end = */ SPI1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ MMCHS0_PADDR,    /* .end = */ MMCHS0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ MMCHS1_PADDR,    /* .end = */ MMCHS1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ MMCHS2_PADDR,    /* .end = */ MMCHS2_PADDR + (16 << PAGE_BITS) },
    { /* .start = */ GPMC0_PADDR,     /* .end = */ GPMC0_PADDR + (4096 << PAGE_BITS) },
    { /* .start = */ ELM0_PADDR,      /* .end = */ ELM0_PADDR + (16 << PAGE_BITS) },
    { /* .start = */ I2C0_PADDR,      /* .end = */ I2C0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ I2C1_PADDR,      /* .end = */ I2C1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ I2C2_PADDR,      /* .end = */ I2C2_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ CMPER_PADDR,     /* .end = */ CMPER_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ CONTROL_PADDR,   /* .end = */ CONTROL_PADDR + (32 << PAGE_BITS) },
    { /* .start = */ EDMA30CC0_PADDR, /* .end = */ EDMA30CC0_PADDR + (256 << PAGE_BITS) },
    { /* .start = */ DCAN0_PADDR,     /* .end = */ DCAN0_PADDR + (2 << PAGE_BITS) },
    { /* .start = */ DCAN1_PADDR,     /* .end = */ DCAN1_PADDR + (2 << PAGE_BITS) },
    { /* .start = */ LCDC0_PADDR,     /* .end = */ LCDC0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ ADCTSC0_PADDR,   /* .end = */ ADCTSC0_PADDR + (1024 << PAGE_BITS) },
    /* Board devices. */
    /* TODO: This should ultimately be replaced with a more general solution. */
};

#endif
