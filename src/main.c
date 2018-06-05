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
#define	SET_GLOBAL_VARIABLES

#include <sysheader.h>
#include <pmu.h>
#include <serial.h>
#include <cpupmu.h>
#include <memory.h>
#include <tz.h>
#include <sysreg.h>
#include <plat_pm.h>
#include <plat_load.h>
#include <main.h>

/* Global Vriables */
unsigned char g_rsa_public_key[512];

struct nx_bl0_fnptr bl1_fn;

void param_set_fnptr(void)
{
	/* @brief: copy and change the boot-loader function pointer */
	g_bl1_fn = ((struct nx_bl0_fnptr *)&bl1_fn);
	g_bl0_fn = ((struct nx_bl0_fnptr *)get_fntpr());
	memcpy((void*)g_bl1_fn, (void*)g_bl0_fn, sizeof(struct nx_bl0_fnptr));
	g_bl1_fn->lib_fn.printchar = printchar;

	/* @breif: set the popular gloval variables */
	g_mbedtls = ((struct nx_mbedtls_fnptr *)&g_bl1_fn->mbedtls_fn);
	g_crypto  = ((struct nx_crypto_fntpr  *)&g_bl1_fn->crypt_fn);

	/* @brief: Parameters to which the BL32 will be delivered. */
	smc_set_fnptr((void*)bl1_smc_handler);
}
#include <alive.h>
void main(void)
{
	struct nx_bootmanager bm, *pbm;
	int serial_ch = g_nsih->serial_ch;
	int s_early = false;
	int is_resume = 0;

	is_resume = check_suspend_state();

	pbm = ((struct nx_bootmanager *)&bm);
	param_set_fnptr();

#ifdef EARLY_SERIAL
	s_early = serial_init(serial_ch);
#endif
	/* @brief: copy the key(bootkey + userkey) -> secure memory */
	memcpy((void*)g_rsa_public_key, (void*)BOOTKEY_BASEADDR, 512);

	if (s_early != true)
		serial_init(serial_ch);

	build_information();

	cpupmu_initialize();

	tz_initialize();

	nx_gpio_bit_access();

	vddpwron_ddr_on();

	/* @brief: NIC400 GPV (DDR Security0 : Slave) */
	mmio_write_32((PHY_BASEADDR_SYS_BUS_GPV + 0x8), 1);

	plat_load(is_resume, pbm);
}
