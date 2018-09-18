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
#include <usbboot.h>
#include <sdxcboot.h>
#include <uartboot.h>
#include <spiboot.h>
#include <nandboot.h>
#include <libnx.h>
#include <checker.h>
#include <tz.h>
#include <sss.h>
#include <cmu.h>
#include <alive.h>
#include <efuse.h>
#include <plat_pm.h>
#include <plat_load.h>

/* Global Vriables */
struct platform_manager g_pi, *g_ppi;

/* Extern Variables */
extern unsigned char g_rsa_public_key[512];
extern unsigned int emmc_next_bl;
/* Extern Functions */
extern void (*enter_self_refresh)(void);
extern void (*exit_self_refresh)(void);

extern void (*pmic_poweroff)(void);

extern struct nx_vddpwr_reg *g_vddpwr_reg;

static int check_load_addr(unsigned int load_addr)
{
	return load_addr;
}

static int check_platfrom(struct nx_bootmanager *pbm,
			unsigned char *rsa_public_key, unsigned int encrypted)
{
	unsigned int option = get_boption();
	int verify_enb = ((option >> VERIFY) & 0x3);
	int ret = 0;

	/* @brief: verification the hash data */
	if ((ret = authenticate_bimage(pbm,
			rsa_public_key, verify_enb)) < 0) {
		ERROR("Verification Failed!! \r\n");
		return -1;
	}

	/* @brief: decrypt the boot-image */
	if ((ret != -1) && encrypted) {
		unsigned int iv[4];
		iv[0] = (*g_bl1_fn->iv)[0];
		iv[1] = (*g_bl1_fn->iv)[1];
		iv[2] = (*g_bl1_fn->iv)[2];
		iv[3] = (*g_bl1_fn->iv)[3];
		aes_cbc_decrypt_bimage(pbm, iv);
	}

	return 0;
}

static void plat_s_launch(unsigned int is_resume,
		unsigned int secure_l, unsigned int n_secure_l, int is_secure)
{
	if (is_secure) {
		SYSMSG("Launch to 0x%08X\r\n", (unsigned int)secure_l);
		while (!serial_tx_empty());
		secure_launch(is_resume, secure_l, n_secure_l, 0);
	} else {
		SYSMSG("Launch to 0x%08X\r\n", (unsigned int)n_secure_l);
		while (!serial_tx_empty());
		non_secure_launch(is_resume, n_secure_l);
	}
}

int plat_next_load(struct nx_bootmanager *pbm, unsigned int option)
{
	int device = ((option >> BOOTMODE) & 0x7);
	int ret = TRUE;

	switch (device) {
		case EMMCBOOT:
			SYSMSG("Loading from eMMC...\r\n");
			ret = emmcboot(pbm, option);
			break;

		case USBBOOT:
			SYSMSG("Loading from USB...\r\n");
			ret = usbboot(pbm);
			break;
#if 0		/* todo */
		case SDFSBOOT:
			SYSMSG("Loading from SDFS...\r\n");
			ret = sdxcfsboot(ptbi. option);
			break;
#endif
		case SDBOOT:
			SYSMSG("Loading from SD...\r\n");
			ret = sdxcboot(pbm, option);
			break;

		case NANDECBOOT:
			SYSMSG("Loading from NAND...\n");
			ret = nandboot(pbm, option);
			break;

		case SPIBOOT:
			SYSMSG("Loading from SPI...\r\n");
			ret = spiboot(pbm, option);
			break;

		case UARTBOOT:
			SYSMSG("Loading from UART...\r\n");
			ret = uartboot(pbm, option);
			break;

		default:
			SYSMSG("Not Support Boot Mode!!! (%X) \r\n", device);
			break;

	}

	return ret;
}

void plat_load(unsigned int is_resume, struct nx_bootmanager *pbm)
{
	unsigned int option = get_boption();
	int success;

	g_nsih->dbi.device_addr = BL2_DEVICE_ADDR;
	success = plat_next_load(pbm, option);
#if 0
	if (success >= 0)
		success = check_platfrom(pbm, &g_rsa_public_key[256], 0);
#endif
	if (success >= 0) {
		/* @brief: Relocate the header of base address */
		memcpy((void*)RE_HEADER_BASEADDR, (void*)&pbm->bi,
			sizeof(struct sbi_header));
		g_nsih = (struct sbi_header *)RE_HEADER_BASEADDR;

		plat_s_launch(is_resume, 0, pbm->bi.launch_addr, 0);
	}

	ERROR("Platform Load Failed!! (%X) \r\n", success);
	while(1);
}

