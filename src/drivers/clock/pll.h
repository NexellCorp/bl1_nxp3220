/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __PLL_H__
#define __PLL_H__

/* PLL(Phase Locked Loop) Register Map */
struct nx_pll_reg {
	volatile unsigned int pll_ctrl;						/* 0x00: PLL Control 0		*/
	volatile unsigned int pll_dbg0;						/* 0x04: PLL Debug 0		*/
	volatile unsigned int rsv0[2];						/* 0x08 ~ 0x0C: Reserved 0	*/
	volatile unsigned int pll_cnt0;						/* 0x10: PLL Count 0		*/
	volatile unsigned int pll_cnt1;						/* 0x14: PLL Count 1		*/
	volatile unsigned int pll_cnt2;						/* 0x18: PLL Count 2		*/
	volatile unsigned int pll_cnt3;						/* 0x1C: PLL Count 3		*/

	volatile unsigned int pll_cfg0;						/* 0x20: PLL Config 0		*/
	volatile unsigned int rsv1[3];						/* 0x24 ~ 0x2C: Reserved 1	*/
	volatile unsigned int pll_cfg1;						/* 0x30: PLL Config 1		*/
	volatile unsigned int rsv2[3];						/* 0x34 ~ 0x3C: Reserved 2	*/
	volatile unsigned int pll_cfg2;						/* 0x40: PLL Config 2		*/
	volatile unsigned int rsv3[3];						/* 0x44 ~ 0x4C: Reserved 3	*/
	volatile unsigned int pll_ctrl1;					/* 0x50: PLL Control 3		*/
	volatile unsigned int rsv4[3];						/* 0x54 ~ 0x5C: Reserved 4	*/
	volatile unsigned int pll_lockinfo;					/* 0x60: PLL Lock Infomration	*/
};

/* for PLL Control 0 Register */
enum oscmux {
	PLL_MUX_OSCCLK,
	PLL_MUX_PLL_FOUT,
};

enum nx_pll {
	PLL0,
	PLL1,
	PLLCPU,
	PLLDDR0,
	PLLDDR1,
};

#define PLL_LOCK_SHIFT		6
#define PLL_CURST_SHFIT		8
#define PLL_MUXSEL_SHIFT	3

#define PLL_LOCK_COUNT		0x200

#define	PLL_DIRTYFLAG		(1 << 1)
#define PLL_RUN_CHANGE		(1 << 0)					/* PLL Run change start. (Write Only) */
#define LINK_RESETB		(1 << 0)
#define LINK_BYPASS		(1 << 1)
#define UPDATE_CONFIG_DIRECTLY	(1 << 15)

/* Function Define */
int clock_is_stable(int num);
int get_pll_lock(int num);
int get_pll_freq(int num);
int get_pre_pll_freq(unsigned int num, unsigned int pm,
			unsigned int sk);
int nx_change_pll(int index, int pm, int sk, int sscg);
int pll_initialize(int freq);

#endif // #ifndef __PLL_H__
