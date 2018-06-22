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
#include <sdmmc.h>
#include <sdxcboot.h>
#include <efuse.h>
#include <cmu.h>

/* SDMMC  Boot Code Debug Message */
#define SDMMC_DRVDBG_ON			0

#if (defined(DBGLOG_ON) && SDMMC_DRVDBG_ON)
#define DRV_DBGOUT(...)		printf("DEBUG-SDMMC :  ", __VA_ARGS__)
#else
#define DRV_DBGOUT(...)		empty_printf("DEBUG-SDMMC  :  ", __VA_ARGS__)
#endif

#define SDMMC_SRC_CLK_FREQ	(200 * 1000 * 1000)				// 200Mhz

/* Global Variables */
static struct nx_sdmmc_fnptr *g_sdmmcfn;

static int sdmmc_get_baseaddr(int module)
{
	int base[3] = {
		PHY_BASEADDR_SDMMC0_MODULE,
		PHY_BASEADDR_SDMMC1_MODULE,
		PHY_BASEADDR_SDMMC2_MODULE
	};

	return base[module];
}

static int sdmmc_get_clk_index(int module)
{
	int index[3] = {
		SDMMC_0_CORE_CLK,
		SDMMC_1_CORE_CLK,
		SDMMC_2_CORE_CLK,
	};

	return index[module];
}

static void sdmmc_set_clk_freq(int module, int freq)
{
	int index = sdmmc_get_clk_index(module);

	cmu_set_rate(index, freq);
	cmu_clk_divstop(index, FALSE);
	cmu_clkgrp_enable(index, TRUE);
}

int sdmmc_init(sdxcboot_status *pst)
{
	struct nx_sdmmc_reg *const base
		= ((struct nx_sdmmc_reg *)sdmmc_get_baseaddr(pst->sd_port));

	/* set the source clock on the sdmmc controller */
	sdmmc_set_clk_freq(pst->sd_port, SDMMC_SRC_CLK_FREQ);

	mmio_write_32(&base->pwren, (0 << 0));					// Power Disable

	mmio_write_32(&base->clkena, SDXC_CLKENA_LOWPWR);			// low power mode & clock disable

	mmio_write_32(&base->tiedrvphase, (SDMMC_CLOCK_SHIFT_180 << 8));
	mmio_write_32(&base->tiesmpphase, (SDMMC_CLOCK_SHIFT_0   << 8));

	mmio_write_32(&base->clksrc, 0);					// Clock Divier
	mmio_write_32(&base->clkdiv, ((1 >> 1) << 8) |
				     ((1 >> 1) << 0));				// 2*n Divier (0 : bypass)
	/* fifo mode, not read wait(only use sdio mode) */
	mmio_clear_32(&base->ctrl, (SDXC_CTRL_DMAMODE_EN | SDXC_CTRL_READWAIT));

	/* reset the controller & dma interface & fifo */
	mmio_write_32(&base->ctrl, (SDXC_CTRL_DMARST  |
				    SDXC_CTRL_FIFORST |
				    SDXC_CTRL_CTRLRST));
	while (mmio_read_32(&base->ctrl) &
		(SDXC_CTRL_DMARST | SDXC_CTRL_FIFORST | SDXC_CTRL_CTRLRST));
	mmio_write_32(&base->pwren, (0x1 << 0));				// Set Power Enable

	/* data timeout = 0xFFFFFF, response timeout = 0x64 */
	mmio_write_32(&base->tmout, (0xFFFFFFU << 8) | (0x64 << 0));

	/* data bus width : 0(1-bit), 1(4-bit) */
	mmio_write_32(&base->ctype, 0);

	/* block size */
	mmio_write_32(&base->blksiz, SDMMC_BLOCK_LENGTH);

	/* issue when rx fifo count >= 8 x 4 bytes & tx fifo count <= 8 x 4 bytes */
	mmio_write_32(&base->fifoth, ((8 - 1) << 16) |				// Rx threshold
				      (8 << 0));				// Tx threshold
	/* mask & clear all interrupts */
	mmio_write_32(&base->intmask, 0);
	mmio_write_32(&base->rintsts, 0xFFFFFFFF);

	/* wake up & power on fifo sram */
	mmio_write_32(&base->tiesram, 0x3);
	mmio_write_32(&base->tiemode, 1);

	return TRUE;
}

