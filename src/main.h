/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
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
