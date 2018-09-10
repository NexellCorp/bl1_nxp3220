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
#ifndef __PMU_H__
#define __PMU_H__

typedef enum
{
	SWITCH_ISOLATE			= 0,					/// Power on reset
	SWITCH_SCPRE			= 1,					/// GPIO reset
	SWITCH_SCALL			= 2,					/// Watchdog Reset
	SWITCH_BLK_PLL_RESET_N		= 3,
	SWITCH_BLK_CMU_RESET_N		= 4,
	SWITCH_BLK_RSTCON_RESET_N 	= 5
} pmu_power_switch;

enum {
	BLOCK_DDR	= 0,
	BLOCK_MM	= 1,
	BLOCK_USB	= 2
};

/* PMU(Power Management Unit) Register Map */
struct	nx_pmu_reg {
	volatile unsigned int nisolate;						// 0x00 : clock mode register 0
	volatile unsigned int scpre;						// 0x04 : clock mode register 1
	volatile unsigned int scall;						// 0x08 : pll setting register
	volatile unsigned int scallack;						// 0x0C : divider setting register

	volatile unsigned int alive_nisolate;					// 0x10 :
	volatile unsigned int alive_scpre;					// 0x14 :
	volatile unsigned int alive_scall;					// 0x18 :
	volatile unsigned int alive_scallout;					// 0x1C :

	volatile unsigned int alive_reset;					// 0x20 :
	volatile unsigned int blk_pll_reset_n;					// 0x24 : reset the pll-block
	volatile unsigned int blk_cmu_reset_n;					// 0x28 : reset the cmu-block
	volatile unsigned int blk_rstcon_reset_n;				// 0x2C : reset the rstcon-block
};

/* Function Define */
void pmu_blk_pwrup(unsigned int domain, unsigned int enable);
void usb_blk_pwrup(void);

#endif /*__PMU_H__ */
