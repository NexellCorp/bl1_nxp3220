/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __BOOTOPTION_H__
#define __BOOTOPTION_H__

#define BOOTMODE		0

#define EMMCBOOT		0	// boot partition boot
#define USBBOOT			1
#define SDFSBOOT		2	// data partition only
#define SDBOOT			3	// data sector boot
#define NANDECBOOT		4
#define SPIBOOT			5
#define UARTBOOT		6
#define XIP			7

#define PORTNUMBER		5

#define SERIALFLASHADDR		3	// 0: 3 step 1: 4 step
#define SERIALFLASHSPEED	4	// 0: 1MHz, 1: 16MHz
#define SELSPIPORT		5	// 0, 1, 2

#define eMMCBOOTMODE		3	// 0: cmd line low, 1: boot cmd(0xFFFFFFFA)
#define eMMCBUSWIDTH		4	// 0: 8 bit, 1: 4 bit
#define SELSDPORT		5	// 0, 1, 2: port number, 3: hispeed p 0

#define NANDTYPE		3	// 0: small, 1: large
#define NANDADDRSTEP		4	// small: 0:3, 1:4, large: 0:4, 1:5
#define NANDPAGE		5	// 0: 2k, 1:4K, 2:8K, 3:16K ~

#define UARTBAUDRATE		3	// 0: 115200, 1:921600
#define UARTPORT		4	// 0 ~ 6 port

#define ICACHE			9	// rstcfg only. 0: disable, 1: enable
#define EXNOBOOTMSG		10	// 0: no message, 1:uart0 message output

/*
 * EXNOBOOTMSG	NOBOOTMSG
 *	0	    0		uart out
 *	0	    1		memory save
 *	1	    0		memory save
 *	1	    1		no message out
 */
#define NEXTTRYPORT		7	// 0, 1, 2: port number, 3: not try

#define USE_SDFS		9	// 0: sector read, 1: file read

#define VALIDFIELD		10	// 0: valid, 1: invalid

#define BOOTCFGUSE		11	// 0: not use, 1: use

#define BOOTTRY			24	// 0: first boot, 1: next boot
#define DECRYPT			25	// virtual flag bit.
#define EXNOBOOTMSG_SAVE	26	// saved external boot mode pin status
#define EXSPEEDUP_SAVE		27	// saved external boot mode pin status
#define SPEEDUP			28	// system speed up
#define NOBOOTMSG		29	// 0: boot message, 1: no boot message
#define VERIFY			30	// 0: 0, 1: 1, 2:2, 3: no verify

#define AESENB			1
#define VERIFYENB		2

#endif /* #ifndef __BOOTOPTION_H__ */
