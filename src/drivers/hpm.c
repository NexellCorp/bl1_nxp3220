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
#include <hpm.h>
#include <sysreg.h>
#include <chip.h>
#include <alive.h>

struct nx_vddpwr_reg *g_aliveNew_reg =
	((struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR);
struct nx_sysreg_sys_reg *g_sysreg =
	((struct nx_sysreg_sys_reg *)PHY_BASEADDR_SYSREG_SYS);

struct nx_cpusys_reg *g_cpusys_reg =
	((struct nx_cpusys_reg *)PHY_BASEADDR_CPUSYS_MOUDLE);

struct nx_cmusys_reg *g_cmusys_reg =
	((struct nx_cmusys_reg *)PHY_BASEADDR_CMU_SRC_MODULE);


void read_hpm_core(struct nx_hpm_info *info_hpm)
{
	unsigned int ro_sel = 0; //INV_D2_LVT
	unsigned int config = 3; //256 cycles
	int val;

#if 1
	mmio_write_32(&g_sysreg->hpm[0], 0);
	val = (ro_sel << 16) | (config << 4) | 0x05;
	mmio_write_32(&g_sysreg->hpm[0], val);

	do {
	        val = mmio_read_32(&g_sysreg->hpm[1]);
	} while (!(val & HPM_SYNC_TO_APM));

	val = mmio_read_32(&g_sysreg->hpm[1]);
#endif
#if 0	
        mmio_set_32(ecid_mod->core_reg, HPM_ENABLE, 0);
        mmio_set_32(ecid_mod->core_reg, HPM_CONFIG, config << 4);
        mmio_set_32(ecid_mod->core_reg, HPM_RO_SEL, ro_sel << 16);
        mmio_set_32(ecid_mod->core_reg, HPM_PREDELAY, 0);
        mmio_set_32(ecid_mod->core_reg, HPM_SYNC_APM, 0);

        mmio_set_32(ecid_mod->core_reg, HPM_SYNC_APM, 1<<2);
        mmio_read_32(ecid_mod->core_reg, &val);
        mmio_set_32(ecid_mod->core_reg, HPM_ENABLE, 1);

        mmio_read_32(ecid_mod->core_reg, &val);
        do {
                mmio_read_32(ecid_mod->core_reg, &val);
        } while (!(val & HPM_SYNC_TO_APM));
#endif
	info_hpm->core_hpm	= val & 0x3ff;

	mmio_write_32(&g_aliveNew_reg->new_scratch[16], info_hpm->core_hpm);
}

/* Since this code is chip dependent, it can be changed. Will be implemented later. */
void hpm_parser(struct nx_hpm_info *info_hpm)
{
	unsigned int ro_sel = 0; //INV_D2_LVT
	unsigned int config = 3; //256 cycles
	int val;

	mmio_write_32(&g_cpusys_reg->hpm0, 0);
	val = (ro_sel << 16) | (config << 4) | 0x05;
	mmio_write_32(&g_cpusys_reg->hpm0, val);

	do {
	        val = mmio_read_32(&g_cpusys_reg->hpm1);
	} while (!(val & HPM_SYNC_TO_APM));

	val = mmio_read_32(&g_cpusys_reg->hpm1);

	info_hpm->cpu_hpm	= val & 0x3ff;

	mmio_write_32(&g_aliveNew_reg->new_scratch[17], info_hpm->cpu_hpm);
}
