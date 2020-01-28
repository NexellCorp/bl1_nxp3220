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
#include <cpupmu.h>
#include <alive.h>
#include <memory.h>
#include <sysreg.h>
#include <checker.h>
#include <plat_load.h>

#define NX_SUSPEND_SIGNATURE			0x57505200
#define NX_SUSPEND_HASH_ADDR			0x41000000
#define SUSPEND_HASH_FULL_SIZE
#ifdef SUSPEND_HASH_FULL_SIZE
#define NX_SUSPEND_HASH_SIZE			(496 * 1024 * 1024)
#else
/* hash only kernel size */
#define NX_SUSPEND_HASH_SIZE			(10 * 1024 * 1024)
#endif
#define NX_HASH_SIZE				(256/32)
#define USE_RTC					(0x2008C000 + 0x024)

struct nx_vddpwr_reg *g_vddpwr_reg =
	((struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR);
struct nx_alive_reg *g_alive_reg =
	((struct nx_alive_reg *)PHY_BASEADDR_ALIVE);
struct nx_sysreg_cpu_reg *g_syscpu_reg =
	((struct nx_sysreg_cpu_reg *)PHY_BASEADDR_SYSREG_CPU);

void (*enter_self_refresh)(void);
void (*exit_self_refresh)(void);
void (*pmic_poweroff)(void);

/* External Variable */
extern struct platform_manager *g_ppi;
extern volatile int g_subcpu_num;
extern volatile int g_cpu_kill_num;

void smp_enable(int enable)
{
	int value;

	value = armv7_get_auxctrl();
	value &= ~(1 << 6);

	if (enable)
		value |= (1 << 6);

	armv7_set_auxctrl(value);
}

void dmb(void)
{
	__asm__ __volatile__ ("dmb");
}

int check_suspend_state(void)
{
	unsigned int reg_value;

	reg_value = mmio_read_32(&g_vddpwr_reg->new_scratch[0]);
	if (reg_value != NX_SUSPEND_SIGNATURE)
		return FALSE;

	return TRUE;
}

int system_cpu_check(unsigned int cpu_id)
{
	unsigned int status;

	status = ((mmio_read_32(&g_syscpu_reg->ctrl[4]) >> cpu_id) & 0x1);

	/* @brief: return value (0: on, 1:off, 2: pending) */
	if (status)
		status = 0;
	else
		status = 1;

	return status;
}

int system_cpu_on(unsigned int cpu_id)
{
	if (cpu_id != 1)
		return -1;

	/* @brief: set the high vector */
	mmio_set_32(&g_syscpu_reg->ctrl[0], (0x1 << (4 + 1)));

	dmb();
	cpu_on_sequence(cpu_id);
	dmb();

	return cpu_id;
}

int system_cpu_off(unsigned int cpu_id)
{
	if (cpu_id != 1)
		return -1;

	dmb();
	cpu_off_sequence(cpu_id);
	dmb();

	return cpu_id;
}

void system_core_reset(void)
{
	mmio_set_32(&g_vddpwr_reg->reg_core_swrst_rst, (1 << 0));
	mmio_set_32(&g_vddpwr_reg->reg_core_swrst_rst_enb, (1 << 0));
}

#define PAD_VDDPWRON_DDR_GPIO_NUM		11
#define PHY_PAD_CTRL				(0x23091000 + 0x120)

static void hold_pad_reten_n(unsigned int pad_num)
{
	unsigned int index  = (((pad_num * 2) >= 32) ? 1 : 0);
	unsigned int bit_pos = (pad_num * 2);
	unsigned int reg_value;

	mmio_set_32(&g_alive_reg->gpio_pullupenb_rst, (1 << pad_num));
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_rst, (1 << pad_num));
	mmio_set_32(&g_alive_reg->gpio_inputenb_rst, (1 << pad_num));

	mmio_set_32(&g_alive_reg->gpio_padout_rst, (1 << pad_num));		/* outpad - low   */
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_set, (1 << pad_num));		/* output enable  */

	/* step xx. change the 'VDDPWRON_DDR' alternate  */
	if (index)
		mmio_clear_32(&g_vddpwr_reg->alive_gpio_altfn_sel_high,
			(0x3 << bit_pos));
	else
		mmio_clear_32(&g_vddpwr_reg->alive_gpio_altfn_sel_low,
			(0x3 << bit_pos));
	do {
		reg_value = mmio_read_32(PHY_PAD_CTRL);
		reg_value = ((reg_value >> 31) & 0x1);
	} while (reg_value != 0);

}

