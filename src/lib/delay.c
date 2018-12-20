/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
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