/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
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

#define SDMMC_SRC_CLK_FREQ	(15 * 1000 * 1000)				// 200Mhz

#define SDMMC_BOOT_PARTENB	(0x1)
#define SDMMC_BOOT_PARTNUM	(0x1)
#define SDMMC_BOOT_ACK		(0x0)

/* Global Variables */
static struct nx_sdmmc_fnptr *g_sdmmcfn;
unsigned int emmc_next_bl;

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

	cmu_srcoff_enable(index, FALSE);
	cmu_set_rate(index, freq);
}

#if 0 /* To Do */
unsigned int sdmmc_sendcmd_internal(sdxcboot_status *pst,
					sdmmc_command *pcmd)
{
	struct nx_sdmmc_reg *const base
		= ((struct nx_sdmmc_reg *)sdmmc_get_baseaddr(pst->sd_port));
	unsigned int cmd, flag;
	unsigned int status = 0;
	volatile unsigned int timeout;
	volatile unsigned int reg_value;

	cmd	= pcmd->cmdidx & 0xFF;
	flag	= pcmd->flag;
	mmio_write_32(&base->rintsts, 0xFFFFFFFF);

	/* Send Command */
	timeout = 0;
	do {
		mmio_write_32(&base->rintsts, SDXC_RINTSTS_HLE);
		mmio_write_32(&base->cmdarg, pcmd->arg);
		mmio_write_32(&base->cmd,
			(cmd | flag | SDXC_CMDFLAG_USE_HOLD_REG));
		while (mmio_read_32(&base->cmd) & SDXC_CMDFLAG_STARTCMD) {
			if (++timeout > SDMMC_TIMEOUT) {
				printf("TO Send CMD \r\n");
				status |= SDMMC_STATUS_CMDBUSY;
//				INFINTE_LOOP();
				while(1);
				goto end;
			}
		}
	} while (mmio_read_32(&base->rintsts) & SDXC_RINTSTS_HLE);

	/* Wait until Command sent to card and got response from card. */
	timeout = 0;
	while (1) {
		reg_value = mmio_read_32(&base->rintsts);
		if (reg_value & SDXC_RINTSTS_CD)
			break;

		if (++timeout > SDMMC_TIMEOUT) {
			printf("TO CMD Done! \r\n");
			status |= SDMMC_STATUS_CMDTOUT;
//			INFINTE_LOOP();
			while(1);
			goto end;
		}

		if ((flag & SDXC_CMDFLAG_STOPABORT) &&
			(mmio_read_32(&base->rintsts) & SDXC_RINTSTS_HTO)) {
			/*
			 * You have to clear FIFO when HTO is occurred.
			 * After that, SDXC module leaves in stopped
			 * state and takes next command.
			 */
			while (0 == (mmio_read_32(&base->status)
					& SDXC_STATUS_FIFOEMPTY)) {
				mmio_read_32(&base->data);
			}
		}
	}

	/* Check Response Error */
	reg_value = mmio_read_32(&base->rintsts);
	if (reg_value & (SDXC_RINTSTS_RCRC
				| SDXC_RINTSTS_RE | SDXC_RINTSTS_RTO)) {
		if (reg_value & SDXC_RINTSTS_RCRC)
			status |= SDMMC_STATUS_RESCRCFAIL;
		if (reg_value & SDXC_RINTSTS_RE)
			status |= SDMMC_STATUS_RESERROR;
		if (reg_value & SDXC_RINTSTS_RTO)
			status |= SDMMC_STATUS_RESTOUT;
	}

	if ((status == SDMMC_STATUS_NOERROR) &&
		(flag & SDXC_CMDFLAG_SHORTRSP)) {
		pcmd->response[0] = mmio_read_32(&base->resp[0]);
		if ((flag & SDXC_CMDFLAG_LONGRSP) ==
				SDXC_CMDFLAG_LONGRSP) {
			pcmd->response[1] = mmio_read_32(&base->resp[1]);
			pcmd->response[2] = mmio_read_32(&base->resp[2]);
			pcmd->response[3] = mmio_read_32(&base->resp[3]);
		}

		if (SDMMC_RSPIDX_R1B == ((pcmd->cmdidx >> 8) & 0xFF)) {
			timeout = 0;
			do {
				if (++timeout > SDMMC_TIMEOUT) {
					WARN("TO Card Data Ready! \r\n");
					status |= SDMMC_STATUS_DATABUSY;
//					INFINTE_LOOP();
					while(1);
					goto end;
				}
			} while (mmio_read_32(&base->status) & SDXC_STATUS_DATABUSY);
		}
	}

end:
	if (SDMMC_STATUS_NOERROR != status) {
		printf("CMD: %X, Argument: %X => Status: %X, Resp:%x\n",
			pcmd->cmdidx, pcmd->arg, status, pcmd->response[0]);
	}

	pcmd->status = status;

	return status;
}
#endif

