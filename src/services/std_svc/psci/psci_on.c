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
#include <psci.h>
#include <libarmv7.h>

/*************************************************************
 * Must be nxp3220
 * CPU Power Off sequence in nxp3220
 * Reference is made to function psci interface .
 *************************************************************/
unsigned int g_subcpu_ep;

int psci_cpu_on_start(unsigned int target_cpu, unsigned int entrypoint)
{
	unsigned int cpu_id = ((target_cpu >> 0) & 0xFF);

	g_subcpu_ep = entrypoint;

	system_cpu_on(cpu_id);

	return system_cpu_check(cpu_id);
}
