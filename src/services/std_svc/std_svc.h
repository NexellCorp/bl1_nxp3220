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
#ifndef __STD_SVC_H__
#define __STD_SVC_H__

/* SMC function IDs for Standard Service queries */
#define ARM_STD_SVC_CALL_COUNT		0x8400ff00
#define ARM_STD_SVC_UID			0x8400ff01
/*					0x8400ff02 is reserved */
#define ARM_STD_SVC_VERSION		0x8400ff03

/* ARM Standard Service Calls version numbers */
#define STD_SVC_VERSION_MAJOR		0x0
#define STD_SVC_VERSION_MINOR		0x1

/* The macros below are used to identify PSCI calls from the SMC function ID */
#define PSCI_FID_MASK			0xffe0u
#define PSCI_FID_VALUE			0u
#define is_psci_fid(_fid) \
	(((_fid) & PSCI_FID_MASK) == PSCI_FID_VALUE)

#endif /* __STD_SVC_H__ */