int plat_s_load(struct platform_info *ppi)
{
	struct nx_bootmanager bm, *pbm;
	unsigned int is_secure_os, is_sss_f, encrypted;
	unsigned int option = get_boption();
	unsigned int device = ((option >> BOOTMODE) & 0x7);
	unsigned int secure_l = 0;

	unsigned int is_resume = check_suspend_state();
	unsigned int success = 0;

	pbm = ((struct nx_bootmanager *)&bm);
	g_ppi = ((struct platform_manager *)&g_pi);
	memcpy((void*)g_ppi, (void*)ppi, sizeof(struct platform_info));

	enter_self_refresh = (void (*)(void))ppi->ensr_func;
	exit_self_refresh = (void (*)(void))ppi->exsr_func;
	pmic_poweroff = (void (*)(void))ppi->pmic_poweroff;

	/*
	 * @brief: If it is a block-device, it is unconditionally read.
	 * It checks the header and returns the result.
	 */
	if ((device != USBBOOT) && (device != UARTBOOT)) {
		is_secure_os = is_sss_f = TRUE;
	} else {
		is_secure_os = ((ppi->is_loadmark >> 0) & 0xF);
		is_sss_f = ((ppi->is_loadmark >> 8) & 0xF);
	}

	/*
	 * @brief: After confirming the resume status, perform the necessary
	 * resume step at boot time.
	 */
	if (is_resume == TRUE) {
		success = system_resume(&is_resume, is_secure_os,
				&secure_l, &pbm->bi.launch_addr);
		g_ppi->s_launch_addr = secure_l;
		if (success == 0) {
			suspend_mark_clear();
			goto plat_launch;
		}
	} else {
		suspend_mark_clear();
	}

	if (is_secure_os) {
		if (is_sss_f) {
			NOTICE("Load the SSS Firmware.. \r\n");
			g_nsih->dbi.device_addr = check_load_addr(ppi->sf_dev_addr);
			success = sss_load(pbm, option);
			if (success != TRUE)
				WARN("SSS Firmware Load Failed!! (%d) \r\n", success);
#if 0
			else
				success = check_platfrom(pbm,
					&g_rsa_public_key[256], 0);
#endif
		}

		if (is_resume != TRUE) {
			NOTICE("Load the Secure OS... \r\n");
			g_nsih->dbi.device_addr = check_load_addr(ppi->s_dev_addr);
			success = plat_next_load(pbm, option);
			if (success != TRUE) {
				WARN("Secure-OS Load Failed!! (%d) \r\n", success);
				return -BL32_LOAD_FAILED;
			}
			encrypted = (pbm->bi.reserved2 & 0xF);
			success = check_platfrom(pbm,
					&g_rsa_public_key[256], encrypted);

			secure_l = g_ppi->s_launch_addr  = pbm->bi.launch_addr;

			/* @brief: set the tzasc regionX for secure-os */
			tzasc_set_regionx(TZC_REGION_1,
				pbm->bi.load_addr, pbm->bi.load_size, TRUE);
		}
	}

	if (is_resume != TRUE) {
		NOTICE("Load the Non-Secure OS... \r\n");
		g_nsih->dbi.device_addr = check_load_addr(ppi->n_dev_addr);
		success = plat_next_load(pbm, option);
		if (success != TRUE) {
			ERROR("Boot Loade 3-3 Load Failed!! (%d) \r\n", success);
			return -BL33_LOAD_FAILED;
		}
#if 0
		success = check_platfrom(pbm, &g_rsa_public_key[256], 0);
#endif
	}
	/* @brief: Copies the header for reference in BL2 */
	memcpy((void*)USERKEY_BASEADDR, (void*)&pbm->bi, sizeof(struct sbi_header));

plat_launch:
	if ((is_secure_os && (secure_l > 0)) || (pbm->bi.launch_addr > 0))
		plat_s_launch(is_resume, secure_l, pbm->bi.launch_addr, is_secure_os);

	return success;
}
