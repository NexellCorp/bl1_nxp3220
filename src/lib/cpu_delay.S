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

ENTRY(set_delay_cnt)
	mcr	p15, 0, r0, c13, c0, 2
	bx	lr
ENDPROC(set_delay_cnt)

ENTRY(cpu_udelay)
//	mrc	p15, 0, r1, c13, c0, 2
	mul	r0, r0, r1
	lsrs	r0, r0, #1
	bxeq	lr
more_delay:
	subs	r0, r0, #1
	bne	more_delay
	bx	lr
ENDPROC(cpu_udelay)
