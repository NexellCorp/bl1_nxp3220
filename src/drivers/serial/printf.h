/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __PRINTF__
#define __PRINTF__

#include <stdarg.h>
#include <serial.h>
#include <bl0_fnptr.h>

#define getc		serial_getc
#define putc		serial_putc

#define serial_done	serial_is_uart_tx_done
#define serial_busy	serial_is_busy
#define serial_tx_empty	serial_is_tx_empty

#define printf g_bl1_fn->lib_fn.printf

void printchar(char **str, int c);
//int print(char **out, const char *format, va_list args);
//int printf(const char *format, ...);
int empty_printf(const char *format, ...);

#endif /* #ifndef __PRINTF__*/
