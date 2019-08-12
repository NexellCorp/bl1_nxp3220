/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.\
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __GIC_H__
#define __GIC_H__

#define GIC_CPUIF_CTRL		0x0
#define GIC_CPUIF_PMR		0x004
#define GIC_CPUIF_IAR		0x00C
#define GIC_CPUIF_EOIR		0x10
#define GIC_CPUIF_PPR		0x14
#define GIC_CPUIF_HPPIR		0x18
#define GIC_CPUIF_AIAR		0x20
#define GIC_CPUIF_AEOIR		0x24

#define GIC_DIST_CTRL		0x0
#define GIC_DIST_GROUP		0x80
#define GIC_DIST_SENABLE	0x100	/* SGI and PPI : 0x100, SPIs : 0x104 ~ 0x13C */
#define GIC_DIST_CENABLE	0x180	/* SGI and PPI : 0x180, SPIs : 0x184 ~ 0x1BC */

#define GIC_DIST_SPEND		0x200			/* 0x200 ~ 0x23C */
#define GIC_DIST_CPEND		0x280			/* 0x280 ~ 0x2BC */
#define GIC_DIST_CACTI		0x380			/* 0x380 ~ 0x3BC */
#define GIC_DIST_IPRIORITYR	0x400			/* 0x400 ~ 0x5FC */

#define GIC_DIST_TARGET		0x800
#define GIC_DIST_SGIR		0xF00
#define GIC_DIST_CPENDSGIR	0xF10
#define GIC_DIST_SPENDSGIR	0xF20

/* Refrence GIC V2.0 (GIC400) */
struct nx_gicc_reg {
	volatile unsigned int ctlr;			/* 0x000 */
	volatile unsigned int pmr;			/* 0x004 */
	volatile unsigned int bpr;			/* 0x008 */
	volatile unsigned int iar;			/* 0x00C */
	volatile unsigned int eoir;			/* 0x010 */
	volatile unsigned int ppr;			/* 0x014 */
	volatile unsigned int hppir;			/* 0x018 */
	volatile unsigned int abpr;			/* 0x01C */
	volatile unsigned int aiar;			/* 0x020 */
	volatile unsigned int aeoir;			/* 0x024 */
	volatile unsigned int ahppir;			/* 0x028 */
	volatile unsigned int rsv0[(0xD0-0x2C)/4];	/* 0x02C ~ 0x0D0 */
	volatile unsigned int apr[2];			/* 0x0D0 ~ 0x0DC */
	volatile unsigned int nsapr;			/* 0x0E0 */
	volatile unsigned int rsv1[(0xFC-0xE4)/4];	/* 0x0E4 ~ 0x0FC */
	volatile unsigned int iidr;			/* 0x0FC */
	volatile unsigned int rsv2[(0x1000-0x100)/4];	/* 0x0100 ~ 0x1000 */
	volatile unsigned int dir;			/* 0x1000 */
};

struct nx_gicd_reg {
	volatile unsigned int ctlr;			/* 0x000 */
	volatile unsigned int typer;			/* 0x004 */
	volatile unsigned int iidr;			/* 0x008 */
	volatile unsigned int rsv0[29];			/* 0x00C ~ 0x080 */
	volatile unsigned int group[16];		/* 0x080 ~ 0x0C0 */
	volatile unsigned int rsv1[16];			/* 0x0C0 ~ 0x100 */
	volatile unsigned int senabler[16];		/* 0x100 ~ 0x140 */
	volatile unsigned int rsv2[16];			/* 0x140 ~ 0x180 */
	volatile unsigned int cenabler[16];		/* 0x180 ~ 0x1C0 */
	volatile unsigned int rsv3[16];			/* 0x1C0 ~ 0x200 */
	volatile unsigned int spendr[16];		/* 0x200 ~ 0x240 */
	volatile unsigned int rsv4[16];			/* 0x240 ~ 0x280 */
	volatile unsigned int cpendr[16];		/* 0x280 ~ 0x2C0 */
	volatile unsigned int rsv5[16];			/* 0x2C0 ~ 0x300 */
	volatile unsigned int sactiver[16];		/* 0x300 ~ 0x340 */
	volatile unsigned int rsv6[16];			/* 0x340 ~ 0x380 */
	volatile unsigned int cactiver[16];		/* 0x380 ~ 0x3BC */
	volatile unsigned int rsv7[16];			/* 0x3BC ~ 0x400 */
	volatile unsigned int priorityr[128];		/* 0x400 ~ 0x600 */
	volatile unsigned int rsv8[128];		/* 0x600 ~ 0x800 */

	volatile unsigned int itargetsr[128];		/* 0x800 ~ 0xA00 */
	volatile unsigned int rsv9[128];		/* 0xA00 ~ 0xC00 */
	volatile unsigned int icfgr[64];		/* 0xC00 ~ 0xD00 */
	volatile unsigned int ppisr;			/* 0xD00 */
	volatile unsigned int spisr[15];		/* 0xD04 ~ 0xD3C */
	volatile unsigned int rsv10[112];		/* 0xD40 ~ 0xF00 */
	volatile unsigned int sgir;			/* 0xF00 */
	volatile unsigned int rsv11[3];
	volatile unsigned int cpendsgir[4];		/* 0xF10 */
	volatile unsigned int spendsgir[4];		/* 0xF20 */
};

/* External Function */
unsigned int gicc_get_baseaddr(void);
unsigned int gicc_get_iar(void *base);

void gicc_set_ctrl(void *base, int val);
void gicc_set_eoir(void *base, int val);

unsigned int gicd_get_baseaddr(void);

void gicd_set_enable(void *base, int val);
void gicd_set_group(void *base, int val);
void gicd_set_sgir(void *base, int val);

void gic_initialize(unsigned int cpu_num);

#endif  /* __GIC_V1_H__ */
