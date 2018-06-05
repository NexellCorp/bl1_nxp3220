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
#ifndef __PSCI_PRIVATE_H__
#define __PSCI_PRIVATE_H__

#include <psci.h>

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
/* Private exported functions from psci_common.c */
int  psci_validate_mpidr(unsigned int mpidr);
void psci_power_down_wfi(void);

/* Private exported functions from psci_on.c */
int psci_cpu_on_start(unsigned int target_cpu, unsigned int entrypoint);

void psci_cpu_on_finish(unsigned int cpu_idx,	psci_power_state_t *state_info);

/* Private exported functions from psci_off.c */
int psci_do_cpu_off(void);

/* Private exported functions from psci_suspend.c */
int psci_cpu_suspend_start(unsigned int entrypoint);
void psci_cpu_suspend_finish(unsigned int cpu_idx,
					psci_power_state_t *state_info);

/* Private exported functions from psci_system_off.c */
void psci_system_off(void);
void psci_system_reset(unsigned int);

#endif /* __PSCI_PRIVATE_H__ */
