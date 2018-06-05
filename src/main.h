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
#ifndef __MAIN_H__
#define __MAIN_H__
#define __MAIN_H__

/* External Function Define */
extern void build_information(void);

extern unsigned int bl1_smc_handler(unsigned int smc_fid,
	unsigned int r1, unsigned int r2, unsigned int r3);
extern void smc_set_fnptr(unsigned int (*smc_handler));

#endif
