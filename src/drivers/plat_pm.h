/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
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
void suspend_mark_clear(void);
void system_suspend(unsigned int entry_point);
 int system_resume(unsigned int *is_resume, unsigned int is_secure_os,
	 unsigned int *s_launch, unsigned int *ns_launch);
void system_power_off(void);

#endif	/* __PLAT_PM_H__ */
