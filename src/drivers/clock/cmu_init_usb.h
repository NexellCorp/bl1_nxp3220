/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __CMU_INIT_USB_H__
#define __CMU_INIT_USB_H__

struct nx_clk_priv cmu_usb[] = {
	CMU_INIT(USB_0_AHB_CLK                     , 0, CMU_TYPE_MAINDIV , (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 0, SRC_USB_0_AHB_CLK, 0),
	CMU_INIT(SYSREG_USB_0_APB_CLK              , 0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 1, USB_0_AHB_CLK, 0),
	CMU_INIT(USB20OTG_0_AHB_CLK                , 0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 2, USB_0_AHB_CLK, 0),
	CMU_INIT(USB20HOST_0_AHB_CLK               , 0, CMU_TYPE_ONLYGATE, (void*)(PHY_BASEADDR_CMU_USB_MODULE + 0x200), 3, USB_0_AHB_CLK, 0),
};

#endif /* #ifndef __CMU_INIT_USB_H__ */
