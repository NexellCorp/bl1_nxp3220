/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __DELAY_H__
#define __DELAY_H__

/* "cpu_delay.S" in Function */
void set_delay_div(volatile unsigned int cnt);
void cpu_udelay(volatile unsigned int us);

void udelay(volatile unsigned int us);
void mdelay(volatile unsigned int ms);
void ldelay(volatile unsigned int delay);

#endif // #ifndef __DELAY_H__
