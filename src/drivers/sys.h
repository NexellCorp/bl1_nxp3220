/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __SYS_H__
#define __SYS_H__

void vddpwron_ddr_on(void);
void wdt_reset_enb(void);

void system_initialize(void);

#endif /* #ifndef __SYS_H__ */
