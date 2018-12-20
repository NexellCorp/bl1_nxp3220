/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __NAND_H__
#define __NAND_H__

enum rstate {
	NAND_PAGE_READ		= 0,
	NAND_SECTOR_READ	= 1,
	BUF_COPY		= 2
};

enum bch_mode {
	NX_NANDC_BCH_512_24	= 4,
	NX_NANDC_BCH_1024_60	= 10
};

struct nx_nandc_reg {
	volatile unsigned int ctrl;		///< 0x000
	volatile unsigned int bch_mode;		///< 0x004
	volatile unsigned int dma_ctrl;		///< 0x008
	volatile unsigned int dma_addr;		///< 0x00C
	volatile unsigned int dma_size;		///< 0x010
	volatile unsigned int dma_subp;		///< 0x014
	volatile unsigned int ddr_ctrl;		///< 0x018
	volatile unsigned int cmd_time;		///< 0x01C
	volatile unsigned int dat_time;		///< 0x020
	volatile unsigned int ddr_time;		///< 0x024
	volatile unsigned int rand;		///< 0x028
	volatile unsigned int status;		///< 0x02C
	volatile unsigned int reserved0[14];	///< 0x030 ~ 0x064
	volatile unsigned int sram_subpagE;	///< 0x068
	volatile unsigned int err_info;		///< 0x06C
	volatile unsigned int cmd;		///< 0x070
	volatile unsigned int adr;		///< 0x074
	volatile unsigned int dat;		///< 0x078
	volatile unsigned int dat_bypass;	///< 0x07C
	volatile unsigned int sram_cfg;		///< 0x080
	volatile unsigned int reserved1[31];	///< 0x084 ~ 0x0FC
	volatile unsigned int sram[0x10];	///< 0x100
};

#endif /* #ifndef __NAND_H__ */
