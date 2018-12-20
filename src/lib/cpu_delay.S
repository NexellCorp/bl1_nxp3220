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
