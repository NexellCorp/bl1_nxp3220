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
#include "sysheader.h"
#include "gic.h"

/*******************************************************************************
 * cpu-interface s/w interface accessors for reading entire registers
 ******************************************************************************/
unsigned char *gicc_get_baseaddr(void)
{
	return ((unsigned char *)PHY_BASEADDR_GICC);
}

unsigned int gicc_get_iar(void *base)
{
	return (mmio_read_32((base + GIC_CPUIF_IAR)) & 0xFFFF);
}

/*******************************************************************************
 * cpu-interface s/w interface accessors for writing entire registers
 ******************************************************************************/
void gicc_set_ctrl(void *base, int val)
{
	mmio_set_32((base + GIC_CPUIF_CTRL), val);
}

void gicc_set_eoir(void *base, int val)
{
	int eoir = val & 0xFFFF;

	mmio_write_32((base + GIC_CPUIF_EOIR), eoir);
}

/*******************************************************************************
 * Distributor interface accessors for reading entire registers
 ******************************************************************************/
unsigned char *gicd_get_baseaddr(void)
{
	return ((unsigned char *)PHY_BASEADDR_GICD);
}

/*******************************************************************************
 * Distributor interface accessors for writing entire registers
 ******************************************************************************/
void gicd_set_enable(void *base, int val)
{
	mmio_set_32((base + GIC_DIST_SENABLE), val);
}

void gicd_set_group(void *base, int val)
{
	mmio_write_32((base + GIC_DIST_GROUP), val);
}

void gicd_set_sgir(void *base, int val)
{
	mmio_write_32((base + GIC_DIST_SGIR), val);
}

/*******************************************************************************
 * Setup the ARM GIC CPU and Distributor interfaces.
******************************************************************************/
#if 0
void gic_disp_init(void)
{
	unsigned int dist_base = gic_disp_get_baseaddr();
	unsigned int cpu_base = gic_cpuif_get_baseaddr();

	int nonsecure = 1, secure = 1;
	int i;

	/* CPU Interface Enable */
	mmio_write_32((cpu_base + GIC_CPUIF_CTRL), (3 << 0));

	/*
	 * Whether or not to activate the interrupt
	 * occurs GROUP0/GROUP1 on the GIC.
	 */
	mmio_write_32((dist_base + GIC_DISP_CTRL),
		(nonsecure << 1) | (secure << 0));

	/*
	 * secure/non-secure used to determine
	 * the priority of the interrupt.
	 */
	mmio_write_32((cpu_base + GIC_CPUIF_PRIORTY), 0xFFFFFFFF);

	/*
	 * GIC set the path that is connected to the
	 * internal IP interrupts, generated. (0: Secure, 1: Non-Secure)
	 */
	for (i = 0; i <= 0xC; i += 4)
		mmio_write_32((dist_base + GIC_DISP_GROUP + i), 0xFFFFFFFF);

}
#endif