int emmcboot_read(struct nx_bootmanager *pbm, sdxcboot_status *pst)
{
	struct sbi_header *pbi = (struct sbi_header *)&pbm->bi;
	unsigned int sector_cnt;
	unsigned int rns = 0;
//	char thash[512];

	/* step 01. decrypt the boot-header */
	if (g_sdmmcfn->sdmmc_readbootsector(pst, 1, (unsigned int *)pbi) == FALSE) {
		ERROR("Boot header read Fail!! \r\n");
		return FALSE;
	}
	rns += 1;

	/* check the nexell signature */
	if (pbi->signature != HEADER_ID) {
		ERROR("No Boot Header (%08X)!! \r\n", pbi->signature);
		return FALSE;
	}

	DBGOUT("load_addr: %x load_size :%x launch_addr: %x. \r\n",
		pbi->load_addr, pbi->load_size, pbi->launch_addr);

	/*
	 * SD/eMMC is because reading the blocks in the unit 512,
	 * and calculates the size for correction.
	 */
	sector_cnt = ((pbi->load_size + (SDMMC_BLOCK_LENGTH - 1))
				/ SDMMC_BLOCK_LENGTH);

	g_sdmmcfn->sdmmc_readbootsector(pst, sector_cnt,
			(unsigned int*)pbi->load_addr);

	return TRUE;
}

int emmcboot_normal(struct nx_bootmanager *pbm, sdxcboot_status *pst,
		unsigned int option)
{
	register unsigned int sd_speed, bitwidth;
	register int ret = FALSE;

	if (pst->bhigh_speed == TRUE)
		sd_speed = SDXC_CLKDIV_HIGH;
	else
		sd_speed = SDXC_CLKDIV_LOW;

	if ((pst->sd_port == 0) && (!(option & 1 << eMMCBUSWIDTH)))
		bitwidth = 8;
	else
		bitwidth = 4;
	DRV_DBGOUT("eMMC %s speed %d-bits Normal mode boot.\r\n",
		sd_speed ? "High" : "Normal", bitwidth);

	ret = emmcboot_read(pbm, pst);

	return ret;
}

int emmcboot_alt(struct nx_bootmanager *pbm, sdxcboot_status *pst,
		unsigned int option)
{
	register unsigned int sd_speed, bitwidth;
	register int ret = FALSE;

	if (pst->bhigh_speed == TRUE)
		sd_speed = SDXC_CLKDIV_HIGH;
	else
		sd_speed = SDXC_CLKDIV_LOW;

	if ((pst->sd_port == 0) && (!(option & 1 << eMMCBUSWIDTH)))
		bitwidth = 8;
	else
		bitwidth = 4;
	DRV_DBGOUT("eMMC %s speed %d-bits Alternate mode boot.\r\n",
		sd_speed ? "High" : "Normal", bitwidth);

	ret = emmcboot_read(pbm, pst);

	return ret;
}

