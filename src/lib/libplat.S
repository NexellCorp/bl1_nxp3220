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

ENTRY(secure_launch)
	mov	r11, r1
	msr	CPSR_c,  #(MODE_SVC|I_BIT)

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
	mov	r0, #(MODE_SVC | I_BIT)
	msr	SPSR_cxsf, r0
	mov	r0, #0
	movs	pc, lr
ENDPROC(non_secure_launch)
