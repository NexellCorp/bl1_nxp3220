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
static struct s_rw_filter g_s_rw_list[] =
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
	/* Index 7 - eFuse bank */
	{
		(PHY_BASEADDR_ECID_SECURE_MODULE),
		(PHY_BASEADDR_ECID_SECURE_MODULE + 0x530),
		0xFFFFFFFF
	},
};

#define ARRAY_SIZE(x) 	(sizeof(x) / sizeof((x)[0]))
#define S_RW_LIST_SIZE	((int)ARRAY_SIZE(g_s_rw_list))

/* if access ok, return 1 */
static unsigned int is_valid_access(unsigned int addr)
{
	struct s_rw_filter *p = g_s_rw_list;
	int i;

	for (i = 0; i < S_RW_LIST_SIZE; i++) {
		if ((p[i].start <= addr) && (p[i].end > addr))
			return 1;
	}

	return 0;
}

int secure_write_32(void *addr, unsigned int value)
{
	/* Check the Secure R/W Memory */
	if (!is_valid_access((unsigned int)addr))
		return -1;

	mmio_write_32(addr, value);

	return 0;
}

int secure_read_32(void *addr)
{
	/* Check the Secure R/W Memory */
	if (!is_valid_access((unsigned int)addr))
		return -1;

	return mmio_read_32(addr);
}
