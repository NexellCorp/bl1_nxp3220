/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexcel and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#include "../include/asm.h"
#include "../include/armv7.h"
#include "../include/nxp3220.h"

.global iram_dev_freq
iram_dev_freq:
	b	.

.global iram_suspend
iram_suspend:
	b	psci_cpu_suspend_start
	b	.

/* monitor mode - fiq handler */
ENTRY(smc_asm_monitor_fiq_handler)
	stmfd	sp!, {r0-r7, lr}
	bl	smc_monitor_fiq_handler
	ldmfd	sp!, {r0-r7, lr}
	subs	pc, r14, #4
ENDPROC(smc_asm_monitor_fiq_handler)

/*
 * SMC Caller Handler
 */
EXPORT(bl1_smc_handler)
ENTRY(smc_asm_handler)
	stmfd	sp!, {r4-r12, lr}
	/*
	 * store the parameters in the stack
	 * before switching to secure mode.
	 */
	stmfd	sp!, {r0-r3}
	/* change the secure-mode */
	mrc	p15, 0, r9, c1, c1, 0
	bic	r9, r9, #(1 << 0)
	mcr	p15, 0, r9, c1, c1, 0

	/* load the parameters in the stack */
	ldmfd	sp!, {r0-r3}
	bl	bl1_smc_handler

	/* change the non-secure-mode */
	mrc	p15, 0, r9, c1, c1, 0
	orr	r9, r9, #(1 << 0)
	mcr	p15, 0, r9, c1, c1, 0

	ldmfd	sp!, {r4-r12, lr}
	movs	pc, r14
ENDPROC(smc_asm_handler)

/*
 * Monitor Mode - Exception Vetor table
 *
 */
.align 6
.global monitor_vectors
monitor_vectors:
        b       iram_suspend							@ 00 - reset will never get here
        b       .    								@ 04 -
        b       smc_asm_handler							@ 08 - SMC Interrupt
        b       .          							@ 0C -
        b       .          							@ 10 -
        b       iram_dev_freq							@ 14 -	No Exception get here
        b       .                   						@ 18 -
	b       smc_asm_monitor_fiq_handler					@ 1C - FIQ Interrupt

/*
 *  Only Monitor Mode
 */
.align 4
ENTRY(monitor_mode_init)
	/* -----------------------------------------
	 * Secure - SVC Mode -> Monitor Mode
	 * Changes in the SVC Mode to MON Mode,
	 * and store the link register values.
	 * -----------------------------------------
	 */
	mov	r8, lr
	mov	lr, #0								@ svc lr
	mov	sp, #0								@ svc sp

	/* Switch to monitor mode  */
	msr	CPSR_c,  #(MODE_MON|I_BIT|F_BIT)

        mrc     p15, 0, r0, c1, c0, 0						@ Read CP15 System Control register
//      bic     r0, r0, #(BIT1_V|BIT1_I|BIT1_Z) 				@ Clear V:13, I:12, Z:11 bit to disable High Vector, I-Cache, Branch Prediction
        bic     r0, r0, #(BIT1_C|BIT1_M)					@ Clear C:2, M:0 bit to disable D Cache, MMU
        mcr     p15, 0, r0, c1, c0, 0						@ Write value back to CP15 System Control register

	/* Stack size set to monitor mode  */
	ldr     sp, =SRAM_BASEADDR
	add     sp, sp, #BL1_SRAM_SIZE
	sub	sp, sp, #BL1_MON_STACK_SIZE

	mrc     p15, 0, r12, c0, c0, 5						@ Get our cpu id
	ands    r12, r12, #0xF							@ Save CPU id

	mov     r1, #0x100
	sub     r2, r12, #1
	and     r2, r2, #3
	mul     r1, r1, r2
	sub     sp, sp, r1

set_montior_vector:
	/* (MVBAR) set to monitor vector base address */
	ldr	r0, =monitor_vectors
	mcr	p15, 0, r0, c12, c0, 1

	bx	r8
ENDPROC(monitor_mode_init)

ENTRY(psci_power_down_wfi)
	dsb	sy								@ ensure write buffer empty
wfi_spill:
	wfi
	b	wfi_spill
ENDPROC(psci_power_down_wfi)
