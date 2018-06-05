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
#include <sysheader.h>
#include <plat_pm.h>
#include <gic.h>
#include <psci.h>
#include <psci_private.h>


/* External Variable */
extern volatile int g_fiq_flag;
extern volatile int g_cpu_kill_num;

void psci_cpu_off_handler(void)
{
	int cpu_id = armv7_get_cpuid();
	int ret;

	/* It is necessary in order to ensure sequential operation.*/
	if (cpu_id != 0) {
		g_fiq_flag |= (1 << cpu_id);
		do {
//			cache_delay_ms(0xFFFFF);
		} while(g_fiq_flag & (1 << cpu_id));

		return;
	}

	/* cpu0 operated to the subcpu power off*/
	ret = system_cpu_off(g_cpu_kill_num);
	if (ret > 0)
		g_fiq_flag = 0;
}

/*************************************************************
 * Must be nxp3220
 * CPU Power Off sequence in nxp3220
 * Reference is made to function psci interface .
 *************************************************************/
int psci_do_cpu_off(void)
{
	unsigned int cpu_id = armv7_get_cpuid();

	system_cpu_off(cpu_id);

	return system_cpu_check(cpu_id);	// 0: ON, 1:OFF, 2:PENDING
}
