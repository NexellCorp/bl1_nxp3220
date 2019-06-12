/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __ARCH_H__
#define __ARCH_H__

/*******************************************************************************
 * MIDR bit definitions
 ******************************************************************************/
#define MIDR_IMPL_MASK		0xff
#define MIDR_IMPL_SHIFT		0x18
#define MIDR_VAR_SHIFT		20
#define MIDR_VAR_BITS		4
#define MIDR_REV_SHIFT		0
#define MIDR_REV_BITS		4
#define MIDR_PN_MASK		0xfff
#define MIDR_PN_SHIFT		0x4

/*******************************************************************************
 * MPIDR macros
 ******************************************************************************/
#define MPIDR_CPU_MASK		MPIDR_AFFLVL_MASK
#define MPIDR_CLUSTER_MASK	MPIDR_AFFLVL_MASK << MPIDR_AFFINITY_BITS
#define MPIDR_AFFINITY_BITS	8
#define MPIDR_AFFLVL_MASK	0xff
#define MPIDR_AFF0_SHIFT	0
#define MPIDR_AFF1_SHIFT	8
#define MPIDR_AFF2_SHIFT	16
#define MPIDR_AFF3_SHIFT	32
#define MPIDR_AFFINITY_MASK	0xff00ffffff
#define MPIDR_AFFLVL_SHIFT	3
#define MPIDR_AFFLVL0		0
#define MPIDR_AFFLVL1		1
#define MPIDR_AFFLVL2		2
#define MPIDR_AFFLVL3		3
#define MPIDR_AFFLVL0_VAL(mpidr) \
		((mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK)
#define MPIDR_AFFLVL1_VAL(mpidr) \
		((mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK)
#define MPIDR_AFFLVL2_VAL(mpidr) \
		((mpidr >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK)
#define MPIDR_AFFLVL3_VAL(mpidr) \
		((mpidr >> MPIDR_AFF3_SHIFT) & MPIDR_AFFLVL_MASK)
/*
 * The MPIDR_MAX_AFFLVL count starts from 0. Take care to
 * add one while using this macro to define array sizes.
 * TODO: Support only the first 3 affinity levels for now.
 */
#define MPIDR_MAX_AFFLVL	2

/* Constant to highlight the assumption that MPIDR allocation starts from 0 */
#define FIRST_MPIDR		0

#endif /* __ARCH_H__ */
