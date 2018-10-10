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
#ifndef __CHIP_H__
#define __CHIP_H__

/* CPU Base Address */
#define PHY_BASEADDR_CPUPMU_MOUDLE		0x22010000
#define PHY_BASEADDR_AXIM_CPU_MODULE		0x22080000

/* Armv7 GIC 2.0 Base Address */
#define PHY_BASEADDR_GIC			0x220C0000
#define PHY_BASEADDR_GICC			(PHY_BASEADDR_GIC + 0x2000)
#define PHY_BASEADDR_GICD			(PHY_BASEADDR_GIC + 0x1000)

/* Power Management Unit Base Address */
#define PHY_BASEADDR_CLKPWR			0x20080000
#define PHY_BASEADDR_ALIVE			(PHY_BASEADDR_CLKPWR + 0xD000)
#define PHY_BASEADDR_PMU			(PHY_BASEADDR_CLKPWR + 0xC400)
#define PHY_BASEADDR_VDDPWR			(PHY_BASEADDR_CLKPWR + 0xC800)

/* PLL Base Address PLL 0,1, CPU, DDR 0,1 */
#define PHY_BASEADDR_PLL0			0x27020000
#define PHY_BASEADDR_PLL1			0x27030000
#define PHY_BASEADDR_PLL_CPU			0x22070000
#define PHY_BASEADDR_PLL_DDR0			0x23070000
#define PHY_BASEADDR_PLL_DDR1			0x23080000

/* CMU Base Address SRC, SYS, DDR */
#define PHY_BASEADDR_CMU_SRC_MODULE		0x27000000
#define PHY_BASEADDR_CMU_SYS_MODULE		0x27010000
#define PHY_BASEADDR_CMU_CPU_MODULE		0x22000000
#define PHY_BASEADDR_CMU_DDR_MODULE		0x23000000

#define PHY_BASEADDR_CMU_USB_MODULE		0x24000000
#define PHY_BASEADDR_CMU_MM_MODULE		0x25000000
#define PHY_BASEADDR_CMU_HSIF_MODULE		0x26000000

/* SYS Base Address */
#define PHY_BASEADDR_SYS_MODULE			0x20030000
#define PHY_BASEADDR_SYSREG_SYS			(PHY_BASEADDR_SYS_MODULE)
#define PHY_BASEADDR_SYSREG_CPU			(PHY_BASEADDR_SYS_MODULE + 0x2000000)
#define PHY_BASEADDR_SYSREG_DDR			(PHY_BASEADDR_SYS_MODULE + 0x3000000)
#define PHY_BASEADDR_SYSREG_USB			(PHY_BASEADDR_SYS_MODULE + 0x4000000)
#define PHY_BASEADDR_SYSREG_MM			(PHY_BASEADDR_SYS_MODULE + 0x5000000)
#define PHY_BASEADDR_SYSREG_HSIF		(PHY_BASEADDR_SYS_MODULE + 0x6000000)

/* MM Base Address */
#define PHY_BASEADDR_SYSREG_MM_MODULE		0x25030000
#define PHY_BASEADDR_SYSREG_MM_MODULE_APB	0x25030000
#define PHY_BASEADDR_SYSREG_MM_MODULE_SECURE	0x25040000

/* DDR Base Address */


/* TZC400 Base Address */
#define PHY_BASEADDR_TZC400			0x230C0000

/* HSIF Base Address */
#define PHY_BASEADDR_SYSREG_HSIF_MODULE		0x26030000
#define PHY_BASEADDR_SYSREG_HSIF_MODULE_APB	0x26030000
#define PHY_BASEADDR_SYSREG_HSIF0_MODULE_SECURE	0x26040000

/* USB Base Address */
#define PHY_BASEADDR_USBOTG20_MODULE		0x240C0000

#define PHY_BASEADDR_SYSREG_USB_MODULE		0x24030000
#define PHY_BASEADDR_SYSREG_USB_MODULE_APB	0x24030000
#define PHY_BASEADDR_SYSREG_USB_MODULE_SECURE	0x24040000


/* GPIO Base Address */
#define PHY_BASEADDR_GPIOA_NONSECURE_MODULE	0x20180000
#define PHY_BASEADDR_GPIOB_NONSECURE_MODULE	0x20190000
#define PHY_BASEADDR_GPIOC_NONSECURE_MODULE	0x201A0000
#define PHY_BASEADDR_GPIOD_NONSECURE_MODULE	0x201B0000
#define PHY_BASEADDR_GPIOE_NONSECURE_MODULE	0x201C0000
#define PHY_BASEADDR_GPIOF_NONSECURE_MODULE	0x201D0000
#define PHY_BASEADDR_GPIOA_SECURE_MODULE	0x20200000
#define PHY_BASEADDR_GPIOB_SECURE_MODULE	0x20210000
#define PHY_BASEADDR_GPIOC_SECURE_MODULE	0x20220000
#define PHY_BASEADDR_GPIOD_SECURE_MODULE	0x20230000
#define PHY_BASEADDR_GPIOE_SECURE_MODULE	0x20240000
#define PHY_BASEADDR_GPIOF_SECURE_MODULE	0x20250000

/* SDMMC Base Address */
#define PHY_BASEADDR_SDMMC0_MODULE		0x26080000
#define PHY_BASEADDR_SDMMC1_MODULE		0x26090000
#define PHY_BASEADDR_SDMMC2_MODULE		0x260A0000

/* SPI Base Address */
#define PHY_BASEADDR_SPI0_MODULE		0x30000000
#define PHY_BASEADDR_SPI1_MODULE		0x20A90000
#define PHY_BASEADDR_SPI2_MODULE		0x20AA0000

/* NAND Base Addres */
#define PHY_BASEADDR_NANDC_MODULE		0x26180000

/* UART Base Address Channel 0 ~ 7 */
#define PHY_BASEADDR_UART_CH0			0x20400000
#define PHY_BASEADDR_UART_CH1			0x20410000
#define PHY_BASEADDR_UART_CH2			0x20420000
#define PHY_BASEADDR_UART_CH3			0x20430000
#define PHY_BASEADDR_UART_CH4			0x20440000
#define PHY_BASEADDR_UART_CH5			0x20450000
#define PHY_BASEADDR_UART_CH6			0x20460000
#define PHY_BASEADDR_UART_CH7			0x20470000

/* EFuse Base Address */
#define PHY_BASEADDR_ECID_SECURE_MODULE		0x20070000

/* F/S SYS_BUS_GPV Address */
#define PHY_BASEADDR_SYS_BUS_GPV		0x21000000
#define PHY_BASEADDR_SYS_BUS_GPV_M0		0x21042000
#define PHY_BASEADDR_SYS_BUS_GPV_M1		0x21043000
#define PHY_BASEADDR_SYS_BUS_GPV_M2		0x21044000
#define PHY_BASEADDR_SYS_BUS_GPV_M3		0x21045000
#define PHY_BASEADDR_SYS_BUS_GPV_M4		0x21046000

#define PHY_BASEADDR_FSYS_BUS_GPV		0x21100000

/* SSS Base Address */
#define PHY_BASEADDR_SSS_MODULE_SSS		0x20D00000
#define SSS_MAILBOX_BASEADDR			0x20D10000

#define PHY_BASEADDR_SSS_MODULE_HOST_KEYMAN	0x20D30000

#endif // __CHIP_H__
