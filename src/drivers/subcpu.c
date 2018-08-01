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
#include <subcpu.h>
#include <alive.h>
#include <plat_pm.h>

/* external vrriables */
extern unsigned int g_subcpu_ep;
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
	unsigned int is_secure_os = 0, secure_l;
	unsigned int is_resume = check_suspend_state();

	/* @brief: Parameters to which the BL32 will be delivered. */
	smc_set_fnptr((void*)bl1_smc_handler);

	/* @brief: New Scratch 10 in Alive Block */
	mmio_write_32(&g_vddpwr_reg->new_scratch[10], (1 << id));

	/* @brief: load the loadmark for secure_os */
	is_secure_os = ((mmio_read_32(&g_vddpwr_reg->new_scratch[7]) >> 0) & 0xF);

	if (g_subcpu_ep) {
		if (is_secure_os) {
			secure_l = mmio_read_32(&g_vddpwr_reg->new_scratch[8]);
			secure_launch(is_resume, secure_l, g_subcpu_ep, 0);
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
