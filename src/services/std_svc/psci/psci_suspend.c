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
#include <psci_private.h>

int psci_cpu_suspend_start(unsigned int entrypoint)
{
	system_suspend(entrypoint);

	return PSCI_E_SUCCESS;
}

/*************************************************************
 * Designed to meet the the PSCI Common Interface.
 *************************************************************/
void psci_cpu_suspend_finish(unsigned int cpu_idx,
			     psci_power_state_t *state_info)
{
	/* Remove warning for futrue externsibility */
	cpu_idx = cpu_idx;
	state_info = state_info;

	/* the function for system wakeup */
	system_resume();
}
