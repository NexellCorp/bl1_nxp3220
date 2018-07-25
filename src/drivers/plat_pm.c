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
#include <cpupmu.h>
#include <alive.h>
#include <memory.h>
#include <sysreg.h>
#include <checker.h>

#define NX_SUSPEND_SIGNATURE			0x57505200

struct nx_vddpwr_reg *g_vddpwr_reg =
	((struct nx_vddpwr_reg *)PHY_BASEADDR_VDDPWR);
struct nx_alive_reg *g_alive_reg =
	((struct nx_alive_reg *)PHY_BASEADDR_ALIVE);
struct nx_sysreg_cpu_reg *g_syscpu_reg =
	((struct nx_sysreg_cpu_reg *)PHY_BASEADDR_SYSREG_CPU);

void (*enter_self_refresh)(void);

/* External Variable */
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
	return mmio_read_32(&g_vddpwr_reg->new_scratch[0]);
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

static void hold_pad_reten_n(unsigned int pad_num)
{
	/* step xx. clear the alive gpio  */
	mmio_set_32(&g_alive_reg->gpio_pullupenb_rst, (1 << pad_num));
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_rst, (1 << pad_num));
	mmio_set_32(&g_alive_reg->gpio_inputenb_rst, (1 << pad_num));

	/* step xx. set the alive gpio */
	mmio_clear_32(&g_alive_reg->gpio_pullupenb_set, (1 << pad_num));	// pull-down
	mmio_clear_32(&g_alive_reg->gpio_pad_outenb_set, (1 << pad_num));	// output enable - off
	mmio_clear_32(&g_alive_reg->gpio_inputenb_set, (1 << pad_num));		// input enable - on

	/* step xx. change the 'VDDPWRON_DDR' alternate  */
	mmio_clear_32(&g_vddpwr_reg->alive_gpio_altfn_sel_low, (1 << pad_num));	// set the alt alive gpio
//	mmio_clear_32(&g_vddpwr_reg->alive_gpio_altfn_sel_high, (1 << pad_num));// set the alt alive gpio
}

static void system_vdd_pwroff(void)
{
	/* step 01. disable the CPU WFI */
	mmio_write_32(&g_vddpwr_reg->disable_cpu_wfi, (1 << 0));

	/* step 02. all alive pend pending clear until power down. */
	mmio_write_32(&g_alive_reg->gpio_detect_pend, 0xFF);

	/* step 03. 1 Cycle(Unit: 21.333us) (24Mhz Osillator Clock(40ns)*512) */
	mmio_write_32(&g_alive_reg->vddoffcnt_value, 0x00000001);

	/* step 04. VDD Control ([1]: VDDPWERON_DDR, [0]:VDDPOWRON) */
	mmio_clear_32(&g_alive_reg->vddctrl_read, 0x1);

	/* step 05. VDD Off -> Wakeup Start Delay  */
	mmio_clear_32(&g_vddpwr_reg->vddoff_delay_for_wakeup_mask, 0x0000000A);

	/* step 06. VDDPOWERON Off, start counting down. */
	mmio_write_32(&g_vddpwr_reg->vddoff_start, (1 << 0));

	while(1);
}

void suspend_mark(unsigned int base, unsigned int size, unsigned int entry_point)
{
	unsigned int hash[256/4];

	sss_generate_hash(base, size, (unsigned char*)&hash);

	mmio_write_32(&g_vddpwr_reg->new_scratch[0], NX_SUSPEND_SIGNATURE);
	mmio_write_32(&g_vddpwr_reg->new_scratch[1], entry_point);
	mmio_write_32(&g_vddpwr_reg->new_scratch[2], base);
	mmio_write_32(&g_vddpwr_reg->new_scratch[3], size);
	mmio_write_32(&g_vddpwr_reg->new_scratch[4], hash[0]);
}

void system_suspend(unsigned int entry_point)
{
	/* step xx. set 'PAD_RETEN_N' to keep retension state.*/
	hold_pad_reten_n(PAD_VDDPWRON_DDR_GPIO_NUM);

	/* @brief: mark the suspend the signature (and hash) */
	suspend_mark(0x40000000, (120 * 1024), entry_point);

	/* step xx. enter the ddrx self-refresh */
	enter_self_refresh();

	/* step xx. set the vddpwr off */
	system_vdd_pwroff();
}


void system_resume(void)
{

}

void system_power_off(void)
{
	system_vdd_pwroff();
}
