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
#include "psci.h"

/* External function */
extern int arm_check_mpidr(unsigned int mpidr);

/*******************************************************************************
 * Simple routine to determine whether a mpidr is valid or not.
 ******************************************************************************/
int psci_validate_mpidr(unsigned int mpidr)
{
	if (arm_check_mpidr(mpidr) < 0)
		return PSCI_E_INVALID_PARAMS;

	return PSCI_E_SUCCESS;
}
