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
#include <sss.h>
#include <plat_load.h>

#define SSS_BIN_SRAMSIZE			(32 * 1024)

#define SSS_CM0_BASEADDR			(PHY_BASEADDR_SSS_MODULE_SSS + 0x8000)

#define FCGLOBAL				(PHY_BASEADDR_SSS_MODULE_SSS + 0x18)
#define CM0_LP_CON				(PHY_BASEADDR_SSS_MODULE_SSS + 0x1C)

#define	MB_STATUS				(SSS_MAILBOX_BASEADDR + 0x00)
#define CM0_HRESET				(SSS_MAILBOX_BASEADDR + 0x04)
#define CM0_SRAM_ACCESS_CONTROL			(SSS_MAILBOX_BASEADDR + 0x08)
#define CM0_FW_STATUS				(SSS_MAILBOX_BASEADDR + 0x14)
#define CM0_DEBUG_CONTROL			(SSS_MAILBOX_BASEADDR + 0x34)

#define	MB_DATA00				(SSS_MAILBOX_BASEADDR + 0x100)
#define	MB_DATA01				(SSS_MAILBOX_BASEADDR + 0x104)
#define	MB_DATA02				(SSS_MAILBOX_BASEADDR + 0x108)
#define	MB_DATA03				(SSS_MAILBOX_BASEADDR + 0x10C)
#define	MB_DATA04				(SSS_MAILBOX_BASEADDR + 0x110)

void sssc_reset(unsigned int enable)
{
	if (enable)
		mmio_set_32(FCGLOBAL, (1 << 0));
	else
		mmio_clear_32(FCGLOBAL, (1 << 0));
}

static void run_wait(unsigned int ctrl)
{
	NOTICE("  MB_DATA00 written!! Data= 0x%08x\r\n",ctrl);
	mmio_write_32(MB_DATA00, ctrl);

	while ((mmio_read_32(MB_STATUS) & 0x1) != 0);
}

static unsigned char get_response(void)
{
	unsigned char reg_value;

	reg_value = mmio_read_32(MB_DATA00) & 0xFF;
	return reg_value;
}

static void Read_ResponseData(void)
{
	unsigned int reg_value;
	unsigned int data_num;
	unsigned int data_len_field;
	unsigned char data_len[4];
	unsigned int *data_field_pointer;
	unsigned int i, j;

	data_field_pointer = (unsigned int *)MB_DATA04;
	NOTICE("Response Result \r\n");
	reg_value = mmio_read_32(MB_DATA00) & 0xFF;

	if(reg_value == 0xA1)
		NOTICE("Response Success. (Response Code=%X) \r\n", MB_DATA00);
	else if(reg_value == 0xf1)
		NOTICE("Response Fail. (Response Code=%X) \r\n", MB_DATA00);

	data_num = mmio_read_32(MB_DATA01);
	data_len_field = mmio_read_32(MB_DATA02);
	NOTICE(" Response data_num = %x, data_len = %x \r\n", data_num, data_len_field);

	for(i = 0; i < data_num; i++) {
		data_len[i] = (data_len_field & 0xff);
		data_len_field >>= 8;
		for(j = 0; j < data_len[i]; j++)
			NOTICE(" Data %d[%d]= %08x\r\n", i, j, *data_field_pointer++);
	}
}

static void cm0_boot_control(unsigned int index)
{
	if ((mmio_read_32(CM0_HRESET) & 0x1) == 1) {
		ERROR("CM0 Already Alive(CM0 Reset == 1 \r\n");
		return;
	}

	if (index == 1)
		mmio_set_32(CM0_DEBUG_CONTROL, 0x8);

	if ((mmio_read_32(CM0_DEBUG_CONTROL) & 0x08) != 0)
		NOTICE("Rom Boot Mode \r\n");
	else
		NOTICE("SRAM Boot Mode \r\n");

	NOTICE("SFR: CM0_SRAM_ACCESS_CONTROL = %08x\r\n",
			mmio_read_32(CM0_SRAM_ACCESS_CONTROL));
	NOTICE("SFR: CM0_DEBUG_CONTROL= %08x\r\n",
			mmio_read_32(CM0_DEBUG_CONTROL));
	NOTICE("*** CM0 Reset Deasserted!! \r\n");
	mmio_set_32(CM0_HRESET, 0x1);

	while ((mmio_read_32(MB_STATUS) & 0x1) != 0);	// ???

	NOTICE("CM0 Booting complete!!\r\n");
	NOTICE("CM0_FW_STATUS = %d\r\n", mmio_read_32(CM0_FW_STATUS));
}

int sss_load(struct nx_bootmanager *pbm, unsigned int option)
{
	int success, res;

	success = plat_next_load(pbm, option);
	if (success != FALSE)
		return false;

	/* @brief: CM0 SRAM Locking after firmware loading */
	mmio_set_32(CM0_SRAM_ACCESS_CONTROL, 0x1);
	cm0_boot_control(0);

	NOTICE("== GetInfo() \r\n");
	run_wait(0x00000101);
	res = get_response();
	if (res == 0xF1) {
		Read_ResponseData();
		return 0;
	}

	NOTICE("== Clean \r\n");
	run_wait(0x00000201);
	res = get_response();
	if (res == 0xF1) {
		Read_ResponseData();
		return 0;
	}

	return true;
}
