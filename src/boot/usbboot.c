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
#include <usbboot.h>
#include <cmu.h>
#include <efuse.h>
#include <sysreg.h>
#include <pmu.h>

/* USB Boot Code Debug Message */
#define USB_DRVDBG_ON			0

#if (defined(DBGLOG_ON) && USB_DRVDBG_ON)
#define DRV_DBGOUT		printf
#else
#define DRV_DBGOUT		empty_printf
#endif

/* Global Variables */
static struct nx_usbotg_reg *g_usbotg_reg
	= ((struct nx_usbotg_reg *)PHY_BASEADDR_USBOTG20_MODULE);
static struct nx_usb_fnptr *g_usbfn;

static void usb_get_identy(unsigned int *identy_d)
{
	int i;

	identy_d[0] = efuse_get_ecid(0);
	identy_d[1] = efuse_get_ecid(1);
	identy_d[2] = efuse_get_ecid(2);
	identy_d[3] = efuse_get_ecid(3);
	for (i = 4; i < (512/4); i++)
		identy_d[i] = 0;
}

void usb_int_bulkout(usbboot_status *pusbbt_st, struct nx_bootmanager *pbm,
		unsigned int fifo_cnt_byte)
{
	if (TRUE != pusbbt_st->b_header_received) {
		unsigned int *pbuf = (unsigned int*)pbm;

		g_usbfn->usb_read_out_fifo(BULK_OUT_EP,
			(unsigned char *)&pbuf[pusbbt_st->rx_header_size/4],
				fifo_cnt_byte);

		if ((fifo_cnt_byte & 3) == 0) {
			pusbbt_st->rx_header_size += fifo_cnt_byte;
		} else {
			DRV_DBGOUT("Stall: Size Align: %d \r\n", fifo_cnt_byte);
			mmio_set_32(&g_usbotg_reg->dcsr.depor[BULK_OUT_EP].doepctl,
				DEPCTL_STALL);
			return;
		}

		if (sizeof(struct sbi_header) <= pusbbt_st->rx_header_size) {
			struct sbi_header *ptbi = (struct sbi_header *)&pbm->bi;

			if (ptbi->signature == HEADER_ID) {
				pusbbt_st->b_header_received = TRUE;

				pusbbt_st->rx_addr = (unsigned char*)pbm->bi.load_addr;
				pusbbt_st->rx_addr_save =
					(unsigned char*)pbm->bi.load_addr;
				pusbbt_st->rx_size = pbm->bi.load_size;
				pusbbt_st->rx_size_save = pusbbt_st->rx_size;

				DRV_DBGOUT("Load Addr: %x, Size: %x \r\n",
					(unsigned char *)pusbbt_st->rx_addr,
						pusbbt_st->rx_size);

				pusbbt_st->b_image_received = FALSE;
				pusbbt_st->rx_image_size = 0;

			} else {
				pusbbt_st->rx_header_size = 0;
				pusbbt_st->rx_image_size  = 0;
				mmio_set_32(&g_usbotg_reg->dcsr.depor[BULK_OUT_EP].doepctl,
					DEPCTL_STALL);
			}
		}

	} else if ((TRUE != pusbbt_st->b_image_received)
			&& (TRUE == pusbbt_st->b_header_received)) {

		g_usbfn->usb_read_out_fifo(BULK_OUT_EP,
				(unsigned char *)pusbbt_st->rx_addr,
				fifo_cnt_byte);

		pusbbt_st->rx_addr += fifo_cnt_byte;
		pusbbt_st->rx_size -= fifo_cnt_byte;

		if (pusbbt_st->rx_size <= 0) {
			pusbbt_st->b_image_received = TRUE;
			pusbbt_st->rx_addr =
				(unsigned char*)pbm->rsa_encrypted_sha256_hash;
			pusbbt_st->rx_addr_save =
				(unsigned char*)pbm->rsa_encrypted_sha256_hash;
			pusbbt_st->rx_size = sizeof(pbm->rsa_encrypted_sha256_hash);
			pusbbt_st->rx_size_save = pusbbt_st->rx_size;
		}
	} else {
		ASSERT((pusbbt_st->rx_size) > 0);
		ASSERT(0 == ((unsigned int)pusbbt_st->rx_addr & 3));

		g_usbfn->usb_read_out_fifo(BULK_OUT_EP,
				(unsigned char *)pusbbt_st->rx_addr,
				fifo_cnt_byte);
		DRV_DBGOUT("Bin Packet Size = %x ==> %x, %x\r\n",
			pusbbt_st->rx_size, pusbbt_st->rx_addr, pusbbt_st->rx_size);

		pusbbt_st->rx_addr += fifo_cnt_byte;
		pusbbt_st->rx_size -= fifo_cnt_byte;

		if (pusbbt_st->rx_size <= 0) {
			DRV_DBGOUT("Download Done!\r\n");
			pusbbt_st->b_downloading     = FALSE;
			pusbbt_st->b_header_received = FALSE;
			pusbbt_st->b_image_received  = FALSE;
			pusbbt_st->rx_header_size    = FALSE;
		}
	}

	mmio_write_32(&g_usbotg_reg->dcsr.depor[BULK_OUT_EP].doeptsiz,
		((1 << 19) | (pusbbt_st->bulkout_max_pktsize << 0)));

	/*ep2 enable, clear nak, bulk, usb active, next ep2, max pkt 64*/
	mmio_write_32(&g_usbotg_reg->dcsr.depor[BULK_OUT_EP].doepctl,
		(1u << 31 | 1 << 26 | 2 << 18 | 1 << 15 |
		pusbbt_st->bulkout_max_pktsize << 0));
}

