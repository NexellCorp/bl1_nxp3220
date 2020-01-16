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
#include <bootheader.h>
#include <nand.h>
#include <nandboot.h>

/* NAND  Boot Code Debug Message */
#define NAND_DRVDBG_ON	0

#if (defined(DBGLOG_ON) && NAND_DRVDBG_ON)
#define DRV_DBGOUT(...)		printf(__VA_ARGS__)
#else
#define DRV_DBGOUT(...)		empty_printf(__VA_ARGS__)
#endif

/* Global Variables */
struct nx_nand_fnptr *g_nand_fn;
struct nx_nandc_reg * const pnc =
	(struct nx_nandc_reg *)PHY_BASEADDR_NANDC_MODULE;

static int nand_check_rnb(int timeout);
static int nand_is_bad(struct nandbootinfo *pnbi, int block);

#ifdef TEST_BAD_BLOCK
#include <gpio.h>

static struct nx_gpio_regs (*const g_gpio_regs)[1] =
	(struct nx_gpio_regs(*)[])(PHY_BASEADDR_GPIOA_SECURE_MODULE);

static int nand_write(struct nandbootinfo *pnbi, int page, int addr,
		unsigned char *buf, int size)
{
	volatile unsigned int delay = 0x100;
	int i;

	if (nand_is_bad(pnbi, page / 64))
		return 0;

	/* nand wp */
	g_gpio_regs[2]->s_gpio.outenb = 1 << 16;
	g_gpio_regs[2]->s_gpio.out = 1 << 16;

	/* encoding mode */
	pnc->bch_mode |= 1 << 4;

	pnc->cmd = 0x80;
	pnc->adr = (addr >>  0) & 0xFF;
	pnc->adr = (addr >>  8) & 0xFF;
	pnc->adr = (page >>  0) & 0xFF;
	pnc->adr = (page >>  8) & 0xFF;
	if (pnbi->address_step)
		pnc->adr = (page >> 16) & 0xFF;

	/* tADL */
	while (delay--)
		;

	for (i = 0; i < size; i++)
		pnc->dat_bypass = buf[i];

	pnc->cmd = 0x10;

	/* tWB */
	if (!nand_check_rnb(0x1000))
		return 0;

	return 1;
}

static int nand_mark_bad(struct nandbootinfo *pnbi, int block)
{
	unsigned char buf[2] = {0, 0};
	int ret;

	ret = nand_is_bad(pnbi, block);
	if (ret)
		return 1;

	return nand_write(pnbi, block * pnbi->pageperblock,
			  pnbi->pagesize, buf, 2);
}

static void nand_check_initial_bad(struct nandbootinfo *pnbi)
{
	int j, b = 0;

	DRV_DBGOUT("initial bad check:\r\n");
	for (j = 0; j < 1024; j++) {
		int k = nand_is_bad(pnbi, j);

		if (k) {
			DRV_DBGOUT("%d ", j);
			b++;
		}
	}
	DRV_DBGOUT("\r\n%d bad block detected\r\n", b);
}

static void nand_test_bad_block(struct nandbootinfo *pnbi)
{
	/* declare static variable for suspend/resume */
	static bool bad_mark = true;

	if (bad_mark) {
		nand_mark_bad(pnbi, 1);
		nand_mark_bad(pnbi, 2);
		nand_mark_bad(pnbi, 3);
		nand_mark_bad(pnbi, 24);
		nand_mark_bad(pnbi, 27);
		nand_mark_bad(pnbi, 29);
		nand_mark_bad(pnbi, 31);
		nand_mark_bad(pnbi, 32);

		bad_mark = false;
	}

	nand_check_initial_bad(pnbi);
}
#endif

static int nand_check_rnb(int timeout)
{
	pnc->status |= 1 << 2;

	while (!(pnc->status & 1 << 2) && --timeout)
		;

	pnc->status |= 1 << 2;

	if (!timeout) {
		DRV_DBGOUT("rnb TO\r\n");
		return 0;
	}

	return 1;
}

static int nand_read(struct nandbootinfo *pnbi, int page, int offs,
			unsigned char *buf, int size)
{
	int i;

	/* read first cmd */
	pnc->cmd = 0x00;
	pnc->adr = (offs >>  0) & 0xFF;
	pnc->adr = (offs >>  8) & 0xFF;
	pnc->adr = (page >>  0) & 0xFF;
	pnc->adr = (page >>  8) & 0xFF;
	if (pnbi->address_step)
		pnc->adr = (page >> 16) & 0xFF;

	/* read second cmd */
	pnc->cmd = 0x30;

	if (!nand_check_rnb(0x1000))
		return 0;

	pnc->rand = 0;	// for test : 0x576A

	/* decoding mode */
	pnc->bch_mode &= ~(1 << 4);

	for (i = 0; i < size; i++)
		buf[i] = (unsigned char)pnc->dat_bypass; /* dat */

	return 1;
}

