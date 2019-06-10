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
#include <nand.h>
#include <nandboot.h>

/* NAND  Boot Code Debug Message */
#define NAND_DRVDBG_ON			0

#if (defined(DBGLOG_ON) && NAND_DRVDBG_ON)
#define DRV_DBGOUT(...)		printf("DEBUG-NAND :  ", __VA_ARGS__)
#else
#define DRV_DBGOUT(...)		empty_printf("DEBUG-NAND  :  ", __VA_ARGS__)
#endif

/* Global Variables */
struct nx_nand_fnptr *g_nand_fn;
struct nx_nandc_reg * const pnc =
	(struct nx_nandc_reg *)PHY_BASEADDR_NANDC_MODULE;

#if 1
static int nand_check_rnb(int timeout)
{
	pnc->status |= 1 << 2;
	while (!(pnc->status & 1 << 2) && --timeout);
	pnc->status |= 1 << 2;

	if (timeout == 0) {
		printf("rnb TO\r\n");
		return 0;
	}
//	else printf("rnb TO value:%x\r\n", timeout);
	return 1;
}

static int nand_read(struct nandbootinfo *pnbi, int page, int addr,
		unsigned char *buf, int size)
{
	int i;

	pnc->cmd = 0x00;	/* read first cmd */
	pnc->adr = (addr >>  0) & 0xff;
	pnc->adr = (addr >>  8) & 0xff;
	pnc->adr = (page >>  0) & 0xFF;
	pnc->adr = (page >>  8) & 0xFF;
	if (pnbi->address_step)
		pnc->adr = (page >> 16) & 0xFF;
	pnc->cmd = 0x30;	/* read second cmd */

	if (!nand_check_rnb(0x1000))
		return 0;

//	pnc->rand = 0x576A;	// for test
	pnc->rand = 0;

	pnc->bch_mode &= ~(1 << 4);	// decoding mode
#if 0
	if (size <= 16) {
		for (i = 0; i < size; i++)
			buf[i] = (unsigned char)pnc->dat;
	} else {
		for (i = 0; i < size; i++) {
			printf("%02x ", (unsigned char)pnc->dat);
			if (i % 16 == 15)
				printf("\r\n");
		}
		printf("\r\n");
	}
#else
	for (i = 0; i < size; i++)
		buf[i] = (unsigned char)pnc->dat;
#endif

	return 1;
}

static int nand_is_bad(struct nandbootinfo *pnbi, int block)
{
	unsigned char oob[2];
	int ret;

	ret = nand_read(pnbi, block * pnbi->pageperblock,
			pnbi->pagesize, oob, 2);

	if (ret) {
		if ((oob[0] == 0xff) || (oob[1] == 0xff))
			ret = 0;	/* not bad */
		else
			ret = 1;
	} else
		ret = 1;
//	printf("%d block is %sbad\r\n", block, ret ? "" : "not ");
	return ret;
}
#endif

#if 0
static int nand_check_dma(int timeout)
{
	pnc->status |= 1 << 6;		// clear dma done pending flag
	pnc->dma_ctrl |= 1 << 0;	// nand write dma start

	while (!(pnc->status & 1 << 6) && --timeout);
	pnc->status |= 1 << 6;

	if (timeout == 0) {
		printf("read dma TO\r\n");
		return 0;
	}
//	else printf("dma TO value:%x\r\n", timeout);

	while (pnc->status & 1 << 25);	// wait for idle NFIF
	pnc->dma_ctrl &= ~(1 << 0);	// dma mode off and to cpu mode

	return 1;
}

static int nand_error_correct(unsigned char *buf)
{
	int errcnt = pnc->err_info;

//	printf("page %d ", page);
	if (errcnt == 0x3f) {
		printf("has none correctable error.\r\n");
		return 0;
	} else if (errcnt > 0) {
		int i, elp, offset = (1024 * 2 - 918 - 105) * 8;
		unsigned char *pdata = (unsigned char *)buf;
		for (i = 0; i < errcnt; i++) {
			elp = pnc->sram[i] & 0x3FFF;
			elp -= offset;
			if (1024 * 8 > elp)
				pdata[elp >> 3] ^= 1 << (7 - (elp & 7));
//			else is ecc data, so no correct
		}
		printf("has %d error corrected\r\n", errcnt);
	} else printf("read done without any error.\r\n");

	return 1;
}

