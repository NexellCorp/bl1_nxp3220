/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
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
