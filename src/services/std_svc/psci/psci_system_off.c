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

void psci_system_off(void)
{
	system_power_off();
}

/*************************************************************
 * System Reset the Reference Fucntion
 *************************************************************/
void psci_system_reset(unsigned int reason)
{
	reason = reason;

	system_core_reset();
}
