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
#include <sysreg.h>
#include <alive.h>
#include <gpio.h>

/* external global virables */
extern struct nx_vddpwr_reg *g_vddpwr_reg;
extern struct nx_alive_reg *g_alive_reg;

#define VDDPWRON_DDR_GPIO_NUM		11

void vddpwron_ddr_on(unsigned int pad)
{
	unsigned int value = (1 << pad);

	/* @brief: Disable the Pull Enable */
	mmio_set_32(&g_alive_reg->gpio_pullupenb_rst, value);
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_rst, value);
	mmio_set_32(&g_alive_reg->gpio_inputenb_rst, value);

	/* step xx. set the alive gpio */
	mmio_set_32(&g_alive_reg->gpio_padout_rst, value);			// outpad - low
	mmio_set_32(&g_alive_reg->gpio_padout_set, value);			// outpad - high
	mmio_set_32(&g_alive_reg->gpio_pad_outenb_set, value);			// output enable
}

#define HRESETO_GPIO_NUM		10

void hreseto_set_repwron(unsigned int pad_num)
{
	unsigned int bit_pos = (pad_num * 2);

	/* @brief: change the alternate 1 function (alive gpio 10) */
	mmio_clear_32(&g_vddpwr_reg->alive_gpio_altfn_sel_low,
		(0x3 << bit_pos));
	mmio_set_32(&g_vddpwr_reg->alive_gpio_altfn_sel_low,
		(0x1 << bit_pos));

	/* @brief: Time to keep 'HRESETO' low. */
	mmio_write_32(&g_vddpwr_reg->vddoff_delay_for_repower_on, 0xFFFF);

	mmio_set_32(&g_vddpwr_reg->repoweron_enb_reg, (1 << 0));

	mmio_clear_32(&g_vddpwr_reg->alive_hard_reset_enb, (1 << 0));
}

void watchdog_reset(void)
{
	/* @brief: reset the secure watch dog timer */
	mmio_set_32(&g_vddpwr_reg->wdt_rst_enb, 0x1);
}

void system_initialize(void)
{
	struct nx_sysreg_sys_reg *sys_reg
		= ((struct nx_sysreg_sys_reg *)PHY_BASEADDR_SYSREG_SYS);
	/*
	 * @brief: Change access permission to all bits
	 * of GPIO A~F to non-secure.
	 */
	nx_gpio_bit_access();

	/* @brief: Disable pull-up for VDDPOWRON pin  */
	vddpwron_ddr_on(VDDPWRON_DDR_GPIO_NUM);

	/* @brief: Enable the HRESETO function  */
	hreseto_set_repwron(HRESETO_GPIO_NUM);

	watchdog_reset();

	/* @brief: The can register, which must be set with secure permission.*/
	mmio_set_32(&sys_reg->can[0].can_cok, 0x1);
	mmio_set_32(&sys_reg->can[1].can_cok, 0x1);

	/* @brief: NIC400 GPV (DDR Security0 : Slave) */
	mmio_set_32((PHY_BASEADDR_SYS_BUS_GPV  + 0x8), 1);
}
