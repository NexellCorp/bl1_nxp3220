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
#include <pmu.h>
#include <sysreg.h>

/* Global Variable Define */
static struct nx_pmu_reg *g_pmu_reg =
	((struct nx_pmu_reg *)PHY_BASEADDR_PMU);

/*
 * After applying power to the PMU block, wait for ack after the request
 * whether the block is normal-state.
 */
static void check_pwrup_blk(int domain, int enable)
{
	struct nx_sysreg_sys_reg *sysreg =
		(struct nx_sysreg_sys_reg *)PHY_BASEADDR_SYS_MODULE;
	unsigned int ack = enable ? 0 : 1;

	switch(domain) {
		case BLOCK_DDR:
			/* Send the Request - Block DDR */
			mmio_set_32(&sysreg->pwrdn_reqn, (0x1 << 3) |		/* [3]: ADB400_BLK_DDR_CFG_BUS_0__SYSREG__PWRDNREQN	*/
							 (0x1 << 5));		/* [5]: ADB400_BLK_DDR_DATA_BUS_0__SYSREG__PWRDNREQN	*/
			/* Waiting for ACK - Block DDR */
			while (ack == ((mmio_read_32(&sysreg->pwrdn_ackn)
						>> 5) & 0x1));			/* [5]: ADB400_BLK_DDR_CFG_BUS_0__SYSREG__PWRDNACKN	*/
			while (ack == ((mmio_read_32(&sysreg->pwrdn_ackn)
						>> 7) & 0x1));			/* [7]: ADB400_BLK_DDR_DATA_BUS_0__SYSREG__PWRDNACKN	*/
			break;
		case BLOCK_MM:
			/* Send the Request - Block MM */
			mmio_set_32(&sysreg->pwrdn_reqn, (0x1 << 1));		/* [1]: ADB400_BLK_MM_CFG_BUS_0__SYSREG__PWRDNREQN	*/
			mmio_set_32(&sysreg->blk_mm_adb, (0x1 << 1) |		/* [1]: BLK_MM_DATA_ADB_DISP__SYSREG__PWRDNREQN		*/
							 (0x1 << 0));		/* [0]: BLK_MM_DATA_ADB_M2M__SYSREG__PWRDNREQN		*/

			/* Waiting for ACK - Block MM */
			while (ack == ((mmio_read_32(&sysreg->pwrdn_ackn)
						>> 3) & 0x1));			/* [3]: ADB400_BLK_MM_CFG_BUS_0__SYSREG__PWRDNACKN	*/
			while (ack == ((mmio_read_32(&sysreg->blk_mm_adb)
						>> 7) & 0x1));			/* [7]: BLK_MM_DATA_ADB_DISP__SYSREG__PWRDNACKN		*/
			while (ack == ((mmio_read_32(&sysreg->blk_mm_adb)
						>> 5) & 0x1));			/* [5]: BLK_MM_DATA_ADB_M2M__SYSREG__PWRDNACKN		*/
			break;
		case BLOCK_USB:
			/* Send the Request - Block USB */
			mmio_set_32(&sysreg->pwrdn_reqn,  (0x1 << 2));		/* [2]: ADB400_BLK_USB_CFG_BUS_0__SYSREG__PWRDNREQN	*/
			mmio_set_32(&sysreg->blk_usb_adb, (0x1 << 0));		/* [0]: BLK_USB_DATA_ADB__SYSREG__PWRDNREQN		*/

			/* Waiting for ACK - Block USB */
			while (ack == ((mmio_read_32(&sysreg->pwrdn_ackn)
						>> 4) & 0x1));			/* [4]: ADB400_BLK_USB_CFG_BUS_0__SYSREG__PWRDNACKN	*/
			while (ack== ((mmio_read_32(&sysreg->blk_usb_adb)
						>> 5) & 0x1));			/* [5]: BLK_USB_DATA_ADB__SYSREG__PWRDNACKN		*/
			break;
	}

}

/* Disable RESET for a block that uses pmu(external power) */
static void pmu_blk_reset_n(unsigned int domain, unsigned int enable)
{
	if (enable) {
		mmio_clear_32(&g_pmu_reg->scpre, (1 << domain));		/* Power On SC */
		mmio_clear_32(&g_pmu_reg->scall, (1 << domain));		/* Power On SC */
		mmio_set_32(&g_pmu_reg->nisolate, (1 << domain));		/* Power On iSolate */
		if (domain == BLOCK_DDR)
			mmio_set_32(&g_pmu_reg->blk_pll_reset_n, (0x3 << 3));
		mmio_set_32(&g_pmu_reg->blk_cmu_reset_n, (0x1 << domain));
		mmio_set_32(&g_pmu_reg->blk_rstcon_reset_n, (0x1 << domain));

	} else {
		mmio_clear_32(&g_pmu_reg->blk_rstcon_reset_n, (0x1 << domain));
		mmio_clear_32(&g_pmu_reg->blk_cmu_reset_n, (0x1 << domain));
		if (domain == BLOCK_DDR)
			mmio_clear_32(&g_pmu_reg->blk_pll_reset_n, (0x3 << 3));
		mmio_clear_32(&g_pmu_reg->nisolate, (1 << domain));		/* Power On iSolate */
		mmio_set_32(&g_pmu_reg->scall, (1 << domain));			/* Power On SC */
		mmio_set_32(&g_pmu_reg->scpre, (1 << domain));			/* Power On SC */
	}
}

void pmu_blk_pwrup(unsigned int domain, unsigned int enable)
{
	pmu_blk_reset_n(domain, enable);
	check_pwrup_blk(domain, enable);
}

void usb_blk_pwrup(void)
{
	/* @breif: Only USB-Block  */
	pmu_blk_reset_n(BLOCK_USB, TRUE);
	check_pwrup_blk(BLOCK_USB, TRUE);
}
