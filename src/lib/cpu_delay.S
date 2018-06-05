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
