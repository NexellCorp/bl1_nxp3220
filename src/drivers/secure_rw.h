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
#ifndef __SECURE_RW_H__
#define __SECURE_RW_H__

#define	RW_IDX_ALIVE_GPIO		0
#define RW_IDX_SYSREG			1

struct s_rw_filter {
	unsigned int start;
	unsigned int end;
	unsigned int mask;
};

/* define the function */
 int secure_write_32(void *addr, unsigned int value);
 int secure_read_32(void *addr);
#endif
