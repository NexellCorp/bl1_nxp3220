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
#include <tz.h>

union tzinfo_reg sys_buscfg[5] = {
	/* sys_buscfg[0] -> BUSCFG6 */
	[0].bit = {
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [00]:AXIM_MDMA_0, [01]: AXIM_MP2TSI_1, [02]: AXIM_MP2TSI_0, [03]: RSVD		*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [04]:AXIM_SSS_0,  [05]: RSVD,          [06]: RSVD,          [07]: AXIM_CSSYS_0	*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [08]:AXIM_PDM_0,  [09]: AXIM_SDMA_1,   [10]: AXIM_SDMA_0,   [11]: AXIM_DMA_1		*/
		SECURE_ONLY, RESERVED   , RESERVED   , RESERVED   ,		/* [12]:AXIM_DMA_0,  [13:31]: RESERVED   						*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	},
	/* sys_buscfg[1] -> BUSCFG7 */
	[1].bit = {
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, RESERVED   ,		/* [00]:AXIM_SDMA_0,   		[01]: AXIM_DMA_1,		[02]: AXIM_DMA_0,		[03]: RSVD 				*/
		SECURE_ONLY, SECURE_ONLY, RESERVED   , RESERVED   ,		/* [04]:CAN_0__SYSREG__CAN,     [05]: CAN_0__SYSREG__RAM,	[06]: RSVD,			[07]: RSVD				*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [08]:GPIO_4__SYSREG__SECURE, [09]: GPIO_3__SYSREG__SECURE,	[10]: GPIO_2__SYSREG__SECURE,	[11]: GPIO_1__SYSREG__SECURE		*/
		SECURE_ONLY, RESERVED   , RESERVED   , EVERY_ONE  ,		/* [12]:GPIO_0__SYSREG__SECURE, [13]: RSVD,			[14]: RSVD,			[15]: GPIO_4				*/
		EVERY_ONE  , EVERY_ONE  , EVERY_ONE  , EVERY_ONE  ,		/* [16]:GPIO_3,			[17]: GPIO_2,			[18]: GPIO_1,			[19]: GPIO_0				*/
		RESERVED   , SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [20]:RSVD,			[21]: SMC_0__SYSREG__CFG,	[22]: CAN_1__SYSREG__CAN,	[23]: CAN_1__SYSREG__RAM		*/
		SECURE_ONLY, RESERVED   , SECURE_ONLY, SECURE_ONLY,		/* [24]:TMU_0,			[25]: RSVD,     		[26]: SYSCTRLTOP_0__SYSREG__NONSECURE,	[27]: SYSCTRLTOP_0		*/
		SECURE_ONLY, EVERY_ONE  , SECURE_ONLY, SECURE_ONLY,		/* [28]:ECID_0__SYSREG__SECURE, [29]: ECID_0,			[30]: SYSREG_SYS_0__SYSREG__SECURE,	[31]: SYSREG_SYS_0		*/
	},
	/* sys_buscfg[2] -> BUSCFG8 */
	[2].bit = {
		EVERY_ONE  , SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [00]:PWM_0,		[01]: SECURE_WDT_0,	[02]: WDT_0,	        [03]: I2S_3		*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, RESERVED,		/* [04]:I2S_2,		[05]: I2S_1,		[06]: I2S_0,		[07]: RSVD		*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [08]:ADC_0,		[09]: I2C_4,		[10]: I2C_3,		[11]: I2C_2		*/
		SECURE_ONLY, SECURE_ONLY, RESERVED   , EVERY_ONE  ,		/* [12]:I2C_1,		[13]: I2C_0,		[14]: RSVD,		[15]: UART_6		*/
		EVERY_ONE  , EVERY_ONE  , EVERY_ONE  , EVERY_ONE  ,		/* [16]:UART_5,		[17]: UART_4,		[18]: UART_3,		[19]: UART_2		*/
		EVERY_ONE  , EVERY_ONE  , SECURE_ONLY, SECURE_ONLY,		/* [20]:UART_1,		[21]: UART_0,		[22]: AXIM_MDMA_0,	[23]: AXIM_PDM_0	*/
		SECURE_ONLY, SECURE_ONLY, RESERVED   , SECURE_ONLY,		/* [24]:AXIM_MP2TSI_1,	[25]: AXIM_MP2TSI_0,	[26]: RSVD,		[27]: AXIM_SSS_0	*/
		RESERVED   , RESERVED   , SECURE_ONLY, SECURE_ONLY,		/* [28]:RSVD,		[29]: RSVD,		[30]: AXIM_CSSYS_0,	[31]: AXIM_SDMA_1	*/
	},
	/* sys_buscfg[3] -> BUSCFG9 */
	[3].bit = {
		EVERY_ONE  , EVERY_ONE  , SECURE_ONLY, SECURE_ONLY,		/* [00]:CMU_SRC_0,	[01]: CMU_SYS_0,	[02]: FSYS_BUS_0__SYSREG__GPV, [03]: DUMMY_0	*/
		SECURE_ONLY, EVERY_ONE  , EVERY_ONE  , RESERVED   ,		/* [04]:SSS_0__SYSREG__PUF, [05]: PLL_0,	[06]: PLL_1,		[07]: RSVD		*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [08]:PDM_0,		[09]: MP2TSI_1,		[10]: MP2TSI_0,		[11]: SPDIFRX_0		*/
		SECURE_ONLY, SECURE_ONLY, RESERVED   , SECURE_ONLY,		/* [12]:SPDIFTX_0,	[13]: SPI_2,		[14]: SPI_1,		[15]: MDMA_0		*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [16]:SDMA_1,		[17]: SDMA_0,		[18]: DMA_1,		[19]: DMA_0		*/
		SECURE_ONLY, EVERY_ONE  , SECURE_ONLY, RESERVED   ,		/* [20]:SECURE_TIMER_0,	[21]: TIMER_0_SYSREG_,	[22]: SSS_0__SYSREG__HOST_KEYMAN, [23:31]: RSVD */
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	},
	/* sys_buscfg[4] -> BUSCFG10 */
	[4].bit = {
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [00]:SSS_0__SYSREG__SSS,     	[01]: SSS_0__SYSREG__MAILBOX,  		[02]: SSS_0__SYSREG__KEYMAN,		[03]: SYS_BUS_0__SYSREG__ETC_TZPROT				*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [04]:SYS_BUS_0__SYSREG__DMA_TZPROT,	[05]: SPI_0,	      			[06]: CFG_BUS_0, 			[07]: ETC_BUS_0				*/
		SECURE_ONLY, RESERVED   , RESERVED   , RESERVED   ,		/* [08]:DMA_BUS_0,			[09]: RSVD,				[10]: RSVD,				[11]: RSVD				*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,		/* [12]:RSVD,				[13]: RSVD,				[14]: RSVD,				[15]: RSVD				*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [16]:SYS_BUS_0__SYSREG__CFG_TZPROT,	[17]: SYS_BUS_0__SYSREG__CFG_TZPROT,	[18]: SYS_BUS_0__SYSREG__CFG_TZPROT, 	[19]: SYS_BUS_0__SYSREG__CFG_TZPROT	*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [20]:SYS_BUS_0__SYSREG__CFG_TZPROT,	[21]: SYS_BUS_0__SYSREG__CFG_TZPROT,	[22]: SYS_BUS_0__SYSREG__CFG_TZPROT,	[23]: SYS_BUS_0__SYSREG__CFG_TZPROT	*/
		SECURE_ONLY, SECURE_ONLY, RESERVED   , RESERVED   ,		/* [24]:SYS_BUS_0__SYSREG__CFG_TZPROT,	[25]: SYS_BUS_0__SYSREG__CFG_TZPROT,	[26]: RSVD,				[27]: RSVD				*/
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,		/* [28]:RSVD,				[29]: RSVD, 				[30]: RSVD,				[31]: RSVD				*/
	}
};

union tzinfo_reg cpu_busctrl[2] = {
	/* cpu_busctrl[0] -> BUSCTRL0 */
	[0].bit = {
		EVERY_ONE  , EVERY_ONE  , SECURE_ONLY, SECURE_ONLY,		/* [00]:AXIM_CPU_0,	[01]: PLL_CPU_0,	[02]: SYSREG_CPU_0__SYSREG__SECURE, [03]: SYSREG_CPU_0 	*/
		EVERY_ONE  , EVERY_ONE  , RESERVED   , RESERVED   ,		/* [04]:CMU_CPU_0,	[05]: CPUPMU_0,		[06:31]: RSVD 										*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	},
	/* cpu_busctrl[1] -> BUSCTRL1 */
	[1].bit = {
		EVERY_ONE  , RESERVED	, RESERVED   , RESERVED   ,		/* [00]:AXIM_CPU_0,	[01]: RSVD,	[02]: RSVD,	[03:31]: RSVD */
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	}

};

union tzinfo_reg usb_cfg_bus_secure[2] = {
	/* usb_cfg_bus_secure[0] -> BUS_SECURE0 */
	[0].bit = {
		EVERY_ONE  , EVERY_ONE  , EVERY_ONE  , EVERY_ONE  ,		/* [00]:AXIM_BLK_USB_DATA_BUS_0,	[01]: BLK_USB_DATA_BUS_0,		[02]: USB20HOST_0__SYSREG__OHCI_CFG,	[03]: USB20HOST_0__SYSREG__EHCI_CFG	*/
		EVERY_ONE  , EVERY_ONE  , EVERY_ONE  , EVERY_ONE  ,		/* [04]:USB20OTG_0__SYSREG__CFG,	[05]: SYSREG_USB_0__SYSREG__SECURE,	[06]: SYSREG_USB_0,			[07]: CMU_USB_0				*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,		/* [08:31]: RSVD */
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,

	},
	/* usb_cfg_bus_secure[1] -> BUS_SECURE1 */
	[1].bit = {
		EVERY_ONE  , EVERY_ONE  , RESERVED   , RESERVED   ,		/* [00]:AXIM_BLK_USB_DATA_BUS_0,	[01]: BLK_USB_DATA_BUS_0__SYSREG__TZPROT,[02:31]: RSVD */
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
		RESERVED   , RESERVED	, RESERVED   , RESERVED   ,
	}
};

union tzinfo_reg mm_bus_secure[3] = {
	/* mm_bus_secure[0] -> CFG_BUS0 */
	[0].bit = {
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [00]:CODA960_0,	[01]: LVDS_0,			[02]: DPC_0,				[03]: VIP_0 					*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [04]:DEINTERLACE_0,	[05]: GZD_0,			[06]: ROTATOR_0,			[07]: SYSREG_MM_0__SYSREG__SECURE		*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [08]:SYSREG_MM_0,	[09]: CMU_MM_0,			[10]: BLK_MM_DATA_BUS_0__SYSREG__GPV,	[11]: AXIM_ROTATOR_0				*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [12]:AXIM_G2D_0,	[13]: AXIM_DEINTERLACE_0,	[14]: AXIM_DPC_0,			[15]: AXIM_VIP_0				*/
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [16]:AXIM_CODA960_0,	[17]: BLK_MM_CFG_BUS_0__SYSREG__G2D, [18]: BLK_MM_CFG_BUS_0__SYSREG__G2D,[19]: BLK_MM_DATA_BUS_0__SYSREG__PL301		*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,		/* [20:31]:RSVD */
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	},
	/* mm_bus_secure[1] -> CFG_BUS_SECURE */
	[1].bit = {
		SECURE_ONLY, SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [00]:AXIM_CODA960_0,	[01]: AXIM_ROTATOR_0,		[02]: AXIM_G2D_0,			[03]: AXIM_DEINTERLACE_0	*/
		SECURE_ONLY, SECURE_ONLY, RESERVED   , RESERVED   ,		/* [04]:AXIM_DPC_0,	[05]: AXIM_VIP_0,		[06]: RSVD,				[07]: RSVD			*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,		/* [08:31]: RSVD */
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	},
	/* mm_bus_secure[1] -> DATA_BUS */
	[2].bit = {
		SECURE_ONLY, SECURE_ONLY, RESERVED   , RESERVED   ,		/* [00]:BLK_MM_DATA_BUS_0__SYSREG__PL301_0,		[01:31]: RSVD */
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	}
};

union tzinfo_reg hsif_bus_secure[2] = {
	/* hsif_bus_secure[0] -> BUSCTRL0 */
	[0].bit = {
		SECURE_ONLY, SECURE_ONLY, EVERY_ONE  , EVERY_ONE  ,		/* [00]:GMAC_RMII_0,		[01]: GMAC_RGMII_0,			[02]: SDMMC_2,		[03]: SDMMC_1 		*/
		EVERY_ONE  , SECURE_ONLY, SECURE_ONLY, SECURE_ONLY,		/* [04]:SDMMC_0,		[05]: SYSREG_HSIF_0__SYSREG__SECURE,	[06]: SYSREG_HSIF_0,	[07]: RSVD,		*/
		EVERY_ONE  , SECURE_ONLY, EVERY_ONE  , EVERY_ONE  ,		/* [08]:NANDC_0,		[09]: AXIM_GMAC_RMII_0,			[10]: AXIM_GMAC_RGMII_0,[11]: AXIM_NANDC_0	*/
		SECURE_ONLY, EVERY_ONE  , EVERY_ONE  , EVERY_ONE  ,		/* [12]:BLK_HSIF_DATA_BUS_0,	[13]: BLK_HSIF_DATA_BUS_0__SYSREG__PL301_0, [14]: AXIM_SDMMC_2,	[15]: AXIM_SDMMC_1	*/
		EVERY_ONE  , RESERVED   , RESERVED   , RESERVED   ,		/* [16]:AXIM_SDMMC_0,	    	[17:31]: RSVD										*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	},
	/* hsif_bus_secure[1] -> BUSCTRL1 */
	[1].bit = {
		EVERY_ONE  , EVERY_ONE  , EVERY_ONE  , EVERY_ONE  ,		/* [00]:AXIM_GMAC_RMII_0,	[01]: AXIM_GMAC_RGMII_0,		[02]: AXIM_NANDC_0,	[03]: AXIM_SDMMC_2	*/
		EVERY_ONE  , EVERY_ONE  , RESERVED   , RESERVED   ,		/* [04]: AXIM_SDMMC_1,		[05]: AXIM_SDMMC_0			[06:31]: Reserved				*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	}
};

union tzinfo_reg ddr_bus_secure[10] = {
	[0].bit = {
	},
	[1].bit = {
	},
	[2].bit = {
	},
	[3].bit = {
	},
	[4].bit = {
	},
	[5].bit = {
		EVERY_ONE  , EVERY_ONE  , EVERY_ONE  , EVERY_ONE  ,		/* [00]: TZASC_DDR_0__SYS		[01]: DDR_0__SYSREG__,		[02]: PLL_DDR_0__SYSREG,	[03]: PLL_DDR_1__SYSREG_	*/
		EVERY_ONE  , EVERY_ONE  , EVERY_ONE  , RESERVED   ,		/* [04]: SYSREG_DDR_0__SYSREG__SECURE_,	[05]: SYSREG_DDR_0__SYSREG,	[06]: CMU_DDR_0__SYSREG		[07:31]: Reserved		*/
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
		RESERVED   , RESERVED   , RESERVED   , RESERVED   ,
	},
	[6].bit = {
	},
	[7].bit = {
	},
	[8].bit = {
	}
};

static void set_tzpc(void)
{
	volatile unsigned char *base;

	base = (volatile unsigned char *)PHY_BASEADDR_SYSREG_SYS;
	mmio_write_32(&base[0x200], sys_buscfg[0].value);
	mmio_write_32(&base[0x204], sys_buscfg[1].value);
	mmio_write_32(&base[0x208], sys_buscfg[2].value);
	mmio_write_32(&base[0x20C], sys_buscfg[3].value);
//	mmio_write_32(&base[0x220], sys_buscfg[4].value);
//	mmio_write_32(&base[0x8000], SYS_REGSECURE0);
//	mmio_write_32(&base[0x8004], SYS_REGSECURE1);
//	mmio_write_32(&base[0x8008], SYS_REGSECURE2);

	base = (volatile unsigned char *)PHY_BASEADDR_SYSREG_CPU;
	mmio_write_32(&base[0x04C], cpu_busctrl[0].value);
	mmio_write_32(&base[0x050], cpu_busctrl[1].value);
//	mmio_write_32(&base[0x8000], CPU_REGSECURE0);
#if 1
	base = (volatile unsigned char *)PHY_BASEADDR_SYSREG_USB;
	mmio_write_32(&base[0x100], usb_cfg_bus_secure[0].value);
	mmio_write_32(&base[0x104], usb_cfg_bus_secure[1].value);
//	mmio_write_32(&base[0x8000], USB_REGSECURE0);
//	mmio_write_32(&base[0x8004], USB_REGSECURE1);

	base = (volatile unsigned char *)PHY_BASEADDR_SYSREG_MM;
//	mmio_write_32(&base[0x040], mm_bus_secure[0].value);
//	mmio_write_32(&base[0x050], mm_bus_secure[1].value);
//	mmio_write_32(&base[0x060], mm_bus_secure[2].value);
//	mmio_write_32(&base[0x8000], MM_REGSECURE0);
//	mmio_write_32(&base[0x8004], MM_REGSECURE1);

	base = (volatile unsigned char *)PHY_BASEADDR_SYSREG_HSIF;
//	mmio_write_32(&base[0x020], hsif_bus_secure[0].value);
//	mmio_write_32(&base[0x030], hsif_bus_secure[1].value);
//	mmio_write_32(&base[0x8000], HSIF_REGSECURE0);
#endif
	base = (volatile unsigned char *)PHY_BASEADDR_SYSREG_HSIF;
	mmio_write_32(&base[0x020], hsif_bus_secure[0].value);
	mmio_write_32(&base[0x030], hsif_bus_secure[1].value);
}


static void set_tzma(void)
{
	struct nx_sysreg_sys_reg *sys
		= ((struct nx_sysreg_sys_reg *)PHY_BASEADDR_SYSREG_SYS);

	mmio_write_32(&sys->axisram, BL1_SRAM_SIZE);
}

static void set_tzasc(void)
{
	struct tzc400_reg *tzc400
		= ((struct tzc400_reg *)PHY_BASEADDR_TZC400);
	unsigned int index = 0;

	mmio_write_32(&tzc400->gate_keepper, 0x00000001);
	mmio_write_32(&tzc400->region[0].attribute, 0xC0000000);
	mmio_write_32(&tzc400->region[0].id_access, 0x00010001);

	for (index = 1; index < 9; index++) {
		mmio_write_32(&tzc400->region[index].base_low,  0x00000000);
		mmio_write_32(&tzc400->region[index].base_high, 0x00000000);
		mmio_write_32(&tzc400->region[index].top_low,   0x00000000);
		mmio_write_32(&tzc400->region[index].top_high,  0x00000000);
		mmio_write_32(&tzc400->region[index].attribute, 0x00000000);
		mmio_write_32(&tzc400->region[index].id_access, 0x00000000);
	}

}

void tzpc_set_ddr(void)
{
	volatile unsigned char *base;
	base = (volatile unsigned char *)PHY_BASEADDR_SYSREG_DDR;
	mmio_write_32(&base[0x024], ddr_bus_secure[5].value);
//	mmio_set_32(&base[0x030], 0xFF1);
	set_tzasc();
}


void tzasc_set_regionx(unsigned int index, unsigned int base, unsigned int size,
	unsigned int secure)
{
	struct tzc400_reg *tzc400
		= ((struct tzc400_reg *)PHY_BASEADDR_TZC400);
	int align = (1 << 12);

	/* tzc400 must be 4KB aligned. */
	size = (size + (align - 1)) / align;
	size *= align;

	if (secure > 0)
		mmio_write_32(&tzc400->region[index].attribute, (0xC << 28));
	else
		mmio_write_32(&tzc400->region[index].id_access,
			((1 << 16) | (1 << 0)));

	mmio_write_32(&tzc400->region[index].base_low,  base);
	mmio_write_32(&tzc400->region[index].top_low,   (base + size));
}

void tz_initialize(void)
{
	set_tzpc();
//	set_tzasc();
	set_tzma();
}