static void usb_pkt_receive(usbboot_status *pusbbt_st, struct nx_bootmanager *pbm)
{
	unsigned int rx_status;
	unsigned int fifo_cnt_byte;

	rx_status = mmio_read_32(&g_usbotg_reg->gcsr.grxstsp);

	if ((rx_status & (0xf << 17)) == SETUP_PKT_RECEIVED) {
		DRV_DBGOUT("SETUP_PKT_RECEIVED \r\n");
		g_usbfn->usb_ep0_int_hndlr(pusbbt_st);
	} else if ((rx_status & (0xf << 17)) == OUT_PKT_RECEIVED) {
		fifo_cnt_byte = (rx_status & 0x7ff0) >> 4;
		DRV_DBGOUT("OUT_PKT_RECEIVED \r\n");

		if ((rx_status & BULK_OUT_EP)&&(fifo_cnt_byte)) {
			usb_int_bulkout(pusbbt_st, pbm,
				fifo_cnt_byte);
			mmio_write_32(&g_usbotg_reg->gcsr.gintmsk,
				(INT_RESUME | INT_OUT_EP | INT_IN_EP |
				 INT_ENUMDONE | INT_RESET | INT_SUSPEND |
				 INT_RX_FIFO_NOT_EMPTY));
			return;
		}
	} else if ((rx_status & (0xf << 17)) == GLOBAL_OUT_NAK) {
		DRV_DBGOUT("GLOBAL_OUT_NAK \r\n");
	} else if ((rx_status & (0xf << 17)) == OUT_TRNASFER_COMPLETED) {
		DRV_DBGOUT("OUT_TRNASFER_COMPLETED \r\n");
	} else if ((rx_status & (0xf << 17)) == SETUP_TRANSACTION_COMPLETED) {
		DRV_DBGOUT("SETUP_TRANSACTION_COMPLETED \r\n");
	} else {
		DRV_DBGOUT("Reserved \r\n");
	}
}

