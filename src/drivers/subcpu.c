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
#include <subcpu.h>
#include <alive.h>
#include <plat_pm.h>
#include <plat_load.h>

/* external vrriables */
extern unsigned int g_subcpu_ep;
extern struct platform_manager *g_ppi;
extern struct nx_vddpwr_reg *g_vddpwr_reg;

/* external functions */
unsigned int bl1_smc_handler(unsigned int smc_fid,
	unsigned int r1, unsigned int r2, unsigned int r3);
void smc_set_fnptr(unsigned int (*smc_handler));

static void subcpu_wfi(void)
{
	__asm__ __volatile__ ("wfi");
}

void subcpu_main(unsigned int id)
{
	unsigned int is_secure_os = 0;
	unsigned int is_resume = check_suspend_state();

	/* @brief: Parameters to which the BL32 will be delivered. */
	smc_set_fnptr((void*)bl1_smc_handler);

	/* @brief: save the subcpu wakeup mark (for debugt) */
	g_ppi->wakeup_mark = (1 << id);

	/* @brief: load the loadmark for secure_os */
	is_secure_os = ((g_ppi->pi.is_loadmark >> 0) & 0xF);

	if (g_subcpu_ep) {
		if (is_secure_os) {
			secure_launch(is_resume, g_ppi->s_launch_addr, g_subcpu_ep, 0);
		} else {
			non_secure_launch(is_resume, g_subcpu_ep);
		}
	} else {
		/* @brief: If not valid, enters WFI state. */
		subcpu_wfi();
	}

	/* @brief: If you exit WFI state, you are defending. */
	while(1);
}
