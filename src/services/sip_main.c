/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#include <sysheader.h>
#include <gic.h>
#include <secure_rw.h>
#include <pmu.h>
#include <tz.h>
#include <sys.h>
#include <plat_load.h>

/* Macro: Dispatcher Scure Service for User */
#define SECURE_HWREG_WRITE		0x82000000
#define SECURE_HWREG_READ		0x82000001
#define SIP_PLATFORM_LOAD		0x82000002
#define PMU_BLK_PWRUP			0x82000003
#define TZPC_ACCESS_DDR			0x82000004

/*******************************************************************************
 * For implementing the functions defiend by the user, the SIP interface the
 * main function
 ******************************************************************************/
int sip_smc_handler(unsigned int smc_fid,
	unsigned int r1, unsigned int r2, unsigned int r3)
{
	r3 = r3;
	int ret = 0;
	switch (smc_fid) {
		case SIP_PLATFORM_LOAD:
			plat_s_load((struct platform_info *)r1);
			break;
		case PMU_BLK_PWRUP:
			pmu_blk_pwrup(r1, r2);
			break;
		case TZPC_ACCESS_DDR:
			tzpc_set_ddr();
			break;
		case SECURE_HWREG_WRITE:
			return secure_write_32((void*)r1, r2);
		case SECURE_HWREG_READ:
			return secure_read_32((void*)r1);
			break;
		default:
			/* FIXME: this can cause data abort */
			/* WARN("Unimplemented SIP Service Call: 0x%x\r\n", smc_fid); */
			break;
	}

	return ret;
}
