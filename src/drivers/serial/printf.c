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

int empty_printf(const char *format, ...)
{
	format = format;
	return 0;
}