static int nand_read_dma(int page, int addr, unsigned int *buf, int pagesize)
{
//	unsigned int datasize;
	int sectorcnt = pagesize / 1024;

	pnc->cmd = 0x00;	/* read first cmd */
	pnc->adr = (addr >>  0) & 0xff;
	pnc->adr = (addr >>  8) & 0xff;
	pnc->adr = (page >>  0) & 0xFF;
	pnc->adr = (page >>  8) & 0xFF;
//	pnc->adr = (page >> 16) & 0xFF;
	pnc->cmd = 0x30;	/* read second cmd */

	if (!nand_check_rnb(0x1000))
		return 0;

	pnc->bch_mode = NX_NANDC_BCH_1024_60;
	pnc->bch_mode &= ~(1 << 4);	// decoding mode

	pnc->dma_addr = (unsigned int)buf; // data & xxx & ecc

//	datasize = 1024 - (14 * 60 + (8 - 1)) / 8 - 1;

	pnc->dma_size =
		((14 * 60 + (8 - 1)) / 8 - 1) << 16 |
		(1024 * sectorcnt - 1) << 0;       // total size;

	pnc->dma_subp = (sectorcnt - 1) << 16 |		// dma subpage number
			(1024 - 1) << 0;	// dma subpage size

	pnc->rand = 0x576a;

	if (!nand_check_dma(0x1000))
		return 0;

	printf("page %d ", page);
	if (!nand_error_correct((unsigned char *)buf))
		return 0;

	return 1;
}
#endif
#if 0
static int nand_write_dma(int page, int addr, unsigned char *buf, int pagesize)
{
	int timeout = 0x1000;

	if (nand_is_bad(page / 64))
		return 0;

	int sectorcnt = pagesize / 1024;

	pnc->bch_mode = NX_NANDC_BCH_1024_60;
	pnc->bch_mode |= (1 << 4);	// encoding mode

	pnc->dma_addr = (unsigned int)buf; // data & xxx & ecc

//	datasize = 1024 - (14 * 60 + (8 - 1)) / 8 - 1;

	pnc->dma_size =
		((14 * 60 + (8 - 1)) / 8 - 1) << 16 |
		(1024 * sectorcnt - 1) << 0;       // total size;

	pnc->dma_subp =
		(sectorcnt - 1) << 16 |		// dma subpage number
		(1024 - 1) << 0;	// dma subpage size

	pnc->rand = 0x576a;

	pnc->cmd = 0x80;
	pnc->adr = (addr >>  0) & 0xFF;
	pnc->adr = (addr >>  8) & 0xFF;
	pnc->adr = (page >>  0) & 0xFF;
	pnc->adr = (page >>  8) & 0xFF;
//	pnc->adr = (page >> 16) & 0xFF;

	volatile unsigned int delay = 0x100;	/* tADL */
	while (delay--);

	if (!nand_check_dma(0x1000))
		return 0;

	pnc->cmd = 0x10;
	/* tWB */
	if (!nand_check_rnb(0x1000))
		return 0;

	return 1;
}
#endif
#if 0
#include <gpio.h>
static struct nx_gpio_regs (*const g_gpio_regs)[1] =
	(struct nx_gpio_regs(*)[])(PHY_BASEADDR_GPIOA_SECURE_MODULE);
