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
#ifndef __PLAT_LOAD_H__
#define __PLAT_LOAD_H__

struct platform_info {
	/* @brief: boot loader level x information */
	unsigned int is_loadmark;
	unsigned int s_dev_addr;
	unsigned int n_dev_addr;
	unsigned int sf_dev_addr;
	/* @brief: Memory Function */
	void (*ensr_func)(void);
	void (*exsr_func)(void);
	/* Suspend/Resume Function */
	void (*pmic_poweroff)(void);
};

struct platform_manager {
	struct platform_info pi;
	/* @brief: save the subcpu wake mark (for debug) */
	unsigned int wakeup_mark;
	unsigned int s_launch_addr;
};

enum {
	SSS_F_LOAD_FAILED = 1,
	BL31_LOAD_FAILED  = 2,
	BL32_LOAD_FAILED  = 3,
	BL33_LOAD_FAILED  = 4
};

/* Function Define */
 int plat_next_load(struct nx_bootmanager *pbm, unsigned int option);
void plat_load(int is_resume, struct nx_bootmanager *pbm);
 int plat_s_load(struct platform_info *ppi);

#endif /* #ifndef __PLAT_LOAD_H__ */
