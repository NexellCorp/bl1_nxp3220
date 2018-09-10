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
#include <delay.h>
#include <armv7_pmu.h>
#include <libarmv7.h>
#include <bootoption.h>

void udelay(volatile unsigned int us)
{

#if 1
	pmu_delay_us(us);
#else
	unsigned int option = get_boption();

	cpu_udelay(1 * us);								// 125MHz
	if (!((!!(option & 1 << SPEEDUP)) ^ (!!(option & 1 << EXSPEEDUP_SAVE))))
		cpu_udelay(125 * us);						// 500MHz
	else if ((option & 1 << SPEEDUP) && !(option & 1 << EXSPEEDUP_SAVE))
		cpu_udelay(200 * us);						// 800MHz
#endif
}

void mdelay(volatile unsigned int ms)
{
	udelay(ms * 1000);
}

void ldelay(volatile unsigned int delay)
{
	volatile unsigned int i, j;

	for (i = 0; i < 1000; i++)
		for (j = 0; j < delay *10; j++);
}