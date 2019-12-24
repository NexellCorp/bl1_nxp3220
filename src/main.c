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

/* ASV choice table based on HPM and IDS values */
struct asv_table {
        unsigned int hpm_limit; /* HPM value to decide target group */
        unsigned int ids_limit; /* IDS value to decide target group */
};

/* ASV table has 5 levels */
static struct asv_table nxp3220_limit[] = {
	/* HPM, IDS */
        { 320, 3},		/*ASV0*/
        { 330, 4},		/*ASV1*/
        { 340, 5},		/*ASV2*/
        { 350, 6},		/*ASV3*/
        { 360, 7},		/*ASV4*/
        { 370, 9},		/*ASV5*/
        { 380, 11},		/*ASV6*/
        { 390, 13},		/*ASV7*/
        { 400, 16},		/*ASV8*/
        { 410, 19},		/*ASV9*/
        { 420, 24},		/*ASV10*/
        { 430, 29},		/*ASV11*/
        { 440, 35},		/*ASV12*/
        { 450, 43},		/*ASV13*/
        { 460, 52},		/*ASV14*/
        { 470, 63},		/*ASV15*/
        { 480, 77},		/*ASV16*/
        { 490, 93},		/*ASV17*/
        { 500, 113},	/*ASV18*/
        { 510, 137},	/*ASV19*/
        { 999, 999},	/* Reserved Group */
};

struct nx_ecid_info pei;
static unsigned int asv_group = 1;

int set_ema(void)
{
	unsigned int ema = 1;
	unsigned int hpm7;
	int asv_i = 0;

	ecid_parser(&pei);
	hpm7 = (pei.hpm_ids[2] >>6) & 0x3ff;

	for (asv_i = 0; asv_i < 21; asv_i++) {
		if (( pei.ids <= nxp3220_limit[asv_i].ids_limit) || \
			(hpm7 <= nxp3220_limit[asv_i].hpm_limit)) {
		break;
		}
	}

	asv_group = asv_i;

	/* ff : asv13 ~ asv19*/
	if (asv_group >= 13) /*ff : 3, else ema : 1*/
		ema = 3;
	else
		ema = 1;

	return mc_pmu_set_ema(ema);
}

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
	/* @brief: copy the key(bootkey + userkey) -> secure memory */
	memcpy((void*)g_rsa_public_key, (void*)BOOTKEY_BASEADDR, 512);

	if (s_early != true)
		serial_init(serial_ch);

	build_information();
	SYSMSG("[bl1] ids:%d, cpu_hpm:%d, asv_group:%d\n\r", pei.ids, (pei.hpm_ids[2]>>6)&0x3FF, asv_group);

	cpupmu_initialize();

	/* @brief: Before setting tzpc, turn on power to access usb-block. */
	if (device != USBBOOT)
		usb_blk_pwrup();

	gic_initialize(cpu_id);

	tz_initialize();

	system_initialize();

	plat_load(is_resume, pbm);
}
