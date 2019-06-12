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

static const unsigned int clk_num[2] = {
	UART_0_APB_CLK, UART_0_CORE_CLK
};

/* Global Variables */
static struct nx_uart_fnptr *g_uartfn;
static xyzsts xyz;

int uartboot(struct nx_bootmanager *pbm, unsigned int option)
{
	connection_info_t info;

	unsigned int channel = ((option >> UARTPORT) & 0x7);
	unsigned int extra_size = 256;

	int size = 0;;
	int err, res;
	int ret = TRUE;

	info.chan = channel;
	info.mode = xyzModem_ymodem;

	/* step 00. get the bl0 function-table address */
	g_uartfn = ((struct nx_uart_fnptr *)&g_bl1_fn->uart_fn);


	/* step 01. set the intialize & baudrate */
	if (pbm->bi.serial_ch != channel) {
		cmu_srcoff_enable(clk_num[0], FALSE);
		cmu_srcoff_enable(clk_num[1], FALSE);
		g_uartfn->serial_init(option);
	}
//	serial_set_baudrate(channel, cmu_get_rate(UART_0_CORE_CLK), baud_rate);

	/* step 02. first protocol handshaking (filename, size) */
	do {
		while (g_uartfn->is_rx_ready(channel))
			g_uartfn->get_ch(channel);
		res = g_uartfn->xyzModem_stream_open(&info, &xyz, &err);
	} while(res);

	/* step 03-1. read the header  */
	if ((res = g_uartfn->xyzModem_stream_read(&xyz, ((char*)&pbm->bi),
			(sizeof(struct sbi_header) + extra_size), &err)) > 0)
		size += res;
	/* step 03-2. check the nexell signature */
	if (pbm->bi.signature != HEADER_ID) {
		ERROR("Header Signature Fail!! (%08x)\r\n", pbm->bi.signature);
		ret = FALSE;
		goto error;
	}

	/* step 04. read the boot-image0 */
	if ((res = g_uartfn->xyzModem_stream_read(&xyz, (char*)pbm->bi.load_addr,
			(pbm->bi.load_size), &err)) > 0)
		size += res;

	/* step 05. read the signature data */
	if ((res = g_uartfn->xyzModem_stream_read(&xyz,
		((char*)&pbm->rsa_encrypted_sha256_hash[0]),
			sizeof(pbm->rsa_encrypted_sha256_hash) + 1, &err)) > 0)
		size += res;

	/* step 06. close the ymodem protocol */
	g_uartfn->xyzModem_stream_close(&err);
	g_uartfn->xyzModem_stream_terminate(&xyz, FALSE, g_uartfn->xyzModem_getc);
error:

	return ret;
}