static int nand_write(struct nandbootinfo *pnbi, int page, int addr,
		unsigned char *buf, int size)
{
	int i, timeout = 0x1000;

	if (nand_is_bad(pnbi, page / 64))
		return 0;

	g_gpio_regs[2]->s_gpio.outenb = 1 << 16;	// nand wp
	g_gpio_regs[2]->s_gpio.out = 1 << 16;

	pnc->bch_mode |= 1 << 4;	// encoding mode

	pnc->cmd = 0x80;
	pnc->adr = (addr >>  0) & 0xFF;
	pnc->adr = (addr >>  8) & 0xFF;
	pnc->adr = (page >>  0) & 0xFF;
	pnc->adr = (page >>  8) & 0xFF;
//	pnc->adr = (page >> 16) & 0xFF;

	volatile unsigned int delay = 0x100;	/* tADL */
	while (delay--);

	for (i = 0; i < size; i++)
#if 1
		pnc->dat = buf[i];
#else
		pnc->dat = (unsigned char)i;
//		pnc->dat_bypass = (unsigned char)i;
#endif
	pnc->cmd = 0x10;
	/* tWB */
	if (!nand_check_rnb(0x1000))
		return 0;

	return 1;
}
#endif
#if 0
static int nand_mark_bad(struct nandbootinfo *pnbi, int block)
{
	int ret;
	ret = nand_is_bad(pnbi, block);
	if (ret)
		return 1;

	unsigned char buf[2] = {0, 0};
	return nand_write(pnbi, block * pnbi->pageperblock, pnbi->pagesize, buf, 2);
}
#endif
#if 0
static int nand_erase(int block)
{
	if (nand_is_bad(block))
		return 0;

	block *= 64;

	pnc->cmd = 0x60;
	pnc->adr = (block >>  0) & 0xFF;
	pnc->adr = (block >>  8) & 0xFF;
//	pnc->adr = (block >> 16) & 0xFF;
	pnc->cmd = 0xd0;
	/* tWB */
	if (!nand_check_rnb(0x1000))
		return 0;

	pnc->cmd = 0x70;
	unsigned char status = pnc->dat;
	/* block erase is not successful */
	if ((status & 1 << 5) && (status & 1 << 0)) {
		printf("%d block erase fail\r\n", block);
		return 0;
	}

	printf("%d block erase success(%02x)\r\n", block, status);
	return 1;
}
#endif
#if 0
static void nand_check_initial_bad(struct nandbootinfo *pnbi)
{
	int j, b = 0;

	printf("initial bad check:\r\n");
	for (j = 0; j < 1024; j++) {
		int k = nand_is_bad(pnbi, j);
		if (k) {
			printf("%d ", j);
			b++;
		}
	}
	printf("\r\n%d bad block detected\r\n", b);
}
#endif

static int nandboot_skip_bad(struct nandbootinfo *pnbi, int block)
{
	int ret;
	int bad_cnt = 0;
	/* find readable block */
	int max_bad_block = 20;	/* 1024: 20, 2048: 40, 4096: 80 */
	do {
		ret = nand_is_bad(pnbi, block + bad_cnt);
		if (ret == 0)
			break;
		max_bad_block--;
		bad_cnt++;
	} while (max_bad_block);

	if (max_bad_block == 0) {
		printf("nand bad block exceeded(%d)\r\n", bad_cnt);
		return 0;
	}
	block += bad_cnt;
	printf("detect %d bad, new boot block is %d\r\n", bad_cnt, block);

	return block;
}

