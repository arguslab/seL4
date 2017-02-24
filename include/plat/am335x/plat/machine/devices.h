/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __PLAT_MACHINE_DEVICES_H
#define __PLAT_MACHINE_DEVICES_H

/* These devices are used by the kernel. */
#define INTC_PPTR                       0xfff01000
#define UART0_PPTR                      0xfff02000
#define DMTIMER0_PPTR                   0xfff03000
#define WDT1_PPTR                       0xfff04000
#define CMPER_PPTR                      0xfff05000
#define ARM_DEBUG_MMAPPING_PPTR         0xfff06000


/* Other devices on the SoC. */
#define INTC_PADDR      0x48200000 /* 4 KB */
#define UART0_PADDR     0x44E09000 /* 4 KB */
#define UART1_PADDR     0x48022000 /* 4 KB */
#define UART2_PADDR     0x48024000 /* 4 KB */
#define UART3_PADDR     0x481A6000 /* 4 KB */
#define UART4_PADDR     0x481A8000 /* 4 KB */
#define UART5_PADDR     0x481AA000 /* 4 KB */
#define DMTIMER0_PADDR  0x44E05000 /* 4 KB */
#define DMTIMER1_PADDR  0x44E31000 /* 4 KB */
#define DMTIMER2_PADDR  0x48040000 /* 4 KB */
#define DMTIMER3_PADDR  0x48042000 /* 4 KB */
#define DMTIMER4_PADDR  0x48044000 /* 4 KB */
#define DMTIMER5_PADDR  0x48046000 /* 4 KB */
#define DMTIMER6_PADDR  0x48048000 /* 4 KB */
#define DMTIMER7_PADDR  0x4804A000 /* 4 KB */
#define WDT0_PADDR      0x44E33000 /* 4 KB */
#define WDT1_PADDR      0x44E35000 /* 4 KB */
#define GPIO0_PADDR     0x44E07000 /* 4 KB */
#define GPIO1_PADDR     0x4804C000 /* 4 KB */
#define GPIO2_PADDR     0x481AC000 /* 4 KB */
#define GPIO3_PADDR     0x481AE000 /* 4 KB */
#define USBSS_PADDR     0x47400000 /* 20 KB */
#define SPI0_PADDR      0x48030000 /* 4 KB */
#define SPI1_PADDR      0x481A0000 /* 4 KB */
#define MMCHS0_PADDR    0x48060000 /* 4 KB */
#define MMCHS1_PADDR    0x481D8000 /* 4 KB */
#define MMCHS2_PADDR    0x47810000 /* 64 KB */
#define GPMC0_PADDR     0x50000000 /* 16 MB */
#define ELM0_PADDR      0x48080000 /* 64 KB */
#define I2C0_PADDR      0x44E0B000 /* 4 KB */
#define I2C1_PADDR      0x4802A000 /* 4 KB */
#define I2C2_PADDR      0x4819C000 /* 4 KB */
#define CMPER_PADDR     0x44E00000 /* 1 KB */
#define CONTROL_PADDR   0x44E10000 /* 128 KB */
#define EDMA30CC0_PADDR 0x49000000 /* 1 MB */
#define DCAN0_PADDR     0x481CC000 /* 8 KB */
#define DCAN1_PADDR     0x481D0000 /* 8 KB */
#define LCDC0_PADDR     0x4830E000 /* 4 KB */
#define ADCTSC0_PADDR   0x44E0D000 /* 4 MB */

#endif
