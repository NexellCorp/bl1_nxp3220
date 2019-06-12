/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __XYZ_MODEM_H__
#define __XYZ_MODEM_H__

#include <type.h>
typedef int bool;

#define ZM_DEBUG_ENB		0

#define xyzModem_xmodem 	1
#define xyzModem_ymodem		2
/* Don't define this until the protocol support is in place */
/*#define xyzModem_zmodem 3 */

#define xyzModem_access		-1
#define xyzModem_noZmodem	-2
#define xyzModem_timeout	-3
#define xyzModem_eof		-4
#define xyzModem_cancel		-5
#define xyzModem_frame		-6
#define xyzModem_cksum		-7
#define xyzModem_sequence	-8

#define xyzModem_close		1
#define xyzModem_abort		2

#define CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT
#define CYGACC_CALL_IF_SET_CONSOLE_COMM(x)

#define diag_vprintf		vprintf
#define diag_printf		printf
#define diag_vsprintf		vsprintf

#define CYGACC_CALL_IF_DELAY_US(x)	udelay(x)

typedef struct {
	char *filename;
	int mode;
	int chan;
} connection_info_t;

typedef struct {
	int chan;
	unsigned char pkt[1024], *bufp;
	unsigned char blk, cblk, crc1, crc2;
	unsigned char next_blk; /* Expected block */
	int len, mode, total_retries;
	int total_SOH, total_STX, total_CAN;
	bool crc_mode, at_eof, tx_ack;
	unsigned long file_length, read_length;
} xyzsts;

#endif /* #ifndef __XYZ_MODEM_H__ */
