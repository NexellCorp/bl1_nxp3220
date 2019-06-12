/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __LIBNX_H__
#define __LIBNX_H__

void *memcpy(void *dest, const void *src, int n);
void *memset(void *str, int c, int n);
int memcmp(const void* s1, const void* s2, int n);

/*
 * This looks more complex than it should be. But we need to
 * get the type for the ~ right in round_down (it needs to be
 * as wide as the result!), and we want to evaluate the macro
 * arguments just once each.
 */
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up_m(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down_m(x, y) ((x) & ~__round_mask(x, y))


#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))


/* Define Function */
unsigned long abs(int n);

int round_up(double x);

#endif // ifndef __LIBNX_H__
