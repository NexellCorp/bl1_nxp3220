/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#include <libnx.h>

void *memcpy(void *dest, const void *src, int n)
{
	const char *s = src;
	char *d = dest;

	while (n--)
		*d++ = *s++;

	return dest;
}

void *memset(void *str, int c, int n)
{
	char *pdata = str;
	while (n--)
		*pdata++ = c;
	return str;
}

int memcmp(const void* s1, const void* s2, int n)
{
	const char *src1 = s1, *src2 = s2;

	while (n--) {
		char res = *src1++ - *src2++;
		if (res)
			return (res);
	}
	return 0;
}

unsigned long abs(int n)
{
	if (n < 0)
		n =- n;

	return n;
}

int round_up(double x)
{
	if (x >= 0)
		return (int) (x + 0.5);
	return (int) (x - 0.5);
}