#define NAND_OOB_BADMARK_SIZE	2
#define NAND_OOB_BADMARK_OFFS	(pnbi->pagesize)
#define NAND_OOB_BADMARK_POS	0
#define IS_BADBLOCK(oob)	\
	((oob[NAND_OOB_BADMARK_POS] == 0x00) && \
	 (oob[NAND_OOB_BADMARK_POS + 1] == 0xff))

static int nand_is_bad(struct nandbootinfo *pnbi, int block)
{
	unsigned char oob[NAND_OOB_BADMARK_SIZE] = { 0, };
	int isbad = 1;
	int ret;

	/*
	 * Check badmark
	 * PAGE + OOB[0:1]=0x00:0xFF
	 */
	ret = nand_read(pnbi, block * pnbi->pageperblock,
			NAND_OOB_BADMARK_OFFS, oob, sizeof(oob));
	if (ret && !IS_BADBLOCK(oob))
		isbad = 0;

	DRV_DBGOUT("%04d block is %sbad oob[0x%02x][0x%02x]\r\n\n",
		block, isbad ? "" : "not ",
		oob[NAND_OOB_BADMARK_POS], oob[NAND_OOB_BADMARK_POS + 1]);

	return isbad;
}

static int nandboot_skip_bad(struct nandbootinfo *pnbi, int block)
{
	/* find readable block */
	int max_bad_block = 20;	/* 1024: 20, 2048: 40, 4096: 80 */
	int count = 0;
	int ret;

	do {
		ret = nand_is_bad(pnbi, block + count);
		if (ret == 0)
			break;
		max_bad_block--;
		count++;
	} while (max_bad_block);

	if (max_bad_block == 0) {
		ERROR("nand bad block exceeded(%d)\r\n", count);
		return 0;
	}

	block += count;
	DRV_DBGOUT("detect %d bad, new boot block is %d\r\n", count, block);

	return block;
}

/*
 * Check the bad block count from the previous bootloader
 * and get the current start offset.
 */
static int nandboot_get_start_block(struct nandbootinfo *pnbi,
		int start_block)
{
	int count = 0, req_block = start_block, block = 0;

	do {
		if (!nand_is_bad(pnbi, block++))
			count++;
	} while (count < req_block);

	return block;
}

#define NAND_BL0_CMD_TIME	0x55061e05
#define NAND_BL0_DAT_TIME	0x3205140a
#define NAND_BL0_DDR_TIME	0x23000002

static struct nand_time {
	unsigned int cmd_time;
	unsigned int dat_time;
	unsigned int ddr_time;
} nand_time = { NAND_BL0_CMD_TIME, NAND_BL0_DAT_TIME, NAND_BL0_DDR_TIME };

static inline void nand_timing(struct sbi_header *pbi, struct nand_time *pnt)
{
	struct nand_time nt = {
		.cmd_time = pbi->nand_cmd_time,
		.dat_time = pbi->nand_dat_time,
		.ddr_time = pbi->nand_ddr_time,
	};

	if (pbi->nand_cmd_time)
		nt.cmd_time = pnt->cmd_time = pbi->nand_cmd_time;
	else
		nt.cmd_time = pnt->cmd_time;

	if (pbi->nand_dat_time)
		nt.dat_time = pnt->dat_time = pbi->nand_dat_time;
	else
		nt.dat_time = pnt->dat_time;

	if (pbi->nand_ddr_time)
		nt.cmd_time = pnt->cmd_time = pbi->nand_cmd_time;
	else
		nt.ddr_time = pnt->ddr_time;

	mmio_write_32(PHY_BASEADDR_NANDC_MODULE + 0x1c, nt.cmd_time);
	mmio_write_32(PHY_BASEADDR_NANDC_MODULE + 0x20, nt.dat_time);
	mmio_write_32(PHY_BASEADDR_NANDC_MODULE + 0x24, nt.ddr_time);
}

