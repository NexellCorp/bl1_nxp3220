/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#include <arch.h>

#if 0
#include <plat_arm.h>
#include <platform_def.h>

/* The power domain tree descriptor which need to be exported by ARM platforms */
extern const unsigned char arm_power_domain_tree_desc[];


/*******************************************************************************
 * This function returns the ARM default topology tree information.
 ******************************************************************************/
const unsigned char *plat_get_power_domain_tree_desc(void)
{
	return arm_power_domain_tree_desc;
}
#endif

#define PLAT_ARM_CLUSTER1_CORE_COUNT	0
#define PLAT_ARM_CLUSTER0_CORE_COUNT	2

#define get_arm_cluster_core_count(mpidr)\
		(((mpidr) & 0x100) ? PLAT_ARM_CLUSTER1_CORE_COUNT :\
				PLAT_ARM_CLUSTER0_CORE_COUNT)

#define	ARM_CLUSTER_COUNT	1

/*******************************************************************************
 * This function validates an MPIDR by checking whether it falls within the
 * acceptable bounds. An error code (-1) is returned if an incorrect mpidr
 * is passed.
 ******************************************************************************/
int arm_check_mpidr(unsigned int mpidr)
{
	unsigned int cluster_id = 0, cpu_id;

	cluster_id = cluster_id;

	mpidr &= MPIDR_AFFINITY_MASK;

	if (mpidr & ~(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK))
		return -1;

	cluster_id = (mpidr >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK;
	cpu_id = (mpidr >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK;
#if 0
	if (cluster_id >= ARM_CLUSTER_COUNT)
		return -1;
#endif
	/* Validate cpu_id by checking whether it represents a CPU in
	   one of the two clusters present on the platform. */
	if (cpu_id >= get_arm_cluster_core_count(mpidr))
		return -1;

	return 0;
}
