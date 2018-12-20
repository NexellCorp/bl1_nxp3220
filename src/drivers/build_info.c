/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#include <type.h>
#include <log.h>

void build_information(void)
{
	SYSMSG("\n\tBL1 by Nexell Co. : Built on %s %s \r\n\n", __DATE__, __TIME__);
}

