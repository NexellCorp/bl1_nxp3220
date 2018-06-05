#ifndef __SECURE_H__
#define __SECURE_H__

#define SYS_BUSCFG6				(0xFFFFFFFF)
#define SYS_BUSCFG7				(0xFFFFFFFF)
#define SYS_BUSCFG8				(0xFFFFFFFF)
#define SYS_BUSCFG9				(0xFFFFFFFF)
#define SYS_BUSCFG10				(0xFFFFFFFF)
#define SYS_BUSCFG11				(0xFFFFFFFF)
#define SYS_BUSCFG12				(0xFFFFFFFF)
#define SYS_REGSECURE0				(0x0)
#define SYS_REGSECURE1				(0x0)
#define SYS_REGSECURE2				(0x0)

#define CPU_BUSCTRL0				(0xFFFFFFFF)
#define CPU_BUSCTRL1				(0xFFFFFFFF)
#define CPU_REGSECURE0				(0x0)

#define USB_CFG_BUS_SECURE0			(0xFFFFFFFF)
#define USB_CFG_BUS_SECURE1			(0xFFFFFFFF)
#define USB_REGSECURE0				(0x0)
#define USB_REGSECURE1				(0x0)

#define MM_CFG_BUS				(0xFFFFFFFF)
#define MM_CFG_BUS_SECURE			(0xFFFFFFFF)
#define MM_DATA_BUS				(0xFFFFFFFF)
#define MM_REGSECURE0				(0x0)
#define MM_REGSECURE1				(0x0)

#define HSIF_BUSCTRL0				(0xFFFFFFFF)
#define HSIF_BUSCTRL1				(0xFFFFFFFF)
#define HSIF_REGSECURE0				(0x0)

enum {
	SECURE_ONLY,
	SECURE_NOT_ONLY,
	RESERVED
};


struct tzinfo_reg {
	unsigned char bit_00 : 1;
	unsigned char bit_01 : 1;
	unsigned char bit_02 : 1;
	unsigned char bit_03 : 1;
	unsigned char bit_04 : 1;
	unsigned char bit_05 : 1;
	unsigned char bit_06 : 1;
	unsigned char bit_07 : 1;
	unsigned char bit_08 : 1;
	unsigned char bit_09 : 1;
	unsigned char bit_10 : 1;
	unsigned char bit_11 : 1;
	unsigned char bit_12 : 1;
	unsigned char bit_13 : 1;
	unsigned char bit_14 : 1;
	unsigned char bit_15 : 1;
	unsigned char bit_16 : 1;
	unsigned char bit_17 : 1;
	unsigned char bit_18 : 1;
	unsigned char bit_19 : 1;
	unsigned char bit_20 : 1;
	unsigned char bit_21 : 1;
	unsigned char bit_22 : 1;
	unsigned char bit_23 : 1;
	unsigned char bit_24 : 1;
	unsigned char bit_25 : 1;
	unsigned char bit_26 : 1;
	unsigned char bit_27 : 1;
	unsigned char bit_28 : 1;
	unsigned char bit_29 : 1;
	unsigned char bit_30 : 1;
	unsigned char bit_31 : 1;
};

#endif /* #ifdef __SECURE_H__ */