static void udc_int_hndlr(usbboot_status *pusbbt_st, struct nx_bootmanager *pbm)
{
	unsigned int int_status;
	int tmp;

	int_status = mmio_read_32(&g_usbotg_reg->gcsr.gintsts);			/* Core Interrupt Register */

	if (int_status & INT_RESET) {
		DRV_DBGOUT("INT_RESET \r\n");
		g_usbfn->usb_reset(pusbbt_st);
	}

	if (int_status & INT_ENUMDONE) {
		DRV_DBGOUT("INT_ENUMDONE :");
		tmp = g_usbfn->usb_set_init(pusbbt_st);

		if (tmp == FALSE) {
			mmio_write_32(&g_usbotg_reg->gcsr.gintsts, int_status);	/* Interrupt Clear */
			return;
		}
	}

	if (int_status & INT_RESUME) {
		DRV_DBGOUT("INT_RESUME \r\n");
	}

	if (int_status & INT_SUSPEND) {
		DRV_DBGOUT("INT_SUSPEND \r\n");
	}

	if (int_status & INT_RX_FIFO_NOT_EMPTY) {
		DRV_DBGOUT("INT_RX_FIFO_NOT_EMPTY \r\n");

		/* Read only register field */
		mmio_write_32(&g_usbotg_reg->gcsr.gintmsk,
			(INT_RESUME | INT_OUT_EP | INT_IN_EP |
			 INT_ENUMDONE | INT_RESET | INT_SUSPEND));
		usb_pkt_receive(pusbbt_st, pbm);
		mmio_write_32(&g_usbotg_reg->gcsr.gintmsk,
			(INT_RESUME | INT_OUT_EP | INT_IN_EP |
			 INT_ENUMDONE | INT_RESET | INT_SUSPEND |
			 INT_RX_FIFO_NOT_EMPTY));
	}

	if ((int_status & INT_IN_EP) || (int_status & INT_OUT_EP)) {
		DRV_DBGOUT("INT_IN or OUT_EP \r\n");
		/* Read only register field */
		g_usbfn->usb_transfer(pusbbt_st);
	}
	mmio_write_32(&g_usbotg_reg->gcsr.gintsts, int_status);			/* Interrupt Clear */
}

