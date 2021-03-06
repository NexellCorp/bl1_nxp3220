/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __SMCC_HELPERS_H__
#define __SMCC_HELPERS_H__

/*******************************************************************************
 * Bit definitions inside the function id as per the SMC calling convention
 ******************************************************************************/
#define FUNCID_TYPE_SHIFT		31
#define FUNCID_CC_SHIFT			30
#define FUNCID_OEN_SHIFT		24
#define FUNCID_NUM_SHIFT		0

#define FUNCID_TYPE_MASK		0x1
#define FUNCID_CC_MASK			0x1
#define FUNCID_OEN_MASK			0x3f
#define FUNCID_NUM_MASK			0xffff

#define FUNCID_TYPE_WIDTH		1
#define FUNCID_CC_WIDTH			1
#define FUNCID_OEN_WIDTH		6
#define FUNCID_NUM_WIDTH		16

#define GET_SMC_CC(id)			((id >> FUNCID_CC_SHIFT) & \
					 FUNCID_CC_MASK)
#define GET_SMC_TYPE(id)		((id >> FUNCID_TYPE_SHIFT) & \
					 FUNCID_TYPE_MASK)

#define SMC_64				1
#define SMC_32				0
#define SMC_UNK				0xffffffff
#define SMC_TYPE_FAST			1
#define SMC_TYPE_STD			0
#define SMC_PREEMPTED			0xfffffffe
/*******************************************************************************
 * Owning entity number definitions inside the function id as per the SMC
 * calling convention
 ******************************************************************************/
#define OEN_ARM_START			0
#define OEN_ARM_END			0
#define OEN_CPU_START			1
#define OEN_CPU_END			1
#define OEN_SIP_START			2
#define OEN_SIP_END			2
#define OEN_OEM_START			3
#define OEN_OEM_END			3
#define OEN_STD_START			4	/* Standard Calls */
#define OEN_STD_END			4
#define OEN_TAP_START			48	/* Trusted Applications */
#define OEN_TAP_END			49
#define OEN_TOS_START			50	/* Trusted OS */
#define OEN_TOS_END			63
#define OEN_LIMIT			64

#endif /* __SMCC_HELPERS_H__ */
