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
#ifndef __SDMMC_H__
#define __SDMMC_H__

typedef enum
{
	SDMMC_CLOCK_SHIFT_0	= 0,
	SDMMC_CLOCK_SHIFT_90	= 1,
	SDMMC_CLOCK_SHIFT_180	= 2,
	SDMMC_CLOCK_SHIFT_270	= 3
} sdmmc_clkshift;

struct  nx_sdmmc_reg {
	// Internal Register
	volatile unsigned int	ctrl;			/* 0x000 : Control Register					*/
	volatile unsigned int	pwren;			/* 0x004 : Power Enable Register				*/
	volatile unsigned int 	clkdiv;			/* 0x008 : Clock Divider Register				*/
	volatile unsigned int	clksrc;			/* 0x00C : Clock Source Register				*/
	volatile unsigned int	clkena;			/* 0x010 : Clock Enable Register				*/
	volatile unsigned int	tmout;			/* 0x014 : Time-Out Register					*/
	volatile unsigned int	ctype;			/* 0x018 : Card Type Register					*/
	volatile unsigned int	blksiz;			/* 0x01C : Block Size Register					*/
	volatile unsigned int	bytcnt;			/* 0x020 : Byte Count Register					*/
	volatile unsigned int	intmask;		/* 0x024 : Interrupt Mask Register				*/
	volatile unsigned int	cmdarg;			/* 0x028 : Command Argument Register				*/
	volatile unsigned int	cmd;			/* 0x02C : Command Register					*/
	volatile unsigned int	resp[4];		/* 0x030 ~ 0x3C : Response 0 ~ 3 Register			*/
	volatile unsigned int	mintsts;		/* 0x040 : Masked Interrupt Status */
	volatile unsigned int	rintsts;		/* 0x044 : Raw Interrupt Status Register			*/
	volatile unsigned int	status;			/* 0x048 : Status Register - Mainly for Debug Purpose		*/
	volatile unsigned int	fifoth;			/* 0x04C : FIFO Threshold Register				*/
	volatile unsigned int	cdetect;		/* 0x050 : Card Detect Register					*/
	volatile unsigned int	wrtprt;			/* 0x054 : Write Protect Register				*/
	volatile unsigned int	gpio;			/* 0x058 : General Purpose Input/Output Register		*/
	volatile unsigned int	tcbcnt;			/* 0x05C : Transferred CIU card byte count			*/
	volatile unsigned int	tbbcnt;			/* 0x060 : Transferred Host Byte Count				*/
	volatile unsigned int	debnce;			/* 0x064 : Card Detect Debounce Register			*/
	volatile unsigned int	usrid;			/* 0x068 : User ID Register					*/
	volatile unsigned int	verid;			/* 0x06C : Version ID Register					*/
	volatile unsigned int	hcon;			/* 0x070 : Hardware Configuration Register			*/
	volatile unsigned int	uhs_reg;		/* 0x074 : UHS_REG register					*/
	volatile unsigned int	rstn;			/* 0x078 : Hardware reset register				*/
	volatile unsigned int	_reserved0;		/* 0x07C							*/
	volatile unsigned int	bmod;			/* 0x080 : Bus Mode Register					*/
	volatile unsigned int	pldmnd;			/* 0x084 : Poll Demand Register					*/
	volatile unsigned int	dbaddr;			/* 0x088 : Descriptor List Base Address Register		*/
	volatile unsigned int	idsts;			/* 0x08C : Internal DMAC Status Register			*/
	volatile unsigned int	idinten;		/* 0x090 : Internal DMAC Interrupt Enable Register		*/
	volatile unsigned int	dscaddr;		/* 0x094 : Current Host Descriptor Address Register		*/
	volatile unsigned int	bufaddr;   		/* 0x098 : Current Buffer Descriptor Address Register		*/
	volatile unsigned char	_reserved1[0x100-0x09c];/* 0x09C ~ 0x100 reserved area					*/
	volatile unsigned int	cardthrctl;		/* 0x100 : Card Read Threshold Enable				*/
	volatile unsigned int	backend_power;		/* 0x104 : Back-end Power					*/
	volatile unsigned int	uhs_reg_ext;		/* 0x108 : eMMC 4.5 1.2V Register				*/
	volatile unsigned int	emmc_ddr_reg;		/* 0x10C : eMMC DDR START bit detection control register	*/
	volatile unsigned int	enable_shift;		/* 0x110 : Phase shift control register				*/
	volatile unsigned int	clkctrl;		/* 0x114 : External clock phase & delay control register	*/
	volatile unsigned char	_reserved2[0x200-0x118];/* 0x118 ~ 0x200						*/
	volatile unsigned int	data;			/* 0x200 : Data							*/
	volatile unsigned char	_reserved3[0x400-0x204];/* 0x204 ~ 0x400 : Reserved 3 Register 				*/
	volatile unsigned int	tiemode;		/* 0x400							*/
	volatile unsigned int	tiesram;		/* 0x404							*/
	volatile unsigned int	tiedrvphase;		/* 0x408							*/
	volatile unsigned int	tiesmpphase;		/* 0x40C							*/
	volatile unsigned int	tiedsdelay;		/* 0x410							*/
};

#endif /* #ifndef __SDMMC_H__ */