unsigned int sdmmc_send_command(sdxcboot_status *pst, sdmmc_command *pcmd)
{
	unsigned int status;

	status = g_sdmmcfn->sdmmc_sendcommandinternal(pst, pcmd);
	if (SDMMC_STATUS_NOERROR != status)
		g_sdmmcfn->sdmmc_sendstatus(pst);

	return status;
}

static int mmc_switch(sdxcboot_status *pst, unsigned char set,
			unsigned char index, unsigned char value)
{
	sdmmc_command cmd;
	unsigned int status;

	/* Remove Warrning */
	set = set;

	cmd.cmdidx = SWITCH_FUNC;
	cmd.arg = ((MMC_SWITCH_MODE_WRITE_BYTE << 24) |
		   (index << 16) | (value << 8));
	cmd.flag = (SDXC_CMDFLAG_STARTCMD	|
		    SDXC_CMDFLAG_CHKRSPCRC	|
		    SDXC_CMDFLAG_SHORTRSP);
	if (SDMMC_STATUS_NOERROR
		!= (status = g_sdmmcfn->sdmmc_sendcommand(pst, &cmd))) {
		ERROR("Error Status : 0x%08X \r\n", status);
		return FALSE;
	}

	return TRUE;
}

static int mmc_partconf(sdxcboot_status *pst,
		unsigned char ack, unsigned char part_num, unsigned char access)
{
	return mmc_switch(pst, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONF,
			  EXT_CSD_BOOT_ACK(ack) |
			  EXT_CSD_BOOT_PART_NUM(part_num) |
			  EXT_CSD_PARTITION_ACCESS(access));
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

static int emmcboot_n(struct nx_bootmanager *pbm, sdxcboot_status *pst,
		unsigned int option)
{
	struct nx_sdmmc_reg *const base
		= ((struct nx_sdmmc_reg *)sdmmc_get_baseaddr(pst->sd_port));

	unsigned int *f_sector = (unsigned int *)&pbm->bi;
	register unsigned int sd_speed, bitwidth;
	unsigned int mode, sector_cnt, rsn = 0;
	register int ret = FALSE;

	/* @brief: Confirm ? */
	if (pst->bhigh_speed == TRUE)
		sd_speed = SDXC_CLKDIV_HIGH;
	else
		sd_speed = SDXC_CLKDIV_LOW;

	if ((pst->sd_port == 0) && (!(option & 1 << eMMCBUSWIDTH)))
		bitwidth = 8;
	else
		bitwidth = 4;

	/* step 01-1. open the sd/emmc device */
	if (TRUE != g_sdmmcfn->sdmmc_open(pst, option)) {
		ERROR("Device Open Failed!! \r\n");
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

	/* @brief: eMMC Boot Mode (1: Normal, 0: Alternate) */
	if ((option & 1 << eMMCBOOTMODE) != 0)
		mode = 1;
	else
		mode = 0;

	NOTICE("eMMC %s speed %d-bits %s mode boot.\r\n",
			sd_speed ? "High" : "Normal",
			bitwidth, mode ? "Alternate": "Normal");

	/* @brief: eMMC Boot Partiton Access (BOOT_PARTITION_ENABLE = 1) */
	mmc_partconf(pst, SDMMC_BOOT_ACK, SDMMC_BOOT_PARTENB, SDMMC_BOOT_PARTNUM);

	rsn = (g_nsih->dbi.device_addr / SDMMC_BLOCK_LENGTH);
	INFO("Device_addr: %X(%X), RSN: %d \r\n",
		g_nsih->dbi.device_addr, &g_nsih->dbi.device_addr, rsn);

	/*
	 * SD/eMMC is because reading the blocks in the unit 512
	 * However, since sign data is 256 bytes, dummy data exists in 256 bytes.
	 */

	if (g_sdmmcfn->sdmmc_read_sectors(pst, rsn++, 1, f_sector) == FALSE) {
		ERROR("%s Header Read Fail! \r\n", ret ? "Boot" : "GPT");
		goto error;
	}
	struct sbi_header *pbi = (struct sbi_header *)&pbm->bi;

	/* step 01-6. check the nexell signature */
	if (pbi->signature != HEADER_ID) {
		ERROR("Not bootable image (%08x).\r\n", pbi->signature);
		return 0;
	}

	NOTICE("Load Addr: 0x%X, Load Size:0x%X, Launch Addr: 0x%08X. \r\n",
		pbi->load_addr, pbi->load_size, pbi->launch_addr);

	/*
	 * SD/eMMC is because reading the blocks in the unit 512,
	 * and calculates the size for correction.
	 */
	sector_cnt = ((pbi->load_size + (SDMMC_BLOCK_LENGTH - 1))
				/ SDMMC_BLOCK_LENGTH);

	ret = g_sdmmcfn->sdmmc_read_sectors(pst, rsn,
				sector_cnt, (unsigned int*)pbi->load_addr);
	rsn += sector_cnt;

	/*
	 * SD/eMMC is because reading the blocks in the unit 512
	 * However, since sign data is 256 bytes, dummy data exists in 256 bytes.
	 */
	ret = g_sdmmcfn->sdmmc_read_sectors(pst, rsn,
				1, (unsigned int*)pbm->rsa_encrypted_sha256_hash);
error:
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

	/*
	 * SD/eMMC is because reading the blocks in the unit 512
	 * However, since sign data is 256 bytes, dummy data exists in 256 bytes.
	 */

	if (g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn++, 1, f_sector) == FALSE) {
		ERROR("%s Header Read Fail! \r\n", ret ? "Boot" : "GPT");
		goto error;
	}

	struct sbi_header *pbi = (struct sbi_header *)&pbm->bi;

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

	ret = g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn,
				sector_cnt, (unsigned int*)pbi->load_addr);
	rsn += sector_cnt;

	/*
	 * SD/eMMC is because reading the blocks in the unit 512
	 * However, since sign data is 256 bytes, dummy data exists in 256 bytes.
	 */
	ret = g_sdmmcfn->sdmmc_read_sectors(pbt_st, rsn,
				1, (unsigned int*)pbm->rsa_encrypted_sha256_hash);

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

	g_sdmmcfn->sdpad_setalt(pst->sd_port);

	g_sdmmcfn->sdmmc_init(pst);
//	sdmmc_init(pst);

	DRV_DBGOUT("SD boot: %d\r\n", pst->sd_port);
	ret = sdmmcboot(pbm, pst, option);

	g_sdmmcfn->sdmmc_close(pst);
	g_sdmmcfn->sdmmc_terminate(pst);

	g_sdmmcfn->sdpad_setgpio(pst->sd_port);

	/* @brief: source clock-off disable for next boot-loader */
	cmu_srcoff_enable(sdmmc_get_clk_index(pst->sd_port), FALSE);

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

	g_sdmmcfn->sdpad_setalt(pst->sd_port);
	g_sdmmcfn->sdmmc_init(pst);

	/* eMMC or MMC ver 4.3+ */
	DRV_DBGOUT("eMMC boot: %d \r\n", pst->sd_port);
	ret = emmcboot_n(pbm, pst, option);

	g_sdmmcfn->sdmmc_close(pst);
	g_sdmmcfn->sdmmc_terminate(pst);

	g_sdmmcfn->sdpad_setgpio(pst->sd_port);

	/* @brief: source clock-off disable for next boot-loader */
	cmu_srcoff_enable(sdmmc_get_clk_index(pst->sd_port), FALSE);

	return ret;
}