static void system_vdd_pwroff(void)
{
	/* step 01. Disable the Hardware Reset Enable */
	mmio_clear_32(&g_vddpwr_reg->alive_hard_reset_enb, (1 << 0));

	/* step 02. Disable the Repowron Enable */
	mmio_clear_32(&g_vddpwr_reg->repoweron_enb_reg, (1 << 0));

	/* step 03. disable the CPU WFI */
	mmio_clear_32(&g_vddpwr_reg->disable_cpu_wfi, (1 << 0));

	/* step 04. 1 Cycle(Unit: 21.333us) (24Mhz Osillator Clock(40ns)*512) */
	mmio_write_32(&g_alive_reg->vddoffcnt_value, 0x0000000);

	/* step 05. VDD Control ([1]: VDDPWERON_DDR, [0]:VDDPOWRON) */
	mmio_clear_32(&g_alive_reg->vddctrl_read, 0x3);

	if ((mmio_read_32(USE_RTC) & 0x1) == 0x1) {
		/* if rct use, disable pwr xti */
		mmio_write_32(&g_vddpwr_reg->vddpwr_on_xti, 0x0);
	}

	/* step 06. VDD Off -> Wakeup Start Delay  */
	mmio_write_32(&g_vddpwr_reg->vddoff_delay_for_wakeup_mask, 1000);
	mmio_write_32(&g_vddpwr_reg->vddoff_delay_for_repower_on, 1000);

	/* step 07. VDDPOWERON Off, start counting down. */
	mmio_write_32(&g_vddpwr_reg->vddoff_start, (1 << 0));

	while(1);
}

int check_suspend_hash(void)
{
	unsigned int hash[NX_HASH_SIZE], s_hash;
	unsigned int base, size;

	base = mmio_read_32(&g_vddpwr_reg->new_scratch[2]);
	size = mmio_read_32(&g_vddpwr_reg->new_scratch[3]);
	s_hash = mmio_read_32(&g_vddpwr_reg->new_scratch[4]);

	sss_generate_hash(base, size, (unsigned char*)&hash);

	NOTICE("Generate Hash: 0x%08X, Stored Hash: 0x%08X \r\n", s_hash, hash[0]);

	if (s_hash != hash[0])
		return false;

	return true;
}

void suspend_mark(unsigned int base, unsigned int size, unsigned int entry_point)
{
	unsigned int hash[NX_HASH_SIZE];

	sss_generate_hash(base, size, (unsigned char*)&hash);

	mmio_write_32(&g_vddpwr_reg->new_scratch[0], NX_SUSPEND_SIGNATURE);
	mmio_write_32(&g_vddpwr_reg->new_scratch[1], entry_point);
	mmio_write_32(&g_vddpwr_reg->new_scratch[2], base);
	mmio_write_32(&g_vddpwr_reg->new_scratch[3], size);
	mmio_write_32(&g_vddpwr_reg->new_scratch[4], hash[0]);
	mmio_write_32(&g_vddpwr_reg->new_scratch[5], g_ppi->s_launch_addr);
}

void suspend_mark_clear(void)
{
	mmio_write_32(&g_vddpwr_reg->new_scratch[0], 0);
	mmio_write_32(&g_vddpwr_reg->new_scratch[1], 0);
	mmio_write_32(&g_vddpwr_reg->new_scratch[2], 0);
	mmio_write_32(&g_vddpwr_reg->new_scratch[3], 0);
	mmio_write_32(&g_vddpwr_reg->new_scratch[4], 0);
	mmio_write_32(&g_vddpwr_reg->new_scratch[5], 0);
}

#define RSTRELEASE_DDR_AXI			(0x23000000 + 0x400 + 0x30)
#define RSTENTER_DDR_AXI			(0x23000000 + 0x400 + 0x40)

void system_suspend(unsigned int entry_point)
{
	/* @brief: mark the suspend the signature (and hash) */
	suspend_mark(NX_SUSPEND_HASH_ADDR, NX_SUSPEND_HASH_SIZE, entry_point);

	/* @brief: when going suspend state, disable mmu */
	disable_mmu();

	dmb();

	/* step xx. enter the ddrx self-refresh */
	enter_self_refresh();

	dmb();

	/* step xx. set 'PAD_RETEN_N' to keep retension state.*/
	hold_pad_reten_n(PAD_VDDPWRON_DDR_GPIO_NUM);

	ldelay(300);

	/* @brief: DDR-PHY Global reset is asserted. */
	mmio_write_32(RSTENTER_DDR_AXI, (1 << 0));

	/* Must waitng 100ns */
	ldelay(100);

	/* step xx. set the vddpwr off */
	system_vdd_pwroff();

	while(1);
}

int system_resume(unsigned int *is_resume, unsigned int is_secure_os,
	unsigned int *s_launch, unsigned int *ns_launch)
{
	int success;

	success = check_suspend_hash();
	if (success == 0) {
		if (is_secure_os)
			*s_launch = mmio_read_32(&g_vddpwr_reg->new_scratch[5]);
		*ns_launch = mmio_read_32(&g_vddpwr_reg->new_scratch[1]);
	} else {
		*is_resume = FALSE;
		ERROR("Resume failure due to invalid hash value!! \r\n");
		NOTICE("Performs a Cold Boot Sequence. \r\n");
	}

	return success;
}

void system_power_off(void)
{
	/* @breif: Power Management IC - Power OFF */
	pmic_poweroff();

	/* Waiting for Power OFF */
	while(1);
}
