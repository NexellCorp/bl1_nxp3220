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
#include <psci_main.h>
#include <psci.h>
#include <psci_private.h>

#include <gic.h>

/* External Variable */
extern volatile int g_smc_id;
extern volatile int g_fiq_flag;
extern volatile int g_cpu_kill_num;


/* Macro to help build the psci capabilities bitfield */
#define define_psci_cap(x)		(1 << (x & 0x1f))

unsigned int psci_caps = define_psci_cap(PSCI_CPU_OFF) 			|
			 define_psci_cap(PSCI_CPU_ON_AARCH32)		|
			 define_psci_cap(PSCI_SYSTEM_OFF)		|
			 define_psci_cap(PSCI_SYSTEM_SUSPEND_AARCH32);

/*******************************************************************************
 * PSCI frontend api for servicing SMCs. Described in the PSCI spec.
 ******************************************************************************/
void psci_set_sgi_caller(int cpu_id)
{
#if 0	// Considering that the original sequence.
	int id;

	for (id = 0; id < 4; id++) {
		if (cpu_id != id) {
			g_fiq_flag |= (1 << id);
			system_cpuidle(id, (id + 8));
		}
	}
#else	// Sequences that are usually used.
	// CPU Number : 0x0 Interrupt Number : 8
	cpu_id = cpu_id;
//	system_cpuidle((1 << 0), 8);
#endif
}

unsigned int psci_version(void)
{
	return PSCI_MAJOR_VER | PSCI_MINOR_VER;
}

int psci_cpu_on(unsigned int target_cpu,
		unsigned int entrypoint, unsigned int context_id)
{
	int rc;

	context_id = context_id;

	/* Determine if the cpu exists of not */
	rc = psci_validate_mpidr(target_cpu);
	if (rc != PSCI_E_SUCCESS)
		return PSCI_E_INVALID_PARAMS;

	/* nxp3220(cortex-a7) Actual secondary cpu on */
	rc = psci_cpu_on_start(target_cpu, entrypoint);

	/* @brief: return value (0: on, 1: off, 2:pending) */
	return rc;
}

int psci_cpu_off(void)
{
	struct nx_gicc_reg *cbase
		= (struct nx_gicc_reg *)gicc_get_baseaddr();
	int rc = 0;

	mmio_clear_32(&cbase->ctlr, (3 << 0));

	smp_enable(0);

	rc = psci_do_cpu_off();

	return rc;
}

int psci_system_suspend(unsigned int entrypoint, unsigned int context_id)
{
	int rc = 0;

	context_id = context_id;

	rc = psci_cpu_suspend_start(entrypoint);

	return rc;
}

int psci_affinity_info(unsigned int target_affinity,
		       unsigned int lowest_affinity_level)
{
	unsigned int cpu_id = (target_affinity & 0xF);
	unsigned int status = 0;

	target_affinity = target_affinity;
	lowest_affinity_level = lowest_affinity_level;

	status = system_cpu_check(cpu_id);

	return status;
}

int psci_features(unsigned int psci_fid)
{
	unsigned int local_caps = psci_caps;

	/* TODO: sanity check of psci_fid */

	/* Check if the psci fid is supported or not */
	if (!(local_caps & define_psci_cap(psci_fid)))
		return PSCI_E_NOT_SUPPORTED;

	/* Return 0 for all other fid's */
	return PSCI_E_SUCCESS;
}

int psci_smc_handler(
	unsigned int smc_fid,
	unsigned int x1, unsigned int x2, unsigned int x3)
{

	DBGOUT(" R0: %X, R1: %X, R2: %X, R3: %X \r\n",
		smc_fid, x1, x2, x3);

	/* Check the psci smc_fid  */
	if (((smc_fid >> FUNCID_CC_SHIFT) & FUNCID_CC_MASK) == SMC_32) {
		/* 32-bit PSCI function, clear top parameter bits */

		switch (smc_fid) {
			case PSCI_VERSION:
				return psci_version();

			case PSCI_CPU_OFF:
				return psci_cpu_off();

			case PSCI_CPU_ON_AARCH32:
				return psci_cpu_on(x1, x2, x3);

			case PSCI_AFFINITY_INFO_AARCH32:
				return psci_affinity_info(x1, x2);

			case PSCI_MIG_INFO_TYPE:
				return PSCI_E_NOT_SUPPORTED;

			case PSCI_SYSTEM_SUSPEND_AARCH32:
				return psci_system_suspend(x1, x2);

			case PSCI_SYSTEM_OFF:
				psci_system_off();
				WARN("PSCI_SYSTEM_OFF\r\n");
				/* We should never return from psci_system_off() */
				break;

			case PSCI_SYSTEM_RESET:
				psci_system_reset(x1);
				WARN("PSCI_SYSTEM_RESET\r\n");
				/* We should never return from psci_system_reset() */
				break;

			case PSCI_FEATURES:
				return psci_features(x1);

			default:
				break;
		}
	}

	WARN("Unimplemented PSCI Call: 0x%x \r\n", smc_fid);

	return -1;
}
