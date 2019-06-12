/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __SPIBOOT_H__
#define __SPIBOOT_H__

void spi_initialize(unsigned int channel);
int spiboot(struct nx_bootmanager *pbm, unsigned int option);

#endif
