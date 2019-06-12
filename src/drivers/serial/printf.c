/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#include <sysheader.h>
#include <stdarg.h>
#include <bl0_fnptr.h>
#include <printf.h>

void printchar(char **str, int c)
{
	extern void putc(char ch);

	if (str) {
		**str = c;
		++(*str);
	} else
		(void)putc((char)c);
}

#if 0
int print(char **out, const char *format, va_list args)
{
	return g_bl1_fn->lib_fn.print(out, format, args);
}

int printf(const char *format, ...)
{
	va_list args;
	int ret;

	va_start(args, format);
	ret = print(0, format, args);
	va_end(args);

	return ret;
}
#endif

int empty_printf(const char *format, ...)
{
	format = format;
	return 0;
}
