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
#include <efuse.h>

static struct nx_efuse_reg  *g_efuse_reg
	= ((struct nx_efuse_reg *)PHY_BASEADDR_ECID_SECURE_MODULE);

static const char gst36StrTable[36] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z'
};

static void lotid_num_to_string(unsigned int lotid, char str[6])
{
	unsigned int value[3];
	unsigned int mad[3];

	value[0] = lotid / 36;
	mad[0]   = lotid % 36;

	value[1] = value[0] / 36;
	mad[1]   = value[0] % 36;

	value[2] = value[1] / 36;
	mad[2]   = value[1] % 36;

	/* Lot ID String */
	str[0] = 'N';
	str[1] = gst36StrTable[ value[2] ];
	str[2] = gst36StrTable[ mad[2] ];
	str[3] = gst36StrTable[ mad[1] ];
	str[4] = gst36StrTable[ mad[0] ];
	str[5] = '\0';
}

static unsigned int mtol(unsigned int data, unsigned int bits)
{
	unsigned int result = 0;
	unsigned int i, mask = 1;

	for (i = 0; i < bits ; i++) {
		if (data & (1 << i))
			result |= (mask << (bits - i - 1));
	}
	return result;
}

/* Since this code is chip dependent, it can be changed. Will be implemented later. */
void ecid_parser(struct nx_ecid_info *pei)
{
	unsigned int ecid[4], i;

	for (i = 0; i < 4; i++)
		ecid[i] = mmio_read_32(&g_efuse_reg->ecid[i]);

	pei->lotid	= mtol((ecid[0] & 0x1FFFFF), 21);
	pei->wafer_no	= mtol((ecid[0] >> 21) & 0x1F, 5);
	pei->x_pos	= mtol(((ecid[0] >> 26) & 0x3F)
				| ((ecid[1] & 0x3) << 6), 8);
	pei->y_pos	= mtol((ecid[1] >>  2) & 0xFF, 8);
	pei->ids	= mtol((ecid[1] >> 16) & 0xFF, 8);
	pei->ro		= mtol((ecid[1] >> 24) & 0xFF, 8);
	pei->usb_product_id	= (ecid[3] & 0xFFFF);
	pei->usb_vendor_id	= (ecid[3]>>16) & 0xFFFF;

	lotid_num_to_string(pei->lotid, pei->str_lotid);
}

void ecid_information(struct nx_ecid_info *pei)
{
	DBGOUT("  Lot ID \t Wafer No\tX Pos\tY Pos\tIDS\tRO\tPID\t\tVID \r\r\n");
	DBGOUT(" %8s\t%8d\t%d\t%d\t%d\t%d\t%08X\t%08X\r\r\n",
		pei->str_lotid, pei->wafer_no, pei->x_pos, pei->y_pos, pei->ids,
		pei->ro, pei->usb_product_id, pei->usb_vendor_id );
}

int efuse_get_bootcfg(void)
{
	return mmio_read_32(&g_efuse_reg->boot_cfg);
}

int efuse_get_ecid(int index)
{
	return mmio_read_32(&g_efuse_reg->ecid[index]);
}

int efuse_get_cfg(void)
{
	return mmio_read_32(&g_efuse_reg->efuse_cfg);
}
