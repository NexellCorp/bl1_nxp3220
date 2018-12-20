/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
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
