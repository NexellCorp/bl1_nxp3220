/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __LIBARMV7_H__
#define __LIBARMV7_H__

void secure_launch(unsigned int is_resume, unsigned int secure_os_addr,
		unsigned int non_secure_addr, unsigned int bootarg);
void non_secure_launch(unsigned int is_resume, unsigned int n_secure);

/* "libarmv7.S" armv7 function */
void set_nonsecure_mode(void);
void set_secure_mode(void);

 int get_secure_status(void);

 int armv7_get_scr(void);
void armv7_set_scr(int reg);

void set_svc_mode(void);

 int armv7_get_auxctrl(void);
void armv7_set_auxctrl(int value);

 int armv7_get_cpuid(void);

/*"libplat.S" armv7 user-function */
 int get_fntpr(void);
void set_fnptr(int);

 int get_boption(void);
void set_boption(int);

 int get_cpuid(void);

void disable_mmu(void);

#endif	// __LIBARMV7_H__
