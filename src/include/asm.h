/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __ASM_H__
#define __ASM_H__

#ifndef ASM_NL
#define ASM_NL		 ;
#endif

#define SYMBOL_NAME_STR(X)	#X
#define SYMBOL_NAME(X)		X
#define SYMBOL_NAME_LABEL(X)	X:

#ifndef __ALIGN
#define __ALIGN .align		4
#endif

#ifndef __ALIGN_STR
#define __ALIGN_STR		".align 4"
#endif

#define ALIGN			__ALIGN
#define ALIGN_STR		__ALIGN_STR

#define EXPORT(name)			\
	.globl SYMBOL_NAME(name) ASM_NL

#define LENTRY(name) 			\
	ALIGN ASM_NL			\
	SYMBOL_NAME_LABEL(name)

#define ENTRY(name)			\
	.globl SYMBOL_NAME(name) ASM_NL \
	LENTRY(name)

#define WEAK(name)			\
	.weak SYMBOL_NAME(name) ASM_NL	\
	LENTRY(name)

#ifndef END
#define END(name)			\
	.size name, .-name
#endif

#ifndef ENDPROC
#define ENDPROC(name)			\
	.type name STT_FUNC ASM_NL	\
	END(name)
#endif

#endif
