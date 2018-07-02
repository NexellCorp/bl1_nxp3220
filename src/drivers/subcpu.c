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

/* @brief: New Scratch 9 in Alive Block */
#define ALIVE_CPUON_FLAG			(0x2008C800 + (4 * 9))

/* external vrriables */
extern unsigned int g_subcpu_ep;

static void subcpu_wfi(void)
{
	__asm__ __volatile__ ("wfi");
}

void subcpu_main(unsigned int id)
{
	if (g_subcpu_ep) {
		mmio_write_32(ALIVE_CPUON_FLAG, (1 << id));
		non_secure_launch(0, g_subcpu_ep);
	} else {
		/* @brief: If not valid, enters WFI state. */
		subcpu_wfi();
	}

	/* @brief: If you exit WFI state, you are defending. */
	while(1);
}
