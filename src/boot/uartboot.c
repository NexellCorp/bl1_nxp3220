/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexcel and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
 */
#include <sysheader.h>
#include <serial.h>
#include <xyz_modem.h>
#include <cmu.h>

/* UART Boot Code Debug Message */
#define UART_DRVDBG_ON				0

#if (defined(DBGLOG_ON) && UART_DRVDBG_ON)
#define DRV_DBGOUT(...)		printf("DEBUG-UART:  ", __VA_ARGS__)
#else
#define DRV_DBGOUT(...)		empty_printf("DEBUG-UART :  ", __VA_ARGS__)
#endif

/* Global Variables */
static struct nx_uart_fnptr *g_uartfn;

int uartboot(struct nx_bootmanager *pbm, unsigned int option)
{
	connection_info_t info;
	xyzsts xyz;

	unsigned int channel = ((option >> UARTPORT) & 0x7);

	int size = 0;;
	int err, res;
	int ret = 0;

	info.mode = xyzModem_ymodem;

	/* step 00. get the bl0 function-table address */
	g_uartfn = ((struct nx_uart_fnptr *)&g_bl1_fn->uart_fn);

	/* step 01. set the intialize & baudrate */
	if (pbm->bi.serial_ch != channel)
		serial_init(channel);
//	serial_set_baudrate(channel, cmu_get_rate(UART_0_CORE_CLK), baud_rate);

	/* step 02. first protocol handshaking (filename, size) */
	do {
		g_uartfn->xyzModem_flush(channel);
		res = g_uartfn->xyzModem_stream_open(&info, &xyz, &err);
	} while(res);

	/* step 03-1. read the header  */
	if ((res = g_uartfn->xyzModem_stream_read(&xyz, ((char*)&pbm->bi),
			sizeof(struct sbi_header), &err)) > 0)
		size += res;
	/* step 03-2. check the nexell signature */
	if (pbm->bi.signature != HEADER_ID) {
		ERROR("Header Signature Ffail(%08x)\r\n", pbm->bi.signature);
		ret = -1;
		goto error;
	}

	/* step 04. read the hash data */
	if ((res = g_uartfn->xyzModem_stream_read(&xyz,
		((char*)&pbm->rsa_encrypted_sha256_hash[0]),
			sizeof(pbm->rsa_encrypted_sha256_hash), &err)) > 0)
		size += res;

	/* step 05. read the boot-image0 */
	if ((res = g_uartfn->xyzModem_stream_read(&xyz, (char*)pbm->bi.load_addr,
			(pbm->bi.load_size + 1), &err)) > 0)
		size += res;
	ret = g_uartfn->xyzModem_error(err);

	/* step 06. close the ymodem protocol */
	g_uartfn->xyzModem_stream_close(&err);
	g_uartfn->xyzModem_stream_terminate(&xyz, FALSE, g_uartfn->xyzModem_getc);

	if (ret < 0) {
		ERROR("Receive Data Broken!!! \r\n");
		ret = -1;
		goto error;
	}
error:
	/* step 09. deinitialize the uartX controller */
//	serial_deinit();

	return ret;
}
