/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#define	SET_GLOBAL_VARIABLES

#include <sysheader.h>
#include <pmu.h>
#include <efuse.h>
#include <serial.h>
#include <memory.h>
#include <gic.h>
#include <tz.h>
#include <cpupmu.h>
#include <sys.h>
#include <plat_pm.h>
#include <plat_load.h>
#include <main.h>
#include <hpm.h>
#include <sysreg.h>

/* Global Vriables */
unsigned char g_rsa_public_key[512];

struct nx_bl0_fnptr bl1_fn;
struct nx_hpm_info s_hpm;

void param_set_fnptr(void)
{
	/* @brief: copy and change the boot-loader function pointer */
	g_bl1_fn = ((struct nx_bl0_fnptr *)&bl1_fn);
	g_bl0_fn = ((struct nx_bl0_fnptr *)get_fntpr());
	memcpy((void*)g_bl1_fn, (void*)g_bl0_fn, sizeof(struct nx_bl0_fnptr));
	g_bl1_fn->lib_fn.printchar = printchar;
	set_fnptr((int)g_bl1_fn);

	/* @breif: set the popular gloval variables */
	g_mbedtls = ((struct nx_mbedtls_fnptr *)&g_bl1_fn->mbedtls_fn);
	g_crypto  = ((struct nx_crypto_fntpr  *)&g_bl1_fn->crypt_fn);

	/* @brief: Parameters to which the BL32 will be delivered. */
	smc_set_fnptr((void*)bl1_smc_handler);
}

void bl0_bootmsg_on(unsigned int enable)
{
	int value = get_boption();

	if (enable)
		value &= ~(1 << NOBOOTMSG);
	else
		value |= (1 << NOBOOTMSG);

	set_boption(value);
}

int set_ema(void)
{
	unsigned int ecid[2], ema = 1;
	struct nx_ecid_info pei;

	ecid_parser(&pei);
	if (pei.wafer_no == 6) /*ff : 3, else ema : 1*/
		ema = 3;

	return mc_pmu_set_ema(ema);	/* arm voltage 1: 1.1V, 3: 1.0V, 4: 0.95V */
}

struct nx_syshpm_reg *g_hpmreg =
	((struct nx_syshpm_reg *)0x27001a00);
struct nx_divpll_reg *g_divreg =
	((struct nx_divpll_reg *)0x22000460);
struct nx_cpuhpm_reg *g_hpmreg_c =
	((struct nx_cpuhpm_reg *)0x22000600);
void main(void)
{
	struct nx_bootmanager bm, *pbm;
	int device = ((get_boption() >> BOOTMODE) & 0x7);
	int serial_ch = g_nsih->serial_ch;
	int cpu_id = get_cpuid();
	int s_early = false;
	unsigned int is_resume = FALSE;

	/* @brief: Disable the No Boot Message option on BL0. */
	bl0_bootmsg_on(TRUE);

	set_ema();

	is_resume = check_suspend_state();

	pbm = ((struct nx_bootmanager *)&bm);
	param_set_fnptr();

#ifdef EARLY_SERIAL
	s_early = serial_init(serial_ch);
#endif
	mmio_write_32(&g_hpmreg->hpm0_sys, 0x2);
	pll_initialize(500);
	read_hpm_core(&s_hpm);
	pll_initialize(400);
	hpm_parser(&s_hpm);
//	printf("[kk] syshpm: 0x%x\n\r", g_hpmreg->hpm0_sys);
//	printf("[kk] syshpm_div: 0x%x\n\r", g_hpmreg->div_v);
//	printf("[kk] cpuhpm: 0x%x\n\r", g_hpmreg_c->hpm0_cpu);
//	printf("[kk] cpuhpm_div: 0x%x\n\r", g_hpmreg_c->div_v1);
//	printf("[kk] divpll: 0x%x\n\r", g_divreg->div_sys);
//
	/* @brief: copy the key(bootkey + userkey) -> secure memory */
	memcpy((void*)g_rsa_public_key, (void*)BOOTKEY_BASEADDR, 512);

	if (s_early != true)
		serial_init(serial_ch);

	build_information();

	cpupmu_initialize();

	/* @brief: Before setting tzpc, turn on power to access usb-block. */
	if (device != USBBOOT)
		usb_blk_pwrup();

	gic_initialize(cpu_id);

	tz_initialize();

	system_initialize();

	plat_load(is_resume, pbm);
}
