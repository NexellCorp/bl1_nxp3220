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
#include <spiboot.h>
#include <spi.h>
#include <cmu.h>

/* SPI  Boot Code Debug Message */
#define SPI_DRVDBG_ON			0

#if (defined(DBGLOG_ON) && SPI_DRVDBG_ON)
#define DRV_DBGOUT(...)		printf("DEBUG-SPI :  ", __VA_ARGS__)
#else
#define DRV_DBGOUT(...)		empty_printf("DEBUG-SPI  :  ", __VA_ARGS__)
#endif

#define SPI_SRCCLK_FREQ		(100 * 1000 * 1000)				// 100 Mhz

/* Global Variables */
static struct nx_spi_reg *g_spi_reg;
static struct nx_spi_fntptr *g_spifn;

static const unsigned int g_clk_num[3][2] = {
	{ SPI_0_APB_CLK, SPI_0_CORE_CLK },
	{ SPI_1_APB_CLK, SPI_1_CORE_CLK },
	{ SPI_2_APB_CLK, SPI_2_CORE_CLK }
};

static int spi_get_baseaddr(int channel)
{
	int baseaddr[3] = {
		PHY_BASEADDR_SPI0_MODULE,
		PHY_BASEADDR_SPI1_MODULE,
		PHY_BASEADDR_SPI2_MODULE
	};

	return baseaddr[channel];
}

static int set_spiclk(int channel, int freq)
{
	cmu_srcoff_enable(g_clk_num[channel][0], FALSE);
	cmu_srcoff_enable(g_clk_num[channel][1], FALSE);

	/* step xx. set the spi - apb clock (default) */
	cmu_set_rate(g_clk_num[channel][0], SPI_SRCCLK_FREQ);

	return cmu_set_rate(g_clk_num[channel][1], freq);
}

void spi_initialize(unsigned int channel)
{
	int reg_value;

	/* step xx. check the maximum channel */
	if (channel == 3)
		channel = 0;

	g_spi_reg = (struct nx_spi_reg*)spi_get_baseaddr(channel);

	/* step xx. set the pad alt-function */
	g_spifn->spipad_setalt(channel);

	/* step xx. set the spi-source clock */
	set_spiclk(channel, SPI_SRCCLK_FREQ);

	/* step xx. spi initialize */
	mmio_write_32(&g_spi_reg->ssienr, 0);					/* spi disable							*/
	mmio_write_32(&g_spi_reg->imr, 0x3F);					/* all int disable;						*/

	reg_value = ((0 << 24) |						/* slave select toggle enable: for eeprom, always low		*/
		     (0 << 21) |						/* 0:1bit(Standard), 1:2bit(Dual), 2:4bit(Quad), 3,8bit(Octal)	*/
		     (7 << 16) |						/* (x + 1) bit frame: 7 ==> 8 bit frame				*/
		     (0 << 12) |						/* Microwire frame format control word				*/
		     (0 << 11) |						/* internal loopback - test mode only				*/
		     (0 << 10) |						/* slave output enable						*/
		     (0 <<  8) |						/* transfer mode. 0:trx, 1:tx, 2:rx, 3:eeprom read mode		*/
		     (0 <<  7) |						/* serial clock prlarity. 0:low inactive, 1:high		*/
		     (0 <<  6) |						/* serial clock phase. 0:middle, 1:start			*/
		     (0 <<  4) |						/* frame format. 0:motorola, 1:texas, 2:Microwire		*/
		     (0 <<  0));						/* Data Frame Size. When MAX XFER == 16 or 32, 4~16bit		*/
	mmio_write_32(&g_spi_reg->ctrlr0, reg_value);
	reg_value = mmio_read_32(&g_spi_reg->ctrlr0);

	/* slave enable, there is only one slave for each controller */
	mmio_write_32(&g_spi_reg->ser, (1 << 0));

	reg_value = 6;								/* 100M/6 = 16.6666MHz, only even divide value			*/
	mmio_write_32(&g_spi_reg->baudr, reg_value);				/* baudrate. even only. in / baud => out, 0: out disable.	*/
}

int spiboot(struct nx_bootmanager *pbm, unsigned int option)
{
	int channel = (option >> SELSPIPORT) & 0x3;
	int addr_step = (2 + ((option & 1 << SERIALFLASHADDR) ? 0 : 1));
	int flash_addr = g_nsih->dbi.device_addr;
	int read_size, extra_size = 256;

	int ret = TRUE;

	/* step 00. get the bl0 function-table address */
	g_spifn = ((struct nx_spi_fntptr *)&g_bl1_fn->spi_fn);

	if (channel == 3)
		channel = 0;

	/* step 01. initialize the spi controller */
	spi_initialize(channel);

	DRV_DBGOUT("%d Step, 0x%08X Device Address!! \r\n", addr_step, flash_addr);

	/* step 02-1. read the boot-header */
	read_size = (sizeof(struct sbi_header) + extra_size);
	DRV_DBGOUT("Read Header!! (NSIH) \r\n");
	g_spifn->read_flash(channel, ((unsigned char *)&pbm->bi),
		flash_addr, read_size, addr_step);
	flash_addr += read_size;

	/* step 02-2. check the nexell signature */
	if (pbm->bi.signature != HEADER_ID) {
		ERROR("Header Signature Failed! (%08x)\r\n", pbm->bi.signature);
		ret = FALSE;
		goto error;
	}

	/* step 03. read the boot-image file */
	DRV_DBGOUT("Read the boot-image !! \r\n");
	read_size = pbm->bi.load_size;
	g_spifn->read_flash(channel, (unsigned char*)pbm->bi.load_addr,
		flash_addr, read_size, addr_step);
	flash_addr += read_size;

	/* step 04. read the signature data */
	DRV_DBGOUT("Read the Hash!! \r\n");
	read_size = sizeof(pbm->rsa_encrypted_sha256_hash);
	g_spifn->read_flash(channel,
		((unsigned char*)&pbm->rsa_encrypted_sha256_hash[0]),
		flash_addr, read_size, addr_step);
	flash_addr += read_size;

error:
	/* step 07. deintialize the spi controller */
//	g_spifn->spi_deinit(option);

	return ret;
}
