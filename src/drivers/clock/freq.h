/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __CFG_FREQ_H__
#define __CFG_FREQ_H__

/* clock (PLLx) configuration */
#define OSC_HZ					(24000000)
#define OSC_KHZ					(OSC_HZ/1000)
#define OSC_MHZ					(OSC_KHZ/1000)

#define CLKSRC_PLL_0				0
#define CLKSRC_PLL_1				1
#define CLKSRC_PLL_2				2
#define CLKSRC_PLL_3				3

#endif // __CFG_FREQ_H__
