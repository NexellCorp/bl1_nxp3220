/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#include "../include/asm.h"
#include "../include/armv7.h"
	.align	4
ENTRY(get_fntpr)
	mrc	p15, 0, r0, c13, c0, 4
	bx	lr
ENDPROC(get_fntpr)

ENTRY(set_fnptr)
	mcr	p15, 0, r0, c13, c0, 4
	bx	lr
ENDPROC(set_fnptr)

ENTRY(get_boption)
	mrc	p15, 0, r0, c13, c0, 1
	bx	lr
ENDPROC(get_boption)

ENTRY(set_boption)
	mcr	p15, 0, r0, c13, c0, 1
	bx	lr
ENDPROC(set_boption)

ENTRY(smc_get_fnptr)
	mrc	p15, 0, r0, c13, c0, 2
	bx	lr
ENDPROC(smc_get_fnptr)

ENTRY(smc_set_fnptr)
	mcr	p15, 0, r0, c13, c0, 2
	bx	lr
ENDPROC(smc_set_fnptr)

ENTRY(get_cpuid)
	mrc	p15, 0, r0, c0, c0, 5
	ands	r0, r0, #0xF
	bx	lr
ENDPROC(get_cpuid)

ENTRY(secure_launch)
	mov	r11, r1
	msr	CPSR_c,  #(MODE_MON | A_BIT | I_BIT)

	mov	lr, r2
	mov	r1, r3

	mov	r3, #0

	bx	r11
ENDPROC(secure_launch)

ENTRY(non_secure_launch)
	mov	r4, r1

	mrs	r0, cpsr
	and	r0, #0x1F
	cmp	r0, #MODE_MON
	bne	.

	/* Change to Secure -> Non-Secure */
	bl	set_nonsecure_mode

	mov	lr, r4

	mov	r1,  #0
	mov	r2,  #0
	mov	r3,  #0
	mov	r4,  #0
	mov	r5,  #0
	mov	r6,  #0
	mov	r7,  #0
	mov	r8,  #0
	mov	r9,  #0
	mov	r10, #0
	mov	r11, #0
	mov	r12, #0

	/* switch to supervisor mode */
	mov	r0, #(MODE_SVC | A_BIT | I_BIT)
	msr	SPSR_cxsf, r0
	mov	r0, #0
	movs	pc, lr
ENDPROC(non_secure_launch)
