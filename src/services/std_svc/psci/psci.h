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
#ifndef __PSCI_H__
#define __PSCI_H__

#define PLAT_MAX_PWR_LVL	1

/*******************************************************************************
 * PSCI version
 ******************************************************************************/
#define PSCI_MAJOR_VER		(1 << 16)
#define PSCI_MINOR_VER		0x0

/*******************************************************************************
 * PSCI error codes
 ******************************************************************************/
#define PSCI_E_SUCCESS		0
#define PSCI_E_NOT_SUPPORTED	-1
#define PSCI_E_INVALID_PARAMS	-2
#define PSCI_E_DENIED		-3
#define PSCI_E_ALREADY_ON	-4
#define PSCI_E_ON_PENDING	-5
#define PSCI_E_INTERN_FAIL	-6
#define PSCI_E_NOT_PRESENT	-7
#define PSCI_E_DISABLED		-8
#define PSCI_E_INVALID_ADDRESS	-9

/*
 * Macro to represent invalid affinity level within PSCI.
 */
#define PSCI_INVALID_PWR_LVL	(PLAT_MAX_PWR_LVL + 1)

/*
 * Type for representing the local power state at a particular level.
 */
typedef unsigned char plat_local_state_t;

/* The local state macro used to represent RUN state. */
#define PSCI_LOCAL_STATE_RUN  	0

/*****************************************************************************
 * This data structure defines the representation of the power state parameter
 * for its exchange between the generic PSCI code and the platform port. For
 * example, it is used by the platform port to specify the requested power
 * states during a power management operation. It is used by the generic code to
 * inform the platform about the target power states that each level should
 * enter.
 ****************************************************************************/
typedef struct psci_power_state {
	/*
	 * The pwr_domain_state[] stores the local power state at each level
	 * for the CPU.
	 */
	plat_local_state_t pwr_domain_state[PLAT_MAX_PWR_LVL + 1];
} psci_power_state_t;

/*******************************************************************************
 * Structure populated by platform specific code to export routines which
 * perform common low level power management functions
 ******************************************************************************/
typedef struct plat_psci_ops {
	void (*cpu_standby)(plat_local_state_t cpu_state);
	int (*pwr_domain_on)(unsigned long mpidr);
	void (*pwr_domain_off)(const psci_power_state_t *target_state);
	void (*pwr_domain_suspend)(const psci_power_state_t *target_state);
	void (*pwr_domain_on_finish)(const psci_power_state_t *target_state);
	void (*pwr_domain_suspend_finish)(
				const psci_power_state_t *target_state);
	void (*system_off)(void) ;
	void (*system_reset)(void) ;
	int (*validate_power_state)(unsigned int power_state,
				    psci_power_state_t *req_state);
	int (*validate_ns_entrypoint)(unsigned long long ns_entrypoint);
	void (*get_sys_suspend_power_state)(
				    psci_power_state_t *req_state);
} plat_psci_ops_t;

#endif /* __PSCI_H__ */
