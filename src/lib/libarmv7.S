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

	.align 5								@ below instruction number is 8, 32bytes

ENTRY(enable_fpu)
	/* Set NSACR to access CP10, CP11 in secure and non-secure mode */
	mrc	p15, 0, r0, c1, c1, 2
	orr	r0, r0, #(3 << 10)						@ enable fpu
	bic	r0, r0, #(3 << 14)
	mcr	p15, 0, r0, c1, c1, 2

	/* Set CPACR for access to CP10, CP11 */
	mov	r0, #(0xF<<20)
	mcr	p15, 0, r0, c1, c0, 2

	/* Set FPEXC EN bit to enable the FPU */
	mov	r0, #0x40000000

	vmsr	fpexc, r0

	mov	pc, lr
ENDPROC(enable_fpu)

ENTRY(set_nonsecure_mode)
	mrc	p15, 0, r0, c1, c1, 0
	orr	r0, r0, #(1 << 0)
	mcr	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(set_nonsecure_mode)

ENTRY(set_secure_mode)
	mrc	p15, 0, r0, c1, c1, 0
	bic	r0, r0, #(1 << 0)
	mcr	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(set_secure_mode)

ENTRY(get_secure_status)
	mrc	p15, 0, r0, c1, c1, 0
	and	r0, r0, #(1 << 0)
	bx	lr
ENDPROC(get_secure_status)

ENTRY(armv7_get_cpuid)
	mrc	p15, 0, r0, c0, c0, 5
	ands	r0, r0, #0xF
	bx	lr
ENDPROC(armv7_get_cpuid)

ENTRY(armv7_get_mpidr)
	mrc	p15, 0, r0, c0, c0, 5						@ Get MPIDR
	bx	lr
ENDPROC(armv7_get_mpidr)

ENTRY(armv7_set_auxctrl)
	mcr	p15, 0, r0, c1, c0, 1
	bx	lr
ENDPROC(armv7_set_auxctrl)

ENTRY(armv7_get_auxctrl)
	mrc	p15, 0, r0, c1, c0, 1
	bx	lr
ENDPROC(armv7_get_auxctrl)

ENTRY(armv7_get_scr)
	mrc	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(armv7_get_scr)

ENTRY(armv7_set_scr)
	mcr	p15, 0, r0, c1, c1, 0
	bx	lr
ENDPROC(armv7_set_scr)

ENTRY(set_mon_mode)
	msr	CPSR_c,  #(MODE_MON|I_BIT)
	bx	lr
ENDPROC(set_mon_mode)

ENTRY(set_svc_mode)
	msr	CPSR_c,  #(MODE_SVC|I_BIT)
	bx	lr
ENDPROC(set_svc_mode)

ENTRY(invaildate_l1_icache)
	/* Invalidate L1 Instruction Cache */
	mrc	p15, 1, r0, c0, c0, 1	   					@ Read Cache Level ID Register (CLIDR)
	tst	r0, #0x3		   					@ Harvard Cache?
	mov	r0, #0			   					@ SBZ
	mcrne	p15, 0, r0, c7, c5, 0	   					@ ICIALLU - Invalidate instruction cache and flush branch target cache
        bx      lr
ENDPROC(invaildate_l1_icache)

ENTRY(invaildate_dcache)
	/* Invalidate Data/Unified Caches */
	mrc	p15, 1, r0, c0, c0, 1	   					@ Read CLIDR
	ands	r3, r0, #0x07000000	  					@ Extract coherency level
	mov	r3, r3, lsr #23 	   					@ Total cache levels << 1
	beq	finished		  					@ If 0, no need to clean

	mov	r10, #0 		  					@ R10 holds current cache level << 1
loop1:
	add	r2, r10, r10, lsr #1	  					@ R2 holds cache "Set" position
	mov	r1, r0, lsr r2		   					@ Bottom 3 bits are the Cache-type for this level
	and	r1, r1, #7		   					@ Isolate those lower 3 bits
	cmp	r1, #2
	blt	skip			   					@ No cache or only instruction cache at this level

	mcr	p15, 2, r10, c0, c0, 0	   					@ Write the Cache Size selection register
	isb				   					@ ISB to sync the change to the CacheSizeID reg
	mrc	p15, 1, r1, c0, c0, 0	   					@ Reads current Cache Size ID register
	and	r2, r1, #7		  					@ Extract the line length field
	add	r2, r2, #4		   					@ Add 4 for the line length offset (log2 16 bytes)
	ldr	r4, =0x3FF
	ands	r4, r4, r1, lsr #3	   					@ R4 is the max number on the way size (right aligned)
	clz	r5, r4			   					@ R5 is the bit position of the way size increment
	ldr	r7, =0x7FFF
	ands	r7, r7, r1, lsr #13	   					@ R7 is the max number of the index size (right aligned)

loop2:
	mov	r9, r4			   					@ R9 working copy of the max way size (right aligned)

loop3:
	orr	r11, r10, r9, LSL r5	   					@ Factor in the Way number and cache number into R11
	orr	r11, r11, r7, LSL r2	   					@ Factor in the Set number
	mcr	p15, 0, r11, c7, c6, 2	   					@ Invalidate by Set/Way
	subs	r9, r9, #1		   					@ Decrement the Way number
	bge	loop3
	subs	r7, r7, #1		   					@ Decrement the Set number
	bge	loop2
skip:
	add	r10, r10, #2		   					@ Increment the cache number
	cmp	r3, r10
	bgt	loop1
finished:
	mov	r0, #0
	mcr	p15, 0, r0, c7, c10, 4						@ Drain write buffer

	mcr	p15, 0, r0, c8, c7, 0						@ invalidate I + D TLBs
	mcr	p15, 0, r0, C2, c0, 2						@ TTB Control Register

	bx	lr
ENDPROC(invaildate_dcache)
