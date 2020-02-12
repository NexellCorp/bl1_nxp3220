/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __HPM_H__
#define __HPM_H__

#define HPM_SYNC_TO_APM (1 << 12)

struct nx_cpusys_reg {
	volatile unsigned int _rev0[21];	// 0x000
	volatile unsigned int hpm0;		// 0x054
	volatile unsigned int hpm1;		// 0x058
};

struct nx_cmusys_reg {
	volatile unsigned int _rev0[384];	// 0x000
	volatile unsigned int src_back0;		// 0x600
};

struct nx_hpm_info {
       unsigned int cpu_hpm;
       unsigned int core_hpm;
};

/* Function Define */
void read_hpm_core(struct nx_hpm_info *hpm);
void hpm_parser(struct nx_hpm_info *hpm);

#endif //__HPM_H__
