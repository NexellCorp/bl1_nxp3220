/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#ifndef __SYSHEADER_H__
#define __SYSHEADER_H__

#include <type.h>
#include <chip.h>
#include <libnx.h>
#include <libarmv7.h>
#include <armv7_pmu.h>
#include <delay.h>

#include <bootheader.h>

#include <log.h>

#include <bl0_fnptr.h>

#if defined(CHIPID_NXP3220)
#include <nxp3220.h>
#include <nxp3220_io_mux.h>
#include <bootoption.h>
#endif

#ifdef SET_GLOBAL_VARIABLES

struct sbi_header *g_nsih
	= ((struct sbi_header *)HEADER_BASEADDR);
struct nx_bl0_fnptr *g_bl0_fn
	= ((struct nx_bl0_fnptr *)BL0_FUNCTION_TABLE);
struct nx_bl0_fnptr *g_bl1_fn;
struct nx_mbedtls_fnptr *g_mbedtls
	= ((struct nx_mbedtls_fnptr *)BL0_FNCT_MBEDTLS_ADDR);
struct nx_crypto_fntpr *g_crypto
	= ((struct nx_crypto_fntpr *)BL0_FNCT_MBEDTLS_ADDR);

#else

extern struct sbi_header *g_nsih;
extern struct nx_bl0_fnptr *g_bl0_fn;
extern struct nx_bl0_fnptr *g_bl1_fn;
extern struct nx_mbedtls_fnptr * g_mbedtls;
extern struct nx_crypto_fntpr *g_crypto;

#endif

#endif // #ifndef __SYSHEADER_H__
