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
#include <gpio.h>

static struct nx_gpio_regs (*const g_gpio_regs)[1] =
	(struct nx_gpio_regs(*)[])(PHY_BASEADDR_GPIOA_SECURE_MODULE);

void gpio_set_outenb(int module, int pad, int enb)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	if (enb) {
		mmio_set_32(&gpio_reg->outenb, (1 << pad));
		mmio_clear_32(&gpio_reg->inputenb, (1 << pad));
	} else {
		mmio_clear_32(&gpio_reg->outenb, (1 << pad));
		mmio_set_32(&gpio_reg->inputenb, (1 << pad));
	}
}

void gpio_set_output(int module, int pad, int val)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	if (val)
		mmio_set_32(&gpio_reg->out, (1 << pad));
	else
		mmio_clear_32(&gpio_reg->out, (1 << pad));
}

int gpio_get_pad(int module, int pad)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	return ((mmio_read_32(&gpio_reg->pad) >> pad) & 0x1);
}

void gpio_set_pullenb(int module, int pad, int enb)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module]);

	if (enb) {
		mmio_set_32(&gpio_reg->pullenb, (1 << pad));
		mmio_set_32(&gpio_reg->pullenb_disable_default, (1 << pad));
	} else {
		mmio_clear_32(&gpio_reg->pullenb, (1 << pad));
		mmio_clear_32(&gpio_reg->pullenb_disable_default, (1 << pad));
	}
}

void gpio_set_pull(int module, int pad, int sel)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	if (sel) {
		mmio_set_32(&gpio_reg->pullsel, (1 << pad));
		mmio_set_32(&gpio_reg->pullsel_disable_default, (1 << pad));
	} else {
		mmio_clear_32(&gpio_reg->pullsel, (1 << pad));
		mmio_clear_32(&gpio_reg->pullsel_disable_default, (1 << pad));
	}
}

void gpio_set_alt_function(int module, int pad, int alt)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	int reg_num = (pad >> 4);
	int alt_clear = ((pad & 0xF) * 2);
	int reg_value, reg_value_ex;

	reg_value = mmio_read_32(&gpio_reg->altfn[reg_num]);
	reg_value &= ~(0x3 << alt_clear);
	reg_value |= ((alt & 0x3) << alt_clear);

	reg_value_ex = mmio_read_32(&gpio_reg->altfn_ex);
	reg_value_ex &= ~(0x1 << (pad & 0xF));
	reg_value_ex |= (((alt >> 2) & 0x1) << pad);

	mmio_write_32(&gpio_reg->altfn_ex, reg_value_ex);
	mmio_write_32(&gpio_reg->altfn[reg_num], reg_value);
}

void nx_gpio_set_alt(const union nx_gpio_pad *pad)
{
	gpio_set_alt_function((int)pad->padi.group,
			(int)pad->padi.pad, (int)pad->padi.alternate);
}

void nx_gpio_s_mark(int module, int pad, int mark)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);
	int reg_value = (1 << pad);

	if (mark)
		mmio_set_32(&gpio_reg->secure_marking_reg, reg_value);
	else
		mmio_clear_32(&gpio_reg->secure_marking_reg, reg_value);
}

void nx_gpio_s_mark32(int module, int value)
{
	register struct nx_gpio_reg *gpio_reg =
		(struct nx_gpio_reg *)(&g_gpio_regs[module & 0x7]);

	mmio_write_32(&gpio_reg->secure_marking_reg, value);
}

void nx_gpio_bit_access(void)
{
	int module;

	for (module = 0; module < 5; module++)
		nx_gpio_s_mark32(module, 0x0);
}
