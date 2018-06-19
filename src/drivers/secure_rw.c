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
#include <secure_rw.h>

/* Gloval Variables (To Do..After.. ) */
static struct s_rw_filter g_s_rw_list[1] =
{
	/* Index 0 - Alive GPIO */
	{
		PHY_BASEADDR_ALIVE,
		(PHY_BASEADDR_ALIVE + 0x25C),
		0xFFFFFFFF
	},
};

static unsigned int check_rw_list(unsigned int addr, unsigned int value)
{
	unsigned int index = 0, cnt;

	cnt = (sizeof(g_s_rw_list)/sizeof(struct s_rw_filter));

	do {
		if ((g_s_rw_list[index].start <= addr)
			&& (g_s_rw_list[index].end >= addr)) {
			value &= g_s_rw_list[index].mask;
			return value;
		}

	} while(index++ < cnt);

	return value;
}

int secure_write_32(void *addr, unsigned int value)
{
	int cal_value;

	/* @brief: Only Alive GPIO */
	addr += PHY_BASEADDR_ALIVE;

	/* Check the Secure R/W Memory */
	cal_value = check_rw_list((unsigned int)addr, value);

	return mmio_write_32(addr, cal_value);
}

int secure_read_32(void *addr)
{
	int cal_value, value;

	/* @brief: Only Alive GPIO */
	addr += PHY_BASEADDR_ALIVE;

	value = mmio_read_32(addr);

	/* Check the Secure R/W Memory */
	cal_value = check_rw_list((unsigned int)addr, value);

	return cal_value;
}
