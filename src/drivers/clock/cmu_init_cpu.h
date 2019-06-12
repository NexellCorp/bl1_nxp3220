/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __CMU_INIT_CPU_H__
#define __CMU_INIT_CPU_H__

struct nx_clk_priv cmu_cpu[] = {
	CMU_INIT(CPU_0_ARM_CLK,				0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 0, -1, 0),
	CMU_INIT(BLK_CPU_BIST_CLK_0_800MHZ_CLK,		0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 1, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_AXI_CLK,				0, CMU_TYPE_SUBDIV0,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 2, CPU_0_ARM_CLK, 0),
	CMU_INIT(AXIM_CPU_0_AXI_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 3, CPU_0_AXI_CLK, 0),
	CMU_INIT(CPU_0_ATCLK_CLK,			0, CMU_TYPE_SUBDIV1,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 4, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_CNTCLK_CLK,			0, CMU_TYPE_SUBDIV2,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 5, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_TSCLK_CLK,			0, CMU_TYPE_SUBDIV3,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 6, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_DBGAPB_CLK,			0, CMU_TYPE_SUBDIV4,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 7, CPU_0_ARM_CLK, 0),
	CMU_INIT(CPU_0_APB_CLK,				0, CMU_TYPE_SUBDIV5,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 8, CPU_0_ARM_CLK, 0),
	CMU_INIT(SYSREG_CPU_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 9, CPU_0_APB_CLK, 0),
	CMU_INIT(AXIM_CPU_0_APB_CLK,			0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x200), 10, CPU_0_APB_CLK, 0),
	CMU_INIT(PLL_CPU_DIV_0_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x400), 0, -1, 1),
	CMU_INIT(HPM_CPU_0_HPM_CLK,			0, CMU_TYPE_MAINDIV,  (void*)(PHY_BASEADDR_CMU_CPU_MODULE + 0x600), 0, -1, 2),

};

#endif /* #ifndef __CMU_INIT_CPU_H__ */