int usbboot(struct nx_bootmanager *pbm)
{
	struct nx_sysreg_usb_reg *sysusb_reg
		= ((struct nx_sysreg_usb_reg *)PHY_BASEADDR_SYSREG_USB_MODULE);

	usbboot_status usbbt_st, *pusbbt_st;
	__attribute__ ((aligned(4))) unsigned char updata[512];
	unsigned short vid, pid;

	unsigned int id = efuse_get_ecid(3);

	unsigned int i;

	g_usbfn = ((struct nx_usb_fnptr *)&g_bl1_fn->usb_fn);

	pusbbt_st = &usbbt_st;

	if (id == 0) {
		vid = USB_VENDOR_ID;
		pid = USB_PRODUCT_ID;
	} else {
		vid = (id >> 16) & 0xFFFF;
		pid = (id >>  0) & 0xFFFF;
	}

	DRV_DBGOUT("VID:%04X, PID:%04X\r\n", vid, pid);

	/* step xx. zero init the vriable  */
	memset((void*)pusbbt_st, 0, sizeof(usbboot_status));

	/* step xx.  */
	for (i = 0; i < DEVICE_DESCRIPTOR_SIZE; i++) {
		pusbbt_st->hs_devdesc[i] = (*g_usbfn->gs_devdesc_hs)[i];
		pusbbt_st->fs_devdesc[i] = (*g_usbfn->gs_devdesc_fs)[i];
	}

	pusbbt_st->hs_devdesc[ 8] = (vid >> 0) & 0xFF;
	pusbbt_st->hs_devdesc[ 9] = (vid >> 8) & 0xFF;
	pusbbt_st->hs_devdesc[10] = (pid >> 0) & 0xFF;
	pusbbt_st->hs_devdesc[11] = (pid >> 8) & 0xFF;
	pusbbt_st->fs_devdesc[ 8] = (vid >> 0) & 0xFF;
	pusbbt_st->fs_devdesc[ 9] = (vid >> 8) & 0xFF;
	pusbbt_st->fs_devdesc[10] = (pid >> 0) & 0xFF;
	pusbbt_st->fs_devdesc[11] = (pid >> 8) & 0xFF;

	pusbbt_st->up_addr = updata;
	pusbbt_st->up_ptr  = updata;
	pusbbt_st->up_size = sizeof(updata);

	/* step xx. The identification information is sent to the endpoint.*/
	usb_get_identy((unsigned int*)updata);

	/* step xx. set the system control register for usb-block */
	mmio_clear_32(&sysusb_reg->usb20phy_otg0_ctrl[0], (1 << 4));		// USB20PHY_OTG0_i_POR [4]
	mmio_set_32(&sysusb_reg->usb20phy_otg0_ctrl[0], (1 << 3));		// USB20PHY_OTG0_i_POR_ENB [3]
	udelay(40*106);
	mmio_set_32(&sysusb_reg->otg_ctrl[0], (1 << 8));			// OTG_i_nUtmiResetSync
	udelay(1*106);
	mmio_set_32(&sysusb_reg->otg_ctrl[0], (1 << 7));			// OTG_i_nResetSync
	udelay(1*106);								// 10 clock need

	/* step xx. usb core soft reset */
	mmio_write_32(&g_usbotg_reg->gcsr.grstctl, CORE_SOFT_RESET);
	while (!(mmio_read_32(&g_usbotg_reg->gcsr.grstctl) & AHB_MASTER_IDLE))
		mmio_read_32(&g_usbotg_reg->gcsr.grstctl);

	/* step xx. usb core the initialize */
	mmio_write_32(&g_usbotg_reg->gcsr.gahbcfg,
					(PTXFE_HALF | NPTXFE_HALF | MODE_SLAVE |
					BURST_SINGLE | GBL_INT_UNMASK));
	mmio_write_32(&g_usbotg_reg->gcsr.gusbcfg,
		  ((0 << 15)							/* PHY Low Power Clock sel */
		 | (1 << 14)							/* Non-Periodic TxFIFO Rewind Enable */
		 | (5 << 10)							/* Turnaround time */
		 | (0 <<  9)							/* 0:HNP disable, 1:HNP enable */
		 | (0 <<  8)							/* 0:SRP disable, 1:SRP enable */
		 | (0 <<  7)							/* ULPI DDR sel */
		 | (0 <<  6)							/* 0: high speed utmi+, 1: full speed serial */
		 | (0 <<  4)							/* 0: utmi+, 1:ulpi */
		 | (1 <<  3)							/* phy i/f  0:8bit, 1:16bit */
		 | (7 <<  0)));							/* HS/FS Timeout**/

	/* step xx. check current mode */
	if ((mmio_read_32(&g_usbotg_reg->gcsr.gintsts) & 0x1) == INT_DEV_MODE) {
		/* step xx. soft disconnect on */
		mmio_set_32(&g_usbotg_reg->dcsr.dctl, SOFT_DISCONNECT);
		udelay(100);							/* (11.52us) */
		/* step xx. soft disconnect off */
		mmio_clear_32(&g_usbotg_reg->dcsr.dctl, SOFT_DISCONNECT);

		/* step xx. initialize the usb device */
		mmio_write_32(&g_usbotg_reg->dcsr.dcfg, (1 << 18));
		mmio_write_32(&g_usbotg_reg->gcsr.gintmsk,
			(INT_RESUME | INT_OUT_EP | INT_IN_EP |
			 INT_ENUMDONE | INT_RESET | INT_SUSPEND |
			 INT_RX_FIFO_NOT_EMPTY));
	}

	pusbbt_st->cur_config = 0;
	pusbbt_st->cur_interface = 0;
	pusbbt_st->cur_setting = 0;
	pusbbt_st->speed = USB_HIGH;
	pusbbt_st->ep0_state = EP0_STATE_INIT;

	pusbbt_st->b_downloading = TRUE;
	while (pusbbt_st->b_downloading) {
		if (mmio_read_32(&g_usbotg_reg->gcsr.gintsts) &
				(WkUpInt | OEPInt | IEPInt | EnumDone |
				 USBRst | USBSusp | RXFLvl)) {
			udc_int_hndlr(pusbbt_st, pbm);
			mmio_write_32(&g_usbotg_reg->gcsr.gintsts, 0xFFFFFFFF);
		}
	}
	/* step xx. usb core soft reset */
	mmio_write_32(&g_usbotg_reg->gcsr.grstctl, CORE_SOFT_RESET);
	while (!(mmio_read_32(&g_usbotg_reg->gcsr.grstctl) & AHB_MASTER_IDLE))
		mmio_read_32(&g_usbotg_reg->gcsr.grstctl);

	/* step xx. reset Avalid, Bvalid, sessend */
	mmio_write_32(&g_usbotg_reg->gcsr.gotgctl, 0x00);

	/* step xx. usb core soft reset */
	mmio_write_32(&g_usbotg_reg->gcsr.grstctl, CORE_SOFT_RESET);

	SYSMSG("USB Download Done.\r\n");

	return TRUE;
}
