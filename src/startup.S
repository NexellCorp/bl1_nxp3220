/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#include "include/asm.h"
#include "include/armv7.h"
#include "include/nxp3220.h"

        .align 4
/* external asm function */
EXPORT(main)
EXPORT(enable_fpu)
EXPORT(armv7_pmu_init)
EXPORT(monitor_mode_init)
EXPORT(gic_initialize)
EXPORT(invaildate_l1_icache)
EXPORT(invaildate_dcache)

/* arm exceptions vectors */
.global vectors
vectors:
        b       reset_handler       						@ 0x00 - Reset
        b       .          							@ 0x04 - Undefined instructions
        b       .                   						@ 0x08 - SWI instructions
        b       .          							@ 0x0C - Instruction fetch aborts
        b       .          							@ 0x10 - Data access aborts
        b       .                  						@ 0x14 - Reserved (was address exception)
        b       .                   						@ 0x18 - IRQ interrupts
        b       .                   						@ 0x1C - FIQ interrupts

reset_handler:
	bl	monitor_mode_init

	/* Set the SMP Bit for DCache */
        mrc     p15, 0, r0, c1, c0, 1
        tst     r0, #BIT_SMP							@ SMP bit
        orreq   r0, r0, #BIT_SMP
        mcreq   p15, 0, r0, c1, c0, 1               				@ Write the Aux Control Register

	/* Set Vector Base Address Register (VBAR) to point to this application's vector table */
        ldr     r0, =vectors
        mcr     p15, 0, r0, c12, c0, 0

	/* Enable the FPU */
	bl	enable_fpu

	/* Check if Secondary Core? */
        mrc     p15, 0, r0, c0, c0, 5
        ands    r0, r0, #0xF
        bne     subcpu_startup

        /* Invaild L1 Instruction Cache	*/
	bl	invaildate_l1_icache

	/* Set the BSS-Area to zero. */
        ldr     r1, =__bss_start__
        ldr     r2, =__bss_end__

        mov     r3, #0x00000000							@ Prepare zero to clear BSS

clbss_l:
        cmp     r1, r2								@ while not at end of BSS
        strlo   r3, [r1]							@ clear 32-bit BSS word
        addlo   r1, r1, #4
        blo     clbss_l

	bl	armv7_pmu_init

	bl	main
	b	.

ENTRY(subcpu_startup)
        mrc     p15, 0, r0, c1, c0, 0						@ Read CP15 System Control register
        bic     r0, r0, #(BIT1_V|BIT1_I|BIT1_Z)					@ Clear V:13, I:12, Z:11 bit to disable High Vector, I-Cache, Branch Prediction
        bic     r0, r0, #(BIT1_C|BIT1_M)					@ Clear C:2, M:0 bit to disable D Cache, MMU
        mcr     p15, 0, r0, c1, c0, 0

	mrc     p15, 0, r0, c1, c0, 0						@ Read CP15 System Control register
	orr     r0, r0, #(BIT1_I|BIT1_Z)					@ Set I:12, Z:11 bit to enable I-Cache, Branch Prediction
	mcr     p15, 0, r0, c1, c0, 0

        mrc     p15, 0, r0, c0, c0, 5
        ands    r0, r0, #0xF

	bl	gic_initialize

	b	subcpu_main
ENDPROC(subcpu_startup)