static int nandboot_get_start_block(struct nandbootinfo *pnbi,
		int my_start_block)
{
	int valid_block = 0, req_valid_block = my_start_block, test_block = 0;

	do {
		if (!nand_is_bad(pnbi, test_block++)) {
			valid_block++;
		}
	} while (valid_block < req_valid_block);

	return test_block;
}
int nandboot(struct nx_bootmanager *pbm, unsigned int option)
{
	struct nandbootinfo nbi;
	struct nandbootinfo *pnbi = &nbi;
	unsigned int start_block = pbm->bi.dbi.device_addr;
//	unsigned char __attribute__ ((packed, aligned(32))) nbuf[1024];
	unsigned char *nbuf = 0xfffff200;
//	unsigned char *nbuf = 0xffff8c00;
	int ret = FALSE, block_size;


	/* step 00. get the bl0 function-table address */
	g_nand_fn = ((struct nx_nand_fnptr *)&g_bl1_fn->nand_fn);

	pnbi->buf = (unsigned char *)nbuf;

	/* step 01-1. initialize the nand controller */
	g_nand_fn->nandc_init(pnbi, option);

	pnc->cmd_time =
		15 << 24 |	// RHW
		1 << 16 |	// SETUP
		2 <<  8 |	// WIDTH
		2 <<  0;	// HOLD

	pnc->dat_time =
		15 << 24 |	// WHR
		0 << 16 |	// SETUP
		4 <<  8 |	// WIDTH
		2 <<  0;	// HOLD

	pnbi->pageperblock = (pnbi->pagesize == 2048) ? 64 : 128;
	block_size = pnbi->pagesize * pnbi->pageperblock;
	pnbi->checkimagecnt = 1;	// no next retry
	pnbi->address_step = 0;

	/* step 01-2. reset the nand device */
	if (g_nand_fn->nand_reset() == 0) {
		printf("reset failure.\r\n");
		goto error;
	}
#if 0
	static bad_mark = 1;
	if (bad_mark) {
		nand_mark_bad(pnbi, 1);
		nand_mark_bad(pnbi, 2);
		nand_mark_bad(pnbi, 3);
		nand_mark_bad(pnbi, 24);
		nand_mark_bad(pnbi, 27);
		nand_mark_bad(pnbi, 29);
		nand_mark_bad(pnbi, 31);
		nand_mark_bad(pnbi, 32);
		bad_mark = 0;
	}
	nand_check_initial_bad (pnbi);
#endif

	printf("nand bad check start address %x\r\n", start_block);
	start_block /= block_size;
	printf("nand bad check start block %x\r\n", start_block);
	start_block = nandboot_get_start_block(pnbi, start_block);
	printf("new start block is %d(0x%08x)\r\n", start_block,
			start_block * block_size);
	start_block = nandboot_skip_bad(pnbi, start_block);
	if (start_block == 0) {
		printf("max bad in boot block\r\n");
		goto error;
	}

	pnbi->zeroimage_page = start_block * pnbi->pageperblock;
	pnbi->copyimage_page = pnbi->zeroimage_page;

	printf("nand boot from %d page\r\n", pnbi->zeroimage_page);

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
#if 0
	unsigned int *ptr = (unsigned int *)&pbm->bi;
	for (i = 0; i < 256 / 4; i++) {
		printf("%08x ", ptr[i]);
		if (i % 8 == 7)
			printf("\r\n");
	}
	printf("\r\n");
#endif

	/* step 03. read the hash data */
	ret = g_nand_fn->nand_read(pnbi,
			((void *)&pbm->rsa_encrypted_sha256_hash[0]),
			256);
	if (ret == FALSE) {
		ERROR("Nand boot hash read failure!! \r\n");
		goto error;
	}

	void *load_addr = pbm->bi.load_addr;
	int blockdata = pnbi->pagesize * pnbi->pageperblock /
		1024 * pnbi->datasize;
	int load_size;// = pbm->bi.load_size;
	int loaded_size = sizeof(struct sbi_header) + 256;	// 512 bytes
	printf("nand image read to %x, size:%d\r\n",
			load_addr, pbm->bi.load_size);

	if (pbm->bi.load_size + loaded_size > blockdata) {
		load_size = blockdata - loaded_size;
	} else
		load_size = pbm->bi.load_size;

	pbm->bi.load_size -= load_size;

	do {
		printf("nand block load size:%d(%d)\r\n",
				load_size, pbm->bi.load_size);
		/* step 04. read the boot-image */
		set_boption(get_boption() | (1 << 23));	// debug message off
		ret = g_nand_fn->nand_read(pnbi, load_addr, load_size);
		set_boption(get_boption() & ~(1 << 23));	// debug message on
		if (ret == FALSE) {
			ERROR("Nand boot boot-image read failure!! \r\n");
			goto error;
		}
		load_addr += load_size;
		start_block++;
		start_block = nandboot_skip_bad(pnbi, start_block);
		if (start_block == 0) {
			printf("max bad at boot blocks\r\n");
			goto error;
		}
		pnbi->zeroimage_page = start_block * pnbi->pageperblock;
		pnbi->copyimage_page = pnbi->zeroimage_page;

		printf("nand boot from %d page\r\n", pnbi->zeroimage_page);

		if (pbm->bi.load_size > blockdata) {
			load_size = blockdata;
		} else
			load_size = pbm->bi.load_size;
		if (load_size == 0)
			break;
		pbm->bi.load_size -= load_size;

	} while (load_size);

error:
	/* step 07. deintialize the nand controller */
//	g_nand_fn->nandc_deinit();

	return ret;
}
