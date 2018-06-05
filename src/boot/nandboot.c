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
struct nx_nandc_reg * const g_nandc_reg =
	(struct nx_nandc_reg *)PHY_BASEADDR_NANDC_MODULE;

int nandboot(struct nx_bootmanager *pbm, unsigned int option)
{
	struct nandbootinfo nbi;
	struct nandbootinfo *pnbi = ((struct nandbootinfo *)&nbi);
	unsigned int hash_size = 256;
	unsigned int nbuf[1024 / 4];
	int timeout, ret = 0;

	/* step 00. get the bl0 function-table address */
	g_nand_fn = ((struct nx_nand_fnptr *)&g_bl1_fn->nand_fn);

	pnbi->buf = (unsigned char *)nbuf;

	/* step 01-1. initialize the nand controller */
	g_nand_fn->nandc_init(pnbi, option);

	timeout = 0x1000000;
	while (!(mmio_read_32(&g_nandc_reg->status) & (1 << 28)) && --timeout);	// Wait for RNB Status
	if (timeout == 0) {
		ERROR("Nand is busy for a long time!!\n");
		ret = 0;
		goto error;
	}

	/* step 01-2. reset the nand device */
	if (g_nand_fn->nand_reset() == 0) {
		ret = 1;
		goto error;
	}

	/* step 02-1. read the boot header */
	ret = g_nand_fn->nand_read(pnbi, (void *)&pbm->bi,
			sizeof(struct sbi_header));
	if (ret == 0) {
		ERROR("Nand boot header read failure!! \n");
		ret = -1;
		goto error;
	}
	/* step 02-2. check the nexell signature */
	if (pbm->bi.signature != HEADER_ID) {
		ERROR("Header Signature Failed! (%08x)\r\n", pbm->bi.signature);
		ret = -1;
		goto error;
	}

	/* step 03. read the hash data */
	ret = g_nand_fn->nand_read(pnbi,
			((void *)&pbm->rsa_encrypted_sha256_hash[0]),
			hash_size);
	if (ret == 0) {
		ERROR("Nand boot hash read failure!! \n");
		ret = -1;
		goto error;
	}

	/* step 04. read the boot-image */
	ret = g_nand_fn->nand_read(pnbi, (void*)pbm->bi.load_addr,
		pbm->bi.load_size);
	if (ret == 0) {
		ERROR("Nand boot boot-image read failure!! \n");
		ret = -1;
		goto error;
	}

error:
	/* step 07. deintialize the nand controller */
	g_nand_fn->nandc_deinit();

	return ret;
}
