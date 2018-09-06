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
#ifndef __PLAT_PM_H__
#define __PLAT_PM_H__

/* function define */
void smp_enable(int enable);
void dmb(void);

int check_suspend_state(void);

int system_cpu_check(unsigned int cpu_id);
int system_cpu_on(unsigned int cpu_id);
int system_cpu_off(unsigned int cpu_id);

void system_core_reset(void);
int check_suspend_hash(void);
void suspend_mark(unsigned int base,
	unsigned int size, unsigned int entry_point);
void system_suspend(unsigned int entry_point);
 int system_resume(int *is_resume, unsigned int is_secure_os,
	 unsigned int *s_launch, unsigned int *ns_launch);
void system_power_off(void);

void suspend_mark_clear(void);

#endif	/* __PLAT_PM_H__ */
