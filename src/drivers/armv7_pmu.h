/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __ARMV7_PMU_H__
#define __ARMV7_PMU_H__

/* Function Define */
void armv7_pmu_init(void);
void pmu_delay_us(volatile unsigned int us);

#endif	/* __ARMV7_PMU_H__ */