int nandboot(struct nx_bootmanager *pbm, unsigned int option)
{
	struct nandbootinfo nbi;
	struct nandbootinfo *pnbi = &nbi;
	struct sbi_header *pbi = (struct sbi_header *)&pbm->bi;
	unsigned int start_block = pbm->bi.dbi.device_addr;
	unsigned char *nbuf = (unsigned char *)0xfffffc00;
	int ret = FALSE, block_size;

	/* step 00. get the bl0 function-table address */
	g_nand_fn = ((struct nx_nand_fnptr *)&g_bl1_fn->nand_fn);

	pnbi->buf = (unsigned char *)nbuf;

	/* step 01-1. initialize the nand controller */
	g_nand_fn->nandc_init(pnbi, option);

	pnbi->pageperblock = (pnbi->pagesize == 2048) ? 64 : 128;
	block_size = pnbi->pagesize * pnbi->pageperblock;
	pnbi->checkimagecnt = 1;	// no next retry
	pnbi->address_step = 0;

	/* step 01-2. reset the nand device */
	if (g_nand_fn->nand_reset() == 0) {
		ERROR("reset failure.\r\n");
		goto error;
	}

#ifdef TEST_BAD_BLOCK
	nand_test_bad_block(pnbi);
#endif

	DRV_DBGOUT("nand page size:%d, page-per-block:%d, block size:0x%x\r\n",
		pnbi->pagesize, pnbi->pageperblock, block_size);
	DRV_DBGOUT("nand bad check start address %x\r\n", start_block);

	start_block /= block_size;

	DRV_DBGOUT("nand bad check start block %d\r\n", start_block);
	start_block = nandboot_get_start_block(pnbi, start_block);

	DRV_DBGOUT("new start block is %d(0x%08x)\r\n",
		    start_block, start_block * block_size);

	start_block = nandboot_skip_bad(pnbi, start_block);
	if (start_block == 0) {
		ERROR("max bad in boot block\r\n");
		goto error;
	}

	pnbi->zeroimage_page = start_block * pnbi->pageperblock;
	pnbi->copyimage_page = pnbi->zeroimage_page;

	DRV_DBGOUT("nand boot start block %d\r\n", start_block);

	/* step 02-1. read the boot header */
	ret = g_nand_fn->nand_read(pnbi, (void *)&pbm->bi,
				   sizeof(struct sbi_header));
	if (ret == FALSE) {
		ERROR("Nand boot header read failure!! \r\n");
		goto error;
	}

	/* step 02-2. check the nexell signature */
	if (pbm->bi.signature != HEADER_ID) {
		ERROR("Header Signature Failed! (%08x)\r\n", pbm->bi.signature);
		goto error;
	}

	/*
	 * set nand timing
	 */
	nand_timing(pbi, &nand_time);

	/* step 03. read the hash data */
	ret = g_nand_fn->nand_read(pnbi,
			((void *)&pbm->rsa_encrypted_sha256_hash[0]),
			256);
	if (ret == FALSE) {
		ERROR("Nand boot hash read failure!! \r\n");
		goto error;
	}

	void *load_addr = (void *)pbm->bi.load_addr;
	unsigned int blockdata = pnbi->pagesize * pnbi->pageperblock /
		1024 * pnbi->datasize;
	int load_size;
	int loaded_size = sizeof(struct sbi_header) + 256;	// 512 bytes

	DRV_DBGOUT("nand image read to %x, size:%d\r\n",
			load_addr, pbm->bi.load_size);

	if (pbm->bi.load_size + loaded_size > blockdata)
		load_size = blockdata - loaded_size;
	else
		load_size = pbm->bi.load_size;

	pbm->bi.load_size -= load_size;

	do {
		DRV_DBGOUT("nand block load size:%d(%d) from %d:0x%x(%d:%d)\r\n",
			load_size, pbm->bi.load_size, start_block,
			pnbi->zeroimage_page * pnbi->pagesize,
			pnbi->zeroimage_page, pnbi->pagesize);

		/* step 04. read the boot-image */
		int option = get_boption();

		set_boption(option | (1 << 23));	// debug message off
		ret = g_nand_fn->nand_read(pnbi, load_addr, load_size);
		set_boption(option);	// option restore
		if (ret == FALSE) {
			ERROR("Nand boot boot-image read failure!! \r\n");
			goto error;
		}
		load_addr += load_size;

		/* Next block */
		start_block++;
		start_block = nandboot_skip_bad(pnbi, start_block);
		if (start_block == 0) {
			ERROR("max bad at boot blocks\r\n");
			goto error;
		}

		pnbi->zeroimage_page = start_block * pnbi->pageperblock;
		pnbi->copyimage_page = pnbi->zeroimage_page;

		if (pbm->bi.load_size > blockdata)
			load_size = blockdata;
		else
			load_size = pbm->bi.load_size;

		if (load_size == 0)
			break;

		pbm->bi.load_size -= load_size;

	} while (load_size);

error:
	/* step 07. deintialize the nand controller */
	// g_nand_fn->nandc_deinit();

	return ret;
}