int sdmmcboot(struct nx_bootmanager* pbm, sdxcboot_status *pbt_st, unsigned int option)
{
	struct nx_sdmmc_reg *const base
		= ((struct nx_sdmmc_reg *)sdmmc_get_baseaddr(pbt_st->sd_port));
	unsigned int *f_sector = (unsigned int *)&pbm->bi;
	unsigned int sector_cnt;
	unsigned int rsn = 0;
	int ret = FALSE;

	/* step 01-1. open the sd/emmc device */
	if (TRUE != g_sdmmcfn->sdmmc_open(pbt_st, option)) {
		ERROR("Device Open Fail\r\r\n");
		goto error;
	}

	/* step 01-2. check the fifo status */
	if (0 == (mmio_read_32(&base->status) & SDXC_STATUS_FIFOEMPTY)) {
		volatile unsigned int timeout = 0x100000;
		mmio_write_32(&base->ctrl, SDXC_CTRL_FIFORST);
		/* Wait until the FIFO reset is completed. */
		while ((mmio_read_32(&base->ctrl) & SDXC_CTRL_FIFORST) && timeout--)
			mmio_read_32(&base->ctrl);
	}

	/* step 01-3. read the first sectors (GPT or Boot-Header) */
	if (g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn++, 1, f_sector) == FALSE) {
		ERROR("MBR Read Fail.\r\n");
		goto error;
	}
	/* step 01-4. check the GPT */
	ret = g_bl1_fn->gpt_fn.is_gpt_part((unsigned char *)f_sector);
	if (g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn++, 1, f_sector) == FALSE) {
		ERROR("%s Header Read Fail! \r\n", ret ? "Boot" : "GPT");
		goto error;
	}
	/* step 01-5. check the GPT Vaild! */
	if (!ret) {
		ret = g_bl1_fn->gpt_fn.is_gpt_valid((unsigned char *)f_sector);
		if (!ret) {
			rsn = g_bl1_fn->gpt_fn.get_first_empty_lba((unsigned char *)f_sector);
			INFO("Read bootimage from sector %d. \r\n", rsn);
			if (g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn++, 1,
					f_sector) == FALSE) {
				ERROR("Boot Header read fail. \r\n");
				goto error;
			}
		} else
			WARN("invalid GPT.\r\n");
	} else
		WARN("not GPT, Read header from sector 1!! \r\n");

	rsn = (g_nsih->dbi.device_addr / SDMMC_BLOCK_LENGTH);
	DRV_DBGOUT("Device_addr: %X(%X), RSN: %d \r\n",
		g_nsih->dbi.device_addr, &g_nsih->dbi.device_addr, rsn);
	/* Header + Hash */
	if (g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn++, 1, f_sector) == FALSE) {
		ERROR("%s Header Read Fail! \r\n", ret ? "Boot" : "GPT");
		goto error;
	}

	struct sbi_header *pbi = (struct sbi_header *)&pbm->bi;
	printf("pbi: 0x%08X , g_sdmmcfn->sdmmc_read_sectors: 0x%08X \r\n",
			pbi, g_sdmmcfn->sdmmc_read_sectors);
	/* step 01-6. check the nexell signature */
	if (pbi->signature != HEADER_ID) {
		ERROR("Not bootable image (%08x).\r\n", pbi->signature);
		return 0;
	}

	DRV_DBGOUT("Load Addr: 0x%X, Load Size:0x%X, Launch Addr: 0x%08X. \r\n",
		pbi->load_addr, pbi->load_size, pbi->launch_addr);

	/*
	 * SD/eMMC is because reading the blocks in the unit 512,
	 * and calculates the size for correction.
	 */
	sector_cnt = ((pbi->load_size + (SDMMC_BLOCK_LENGTH - 1))
				/ SDMMC_BLOCK_LENGTH);

	ret = g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn++,
				sector_cnt, (unsigned int*)pbi->load_addr);
error:
	return ret;
}

unsigned int sdxcboot(struct nx_bootmanager *pbm, unsigned int option)
{
	sdxcboot_status st, *pst;
	int ret = FALSE;

	g_sdmmcfn = ((struct nx_sdmmc_fnptr *)&g_bl1_fn->sdmmc_fn);

	pst = &st;
	pst->sd_port = ((option >> SELSDPORT) & 0x3);


	if (pst->sd_port >= 3) {
		pst->sd_port = 0;
		pst->bhigh_speed = TRUE;
	} else {
		pst->sd_port = (2 - pst->sd_port);
		pst->bhigh_speed = FALSE;
	}
	printf("SD boot: %d\r\n", pst->sd_port);

	g_sdmmcfn->sdpad_setalt(pst->sd_port);

	sdmmc_init(pst);

	DRV_DBGOUT("SD boot: %d\r\n", pst->sd_port);
	ret = sdmmcboot(pbm, pst, option);

	g_sdmmcfn->sdmmc_close(pst);
	g_sdmmcfn->sdmmc_terminate(pst);

	g_sdmmcfn->sdpad_setgpio(pst->sd_port);

	return ret;
}

int emmcboot(struct nx_bootmanager *pbm, unsigned int option)
{
	sdxcboot_status st, *pst;
	int ret = FALSE;

	g_sdmmcfn = ((struct nx_sdmmc_fnptr *)&g_bl1_fn->sdmmc_fn);

	pst = &st;
	pst->sd_port = ((option >> SELSDPORT) & 0x3);

	if (pst->sd_port >= 3) {
		pst->sd_port = 0;
		pst->bhigh_speed = TRUE;
	} else
		pst->bhigh_speed = FALSE;

	/* eMMC or MMC ver 4.3+ */
	DRV_DBGOUT("eMMC boot: %d \r\n", pst->sd_port);
	if ((option & 1 << eMMCBOOTMODE) != 0)
		ret = emmcboot_alt(pbm, pst, option);
	else
		ret = emmcboot_normal(pbm, pst, option);

	return ret;
}
