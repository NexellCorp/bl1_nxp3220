/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
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
