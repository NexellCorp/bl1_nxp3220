/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __CMU_INIT_SYS_H__
#define __CMU_INIT_SYS_H__

struct nx_clk_priv cmu_sys[] = {
	CMU_INIT(SYS_0_AXI_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 0, SRC_SYS_0_AXI_CLK, 0),
	CMU_INIT(SYS_BUS_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 1, SYS_0_AXI_CLK, 0),
	CMU_INIT(ETC_BUS_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 2, SYS_0_AXI_CLK, 0),
	CMU_INIT(FSYS_BUS_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 3, SYS_0_AXI_CLK, 0),
	CMU_INIT(DMA_BUS_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 4, SYS_0_AXI_CLK, 0),
	CMU_INIT(CFG_BUS_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 5, SYS_0_AXI_CLK, 0),
	CMU_INIT(AXISRAM_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 6, SYS_0_AXI_CLK, 0),
	CMU_INIT(MP2TSI_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 7, SYS_0_AXI_CLK, 0),
	CMU_INIT(MP2TSI_1_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 8, SYS_0_AXI_CLK, 0),
	CMU_INIT(TOP_BIST_CLK_0_333MHZ_CLK,	0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 9, SYS_0_AXI_CLK, 0),
	CMU_INIT(DMA_0_AXI_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 10, SYS_0_AXI_CLK, 0),
	CMU_INIT(SSS_0_AXI_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 11, SYS_0_AXI_CLK, 0),
	CMU_INIT(DMA_1_AXI_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 12, SYS_0_AXI_CLK, 0),
	CMU_INIT(SDMA_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 13, SYS_0_AXI_CLK, 0),
	CMU_INIT(SDMA_1_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 14, SYS_0_AXI_CLK, 0),
	CMU_INIT(MDMA_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 15, SYS_0_AXI_CLK, 0),
	CMU_INIT(AXIM_SDMMC_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 16, SYS_0_AXI_CLK, 0),
	CMU_INIT(AXIM_SDMMC_1_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 17, SYS_0_AXI_CLK, 0),
	CMU_INIT(AXIM_SDMMC_2_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 18, SYS_0_AXI_CLK, 0),
	CMU_INIT(SYS_0_APB_CLK,			0, CMU_TYPE_SUBDIV0,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 19, SYS_0_AXI_CLK, 0),
	CMU_INIT(DPC_PADPLACE_0_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 20, SYS_0_APB_CLK, 0),
	CMU_INIT(I2S_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 21, SYS_0_APB_CLK, 0),
	CMU_INIT(I2S_1_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 22, SYS_0_APB_CLK, 0),
	CMU_INIT(I2S_2_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 23, SYS_0_APB_CLK, 0),
	CMU_INIT(I2S_3_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 24, SYS_0_APB_CLK, 0),
	CMU_INIT(MP2TSI_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 25, SYS_0_APB_CLK, 0),
	CMU_INIT(MP2TSI_1_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 26, SYS_0_APB_CLK, 0),
	CMU_INIT(WDT_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 27, SYS_0_APB_CLK, 0),
	CMU_INIT(WDT_0_POR_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 28, SYS_0_APB_CLK, 0),
	CMU_INIT(SECURE_WDT_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 29, SYS_0_APB_CLK, 0),
	CMU_INIT(SECURE_WDT_0_POR_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 30, SYS_0_APB_CLK, 0),
	CMU_INIT(SYSREG_SYS_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 31, SYS_0_APB_CLK, 0),
	CMU_INIT(ECID_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 32, SYS_0_APB_CLK, 0),
	CMU_INIT(SYSCTRLTOP_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 33, SYS_0_APB_CLK, 0),
	CMU_INIT(CAN_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 34, SYS_0_APB_CLK, 0),
	CMU_INIT(CAN_1_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 35, SYS_0_APB_CLK, 0),
	CMU_INIT(TMU_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 36, SYS_0_APB_CLK, 0),
	CMU_INIT(DMA_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 37, SYS_0_APB_CLK, 0),
	CMU_INIT(SSS_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 38, SYS_0_APB_CLK, 0),
	CMU_INIT(DMA_1_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 39, SYS_0_APB_CLK, 0),
	CMU_INIT(SMC_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 40, SYS_0_APB_CLK, 0),
	CMU_INIT(GPIO_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 41, SYS_0_APB_CLK, 0),
	CMU_INIT(SDMA_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 42, SYS_0_APB_CLK, 0),
	CMU_INIT(GPIO_1_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 43, SYS_0_APB_CLK, 0),
	CMU_INIT(SDMA_1_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 44, SYS_0_APB_CLK, 0),
	CMU_INIT(GPIO_2_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 45, SYS_0_APB_CLK, 0),
	CMU_INIT(GPIO_3_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 46, SYS_0_APB_CLK, 0),
	CMU_INIT(GPIO_4_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 47, SYS_0_APB_CLK, 0),
	CMU_INIT(ADC_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 48, SYS_0_APB_CLK, 0),
	CMU_INIT(MDMA_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 49, SYS_0_APB_CLK, 0),
	CMU_INIT(DUMMY_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 50, SYS_0_APB_CLK, 0),
	CMU_INIT(SPDIFTX_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 51, SYS_0_APB_CLK, 0),
	CMU_INIT(SPDIFRX_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 52, SYS_0_APB_CLK, 0),
	CMU_INIT(AXIM_SDMMC_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 53, SYS_0_APB_CLK, 0),
	CMU_INIT(AXIM_SDMMC_1_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 54, SYS_0_APB_CLK, 0),
	CMU_INIT(AXIM_SDMMC_2_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x200), 55, SYS_0_APB_CLK, 0),
	CMU_INIT(SYS_0_HSIF_AXI_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 0, SRC_SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(BLK_HSIF_DATA_BUS_0_AXI_CLK,	0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 1, SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(SDMMC_0_AHB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 2, SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(SDMMC_1_AHB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 3, SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(SDMMC_2_AHB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 4, SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(GMAC_RGMII_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 5, SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(GMAC_RMII_0_AXI_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 6, SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(SYS_0_HSIF_APB_CLK,		0, CMU_TYPE_SUBDIV0,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 7, SYS_0_HSIF_AXI_CLK, 1),
	CMU_INIT(BLK_HSIF_DATA_BUS_0_APB_CLK,	0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 8, SYS_0_HSIF_APB_CLK, 1),
	CMU_INIT(HSIF_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 9, SYS_0_HSIF_APB_CLK, 1),
	CMU_INIT(SYSREG_HSIF_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 10, SYS_0_HSIF_APB_CLK, 1),
	CMU_INIT(GMAC_RGMII_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 11, SYS_0_HSIF_APB_CLK, 1),
	CMU_INIT(GMAC_RMII_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 12, SYS_0_HSIF_APB_CLK, 1),
	CMU_INIT(NANDC_0_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x400), 13, SYS_0_HSIF_APB_CLK, 1),
	CMU_INIT(CPU_BACKUP_0_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x600), 0, SRC_CPU_BACKUP_0_CLK, 2),
	CMU_INIT(CSSYS_0_HCLK_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x800), 0, SRC_CSSYS_0_HCLK_CLK, 3),
	CMU_INIT(CSSYS_0_SCAN_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0xA00), 0, SRC_CSSYS_0_SCAN_CLK, 4),
	CMU_INIT(BLK_CMU_0_APB_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0xC00), 0, SRC_BLK_CMU_0_APB_CLK, 5),
	CMU_INIT(VIP_PADPLACE_0_SCAN_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0xE00), 0, SRC_VIP_PADPLACE_0_SCAN_CLK, 6),
	CMU_INIT(PADOUT_0_AXI_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1000), 0, SRC_PADOUT_0_AXI_CLK, 7),
	CMU_INIT(PADOUT_1_AXI_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1200), 0, SRC_PADOUT_1_AXI_CLK, 8),
	CMU_INIT(PADOUT_2_AXI_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1400), 0, SRC_PADOUT_2_AXI_CLK, 9),
	CMU_INIT(PADOUT_3_AXI_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1600), 0, SRC_PADOUT_3_AXI_CLK, 10),
	CMU_INIT(PADOUT_4_AXI_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1800), 0, SRC_PADOUT_4_AXI_CLK, 11),
	CMU_INIT(HPM_SYS_0_HPM_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1A00), 0, SRC_HPM_SYS_0_HPM_CLK, 12),
	CMU_INIT(UART_0_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1C00), 0, SRC_UART_0_CORE_CLK, 13),
	CMU_INIT(UART_1_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1C00), 1, UART_0_CORE_CLK, 13),
	CMU_INIT(UART_2_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1C00), 2, UART_0_CORE_CLK, 13),
	CMU_INIT(UART_3_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1C00), 3, UART_0_CORE_CLK, 13),
	CMU_INIT(UART_4_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1C00), 4, UART_0_CORE_CLK, 13),
	CMU_INIT(UART_5_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1C00), 5, UART_0_CORE_CLK, 13),
	CMU_INIT(UART_6_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1C00), 6, UART_0_CORE_CLK, 13),
	CMU_INIT(UART_0_APB_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1E00), 0, SRC_UART_0_APB_CLK, 14),
	CMU_INIT(UART_1_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1E00), 1, UART_0_APB_CLK, 14),
	CMU_INIT(UART_2_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1E00), 2, UART_0_APB_CLK, 14),
	CMU_INIT(UART_3_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1E00), 3, UART_0_APB_CLK, 14),
	CMU_INIT(UART_4_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1E00), 4, UART_0_APB_CLK, 14),
	CMU_INIT(UART_5_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1E00), 5, UART_0_APB_CLK, 14),
	CMU_INIT(UART_6_APB_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x1E00), 6, UART_0_APB_CLK, 14),
	CMU_INIT(I2S_0_SCAN_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2000), 0, SRC_I2S_0_SCAN_CLK, 15),
	CMU_INIT(I2S_1_SCAN_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2000), 1, I2S_0_SCAN_CLK, 15),
	CMU_INIT(I2S_2_SCAN_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2000), 2, I2S_0_SCAN_CLK, 15),
	CMU_INIT(I2S_3_SCAN_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2000), 3, I2S_0_SCAN_CLK, 15),
	CMU_INIT(I2S_0_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2200), 0, SRC_I2S_0_CORE_CLK, 16),
	CMU_INIT(I2S_1_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2400), 0, SRC_I2S_1_CORE_CLK, 17),
	CMU_INIT(I2S_2_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2600), 0, SRC_I2S_2_CORE_CLK, 18),
	CMU_INIT(I2S_3_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2800), 0, SRC_I2S_3_CORE_CLK, 19),
	CMU_INIT(I2C_0_APB_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2A00), 0, SRC_I2C_0_APB_CLK, 20),
	CMU_INIT(I2C_1_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2A00), 1, I2C_0_APB_CLK, 20),
	CMU_INIT(I2C_2_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2A00), 2, I2C_0_APB_CLK, 20),
	CMU_INIT(I2C_3_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2A00), 3, I2C_0_APB_CLK, 20),
	CMU_INIT(I2C_4_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2A00), 4, I2C_0_APB_CLK, 20),
	CMU_INIT(MP2TSI_0_SCAN_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2C00), 0, SRC_MP2TSI_0_SCAN_CLK, 21),
	CMU_INIT(MP2TSI_1_SCAN_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2C00), 1, MP2TSI_0_SCAN_CLK, 21),
	CMU_INIT(SDMMC_0_SCAN_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2E00), 0, SRC_SDMMC_0_SCAN_CLK, 22),
	CMU_INIT(SDMMC_1_SCAN_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2E00), 1, SDMMC_0_SCAN_CLK, 22),
	CMU_INIT(SDMMC_2_SCAN_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x2E00), 2, SDMMC_0_SCAN_CLK, 22),
	CMU_INIT(SDMMC_0_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3000), 0, SRC_SDMMC_0_CORE_CLK, 23),
	CMU_INIT(SDMMC_1_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3200), 0, SRC_SDMMC_1_CORE_CLK, 24),
	CMU_INIT(SDMMC_2_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3400), 0, SRC_SDMMC_2_CORE_CLK, 25),
	CMU_INIT(SPI_0_APB_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3600), 0, SRC_SPI_0_APB_CLK, 26),
	CMU_INIT(SPI_1_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3600), 1, SPI_0_APB_CLK, 26),
	CMU_INIT(SPI_2_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3600), 2, SPI_0_APB_CLK, 26),
	CMU_INIT(SPI_0_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3800), 0, SRC_SPI_0_CORE_CLK, 27),
	CMU_INIT(SPI_1_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3800), 1, SPI_0_CORE_CLK, 27),
	CMU_INIT(SPI_2_CORE_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3800), 2, SPI_0_CORE_CLK, 27),
	CMU_INIT(PDM_0_AXI_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3A00), 0, SRC_PDM_0_AXI_CLK, 28),
	CMU_INIT(PDM_0_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3C00), 0, SRC_PDM_0_CORE_CLK, 29),
	CMU_INIT(PWM_0_APB_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x3E00), 0, SRC_PWM_0_APB_CLK, 30),
	CMU_INIT(PWM_0_TCLK0_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4000), 0, SRC_PWM_0_TCLK0_CLK, 31),
	CMU_INIT(PWM_0_TCLK1_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4200), 0, SRC_PWM_0_TCLK1_CLK, 32),
	CMU_INIT(PWM_0_TCLK2_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4400), 0, SRC_PWM_0_TCLK2_CLK, 33),
	CMU_INIT(PWM_0_TCLK3_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4600), 0, SRC_PWM_0_TCLK3_CLK, 34),
	CMU_INIT(CAN_0_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4800), 0, SRC_CAN_0_CORE_CLK, 35),
	CMU_INIT(CAN_1_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4A00), 0, SRC_CAN_1_CORE_CLK, 36),
	CMU_INIT(TIMER_0_APB_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4C00), 0, SRC_TIMER_0_APB_CLK, 37),
	CMU_INIT(TIMER_0_TCLK0_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x4E00), 0, SRC_TIMER_0_TCLK0_CLK, 38),
	CMU_INIT(TIMER_0_TCLK1_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5000), 0, SRC_TIMER_0_TCLK1_CLK, 39),
	CMU_INIT(TIMER_0_TCLK2_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5200), 0, SRC_TIMER_0_TCLK2_CLK, 40),
	CMU_INIT(TIMER_0_TCLK3_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5400), 0, SRC_TIMER_0_TCLK3_CLK, 41),
	CMU_INIT(SECURE_TIMER_0_APB_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5600), 0, SRC_SECURE_TIMER_0_APB_CLK, 42),
	CMU_INIT(SECURE_TIMER_0_TCLK0_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5800), 0, SRC_SECURE_TIMER_0_TCLK0_CLK, 43),
	CMU_INIT(SECURE_TIMER_0_TCLK1_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5A00), 0, SRC_SECURE_TIMER_0_TCLK1_CLK, 44),
	CMU_INIT(SECURE_TIMER_0_TCLK2_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5C00), 0, SRC_SECURE_TIMER_0_TCLK2_CLK, 45),
	CMU_INIT(SECURE_TIMER_0_TCLK3_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x5E00), 0, SRC_SECURE_TIMER_0_TCLK3_CLK, 46),
	CMU_INIT(SMC_0_AXI_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6000), 0, SRC_SMC_0_AXI_CLK, 47),
	CMU_INIT(SPDIFTX_0_CORE_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6200), 0, SRC_SPDIFTX_0_CORE_CLK, 48),
	CMU_INIT(GMAC_RGMII_0_TX_CLK_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6400), 0, SRC_GMAC_RGMII_0_TX_CLK_CLK, 49),
	CMU_INIT(GMAC_RGMII_0_PTP_REF_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6600), 0, SRC_GMAC_RGMII_0_PTP_REF_CLK, 50),
	CMU_INIT(GMAC_RGMII_0_SCAN_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6800), 0, SRC_GMAC_RGMII_0_SCAN_CLK, 51),
	CMU_INIT(GMAC_RMII_0_PTP_REF_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6A00), 0, SRC_GMAC_RMII_0_PTP_REF_CLK, 52),
	CMU_INIT(GMAC_RMII_0_SCAN_RMII_CLK,	0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6C00), 0, SRC_GMAC_RMII_0_SCAN_RMII_CLK, 53),
	CMU_INIT(GMAC_RMII_0_SCAN_TRXCLK_CLK,	0, CMU_TYPE_SUBDIV0,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6C40), 1, GMAC_RMII_0_SCAN_RMII_CLK, 53),
	CMU_INIT(NANDC_0_AXI_CLK,		0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_SYS_MODULE + 0x6E00), 0, SRC_NANDC_0_AXI_CLK, 54),
};

#endif // #ifndef __CMU_INIT_SYS_H__