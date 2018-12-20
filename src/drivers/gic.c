/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#include "sysheader.h"
#include "gic.h"

/*******************************************************************************
 * cpu-interface s/w interface accessors for reading entire registers
 ******************************************************************************/
unsigned int gicc_get_baseaddr(void)
{
	return ((unsigned int)PHY_BASEADDR_GICC);
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
unsigned int gicd_get_baseaddr(void)
{
	return ((unsigned int)PHY_BASEADDR_GICD);
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
void gic_initialize(unsigned int cpu_id)
{
	struct nx_gicd_reg *dbase = (struct nx_gicd_reg *)gicd_get_baseaddr();
	struct nx_gicc_reg *cbase = (struct nx_gicc_reg *)gicc_get_baseaddr();
	unsigned int index;

	/* Interrupt Prioirty Mask */
	mmio_write_32(&cbase->pmr, 0xFF);

	/* CPU Interface (Secure/Non-Secure) Enable */
	mmio_write_32(&cbase->ctlr, (3 << 0));

	mmio_write_32(&dbase->group[0], 0xFFFFFFFF);

	if (cpu_id != 0)
		return;

	for (index = 0; index < (0x200/4); index++)
		mmio_write_32(&dbase->priorityr[index], 0x0);;

	for (index = 0; index < (0x40/4); index++) {
#if 0
		mmio_write_32(&dbase->cenabler[index], 0xFFFFFFFF);		/* 0x220C1180 ~ 0x220C11FF */
		mmio_write_32(&dbase->cpendr[index], 0xFFFFFFFF);		/* 0x220C1280 ~ 0x220C12FF */
#endif
		/*
		 * GIC set the path that is connected to the
		 * internal IP interrupts, generated. (0: Secure, 1: Non-Secure)
		 */
		mmio_write_32(&dbase->group[index], 0xFFFFFFFF);		/* 0x220C1080 ~ 0x220C10FF */
	}

	mmio_write_32(&dbase->ctlr, 0x0);

	for (index = 0; index < (0x200/4); index++)
		mmio_write_32(&dbase->priorityr[index], 0x80808080);

	mmio_write_32(&dbase->icfgr[9], 0xF5555555);				/* 0xC24 */

	/*
	 * Whether or not to activate the interrupt
	 * occurs GROUP0/GROUP1 on the GIC.
	 */
	mmio_write_32(&dbase->ctlr, 0x3);
}
