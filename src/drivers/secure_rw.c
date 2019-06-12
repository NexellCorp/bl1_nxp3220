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
#include <secure_rw.h>

/* Gloval Variables (To Do..After.. ) */
static struct s_rw_filter g_s_rw_list[7] =
{
	/* Index 0 - Alive GPIO */
	{
		PHY_BASEADDR_ALIVE,
		(PHY_BASEADDR_ALIVE + 0x25C),
		0xFFFFFFFF
	},
	/* Index 1 - eFuse HPM */
	{
		(PHY_BASEADDR_ECID_SECURE_MODULE + 0x530),
		(PHY_BASEADDR_ECID_SECURE_MODULE + 0x530 + 0xF),
		0xFFFFFFFF
	},
	/* Index 2 - SYS BUS GPV M0 */
	{
		(PHY_BASEADDR_SYS_BUS_GPV_M0),
		(PHY_BASEADDR_SYS_BUS_GPV_M0 + 0x138),
		0xFFFFFFFF
	},
	/* Index 3 - SYS BUS GPV M1 */
	{
		(PHY_BASEADDR_SYS_BUS_GPV_M1),
		(PHY_BASEADDR_SYS_BUS_GPV_M1 + 0x138),
		0xFFFFFFFF
	},
	/* Index 4 - SYS BUS GPV M2 */
	{
		(PHY_BASEADDR_SYS_BUS_GPV_M2),
		(PHY_BASEADDR_SYS_BUS_GPV_M2 + 0x138),
		0xFFFFFFFF
	},
	/* Index 5 - SYS BUS GPV M3 */
	{
		(PHY_BASEADDR_SYS_BUS_GPV_M3),
		(PHY_BASEADDR_SYS_BUS_GPV_M3 + 0x138),
		0xFFFFFFFF
	},
	/* Index 6 - SYS BUS GPV M4 */
	{
		(PHY_BASEADDR_SYS_BUS_GPV_M4),
		(PHY_BASEADDR_SYS_BUS_GPV_M4 + 0x138),
		0xFFFFFFFF
	},
};

static unsigned int check_rw_list(unsigned int addr, unsigned int value)
{
	volatile unsigned int index = 0, cnt;

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
	volatile unsigned int cal_value;

	/* Check the Secure R/W Memory */
	cal_value = check_rw_list((unsigned int)addr, value);

	return mmio_write_32(addr, cal_value);
}

int secure_read_32(void *addr)
{
	volatile unsigned int cal_value, value;

	value = mmio_read_32(addr);

	/* Check the Secure R/W Memory */
	cal_value = check_rw_list((unsigned int)addr, value);

	return cal_value;
}
