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
#ifndef __USBBOOT_H__
#define __USBBOOT_H__

#define USB_VENDOR_ID			(0x2375)
#define USB_PRODUCT_ID			(0x3220)

#define	HIGH_USB_VER			0x0200		// 2.0
#define	HIGH_MAX_PKT_SIZE_EP0		64
#define	HIGH_MAX_PKT_SIZE_EP1		512		// bulk
#define	HIGH_MAX_PKT_SIZE_EP2		512		// bulk

#define	FULL_USB_VER			0x0110		// 1.1
#define	FULL_MAX_PKT_SIZE_EP0		8		// Do not modify
#define	FULL_MAX_PKT_SIZE_EP1		64		// bulk
#define	FULL_MAX_PKT_SIZE_EP2		64		// bulk

#define RX_FIFO_SIZE			512
#define NPTX_FIFO_START_ADDR		RX_FIFO_SIZE
#define NPTX_FIFO_SIZE			512
#define PTX_FIFO_SIZE			512

#define	DEVICE_DESCRIPTOR_SIZE		(18)
#define	CONFIG_DESCRIPTOR_SIZE		(9 + 9 + 7 + 7)

// configuration descriptor: bmAttributes
enum CONFIG_ATTRIBUTES {
	CONF_ATTR_DEFAULT		= 0x80,
	CONF_ATTR_REMOTE_WAKEUP 	= 0x20,
	CONF_ATTR_SELFPOWERED		= 0x40
};

// endpoint descriptor
enum ENDPOINT_ATTRIBUTES {
	EP_ADDR_IN			= 0x80,
	EP_ADDR_OUT			= 0x00,

	EP_ATTR_CONTROL			= 0x00,
	EP_ATTR_ISOCHRONOUS		= 0x01,
	EP_ATTR_BULK			= 0x02,
	EP_ATTR_INTERRUPT		= 0x03
};

// Standard bRequest codes
enum STANDARD_REQUEST_CODE {
	STANDARD_GET_STATUS		= 0,
	STANDARD_CLEAR_FEATURE		= 1,
	STANDARD_RESERVED_1		= 2,
	STANDARD_SET_FEATURE		= 3,
	STANDARD_RESERVED_2		= 4,
	STANDARD_SET_ADDRESS		= 5,
	STANDARD_GET_DESCRIPTOR		= 6,
	STANDARD_SET_DESCRIPTOR		= 7,
	STANDARD_GET_CONFIGURATION	= 8,
	STANDARD_SET_CONFIGURATION	= 9,
	STANDARD_GET_INTERFACE		= 10,
	STANDARD_SET_INTERFACE		= 11,
	STANDARD_SYNCH_FRAME		= 12
};

enum DESCRIPTORTYPE {
	DESCRIPTORTYPE_DEVICE		= 1,
	DESCRIPTORTYPE_CONFIGURATION	= 2,
	DESCRIPTORTYPE_STRING		= 3,
	DESCRIPTORTYPE_INTERFACE	= 4,
	DESCRIPTORTYPE_ENDPOINT		= 5
};

#define CONTROL_EP		0
#define BULK_IN_EP		1
#define BULK_OUT_EP		2

/*
 * USB2.0 HS OTG
 */

struct nx_usb_otg_gcsr_reg {
	volatile unsigned int gotgctl;						/* 0x000 R/W OTG Control and Status Register */
	volatile unsigned int gotgint;						/* 0x004 R/W OTG Interrupt Register */
	volatile unsigned int gahbcfg;						/* 0x008 R/W Core AHB Configuration Register */
	volatile unsigned int gusbcfg;						/* 0x00C R/W Core USB Configuration Register */
	volatile unsigned int grstctl;						/* 0x010 R/W Core Reset Register */
	volatile unsigned int gintsts;						/* 0x014 R/W Core Interrupt Register */
	volatile unsigned int gintmsk;						/* 0x018 R/W Core Interrupt Mask Register */
	volatile unsigned int grxstsr;						/* 0x01C R   Receive Status Debug Read Register */
	volatile unsigned int grxstsp;						/* 0x020 R/W Receive Status Debug Pop Register */
	volatile unsigned int grxfsiz;						/* 0x024 R/W Receive FIFO Size Register */
	volatile unsigned int gnptxfsiz;					/* 0x028 R   Non-Periodic Transmit FIFO Size Register */
	volatile unsigned int gnptxsts;						/* 0x02C R/W Non-Periodic Transmit FIFO/Queue Status Register */
	volatile unsigned int greserved0;					/* 0x030     Reserved */
	volatile unsigned int greserved1;					/* 0x034     Reserved */
	volatile unsigned int greserved2;					/* 0x038     Reserved */
	volatile unsigned int guid;						/* 0x03C R   User ID Register */
	volatile unsigned int gsnpsid;						/* 0x040 R   Synopsys ID Register */
	volatile unsigned int ghwcfg1;						/* 0x044 R   User HW Config1 Register */
	volatile unsigned int ghwcfg2;						/* 0x048 R   User HW Config2 Register */
	volatile unsigned int ghwcfg3;						/* 0x04C R   User HW Config3 Register */
	volatile unsigned int ghwcfg4;						/* 0x050 R   User HW Config4 Register */
	volatile unsigned int glpmcfg;						/* 0x054 R/W Core LPM Configuration Register */
	volatile unsigned int greserved3[(0x100-0x058)/4];			/* 0x058 ~ 0x0FC */
	volatile unsigned int hptxfsiz;						/* 0x100 R/W Host Periodic Transmit FIFO Size Register */
	volatile unsigned int dieptxf[15];					/* 0x104 ~ 0x13C R/W Device IN Endpoint Transmit FIFO Size Register */
	volatile unsigned int greserved4[(0x400-0x140)/4];			/* 0x140 ~ 0x3FC */
};

struct nx_usb_otg_host_channel_reg {
	volatile unsigned int hcchar;						/* 0xn00 R/W Host Channel-n Characteristics Register */
	volatile unsigned int hcsplt;						/* 0xn04 R/W Host Channel-n Split Control Register */
	volatile unsigned int hcint;						/* 0xn08 R/W Host Channel-n Interrupt Register */
	volatile unsigned int hcintmsk;						/* 0xn0C R/W Host Channel-n Interrupt Mask Register */
	volatile unsigned int hctsiz;						/* 0xn10 R/W Host Channel-n Transfer Size Register */
	volatile unsigned int hcdma;						/* 0xn14 R/W Host Channel-n DMA Address Register */
	volatile unsigned int hcreserved[2];					/* 0xn18, 0xn1C Reserved */
};

struct nx_usb_otg_hmcsr_reg {
	volatile unsigned int hcfg;						/* 0x400 R/W Host Configuration Register */
	volatile unsigned int hfir;						/* 0x404 R/W Host Frame Interval Register */
	volatile unsigned int hfnum;						/* 0x408 R   Host Frame Number/Frame Time Remaining Register */
	volatile unsigned int hreserved0;					/* 0x40C     Reserved */
	volatile unsigned int hptxsts;						/* 0x410 R/W Host Periodic Transmit FIFO/Queue Status Register */
	volatile unsigned int haint;						/* 0x414 R   Host All Channels Interrupt Register */
	volatile unsigned int haintmsk;						/* 0x418 R/W Host All Channels Interrupt Mask Register */
	volatile unsigned int hreserved1[(0x440-0x41c)/4];			/* 0x41C ~ 0x43C Reserved */
	volatile unsigned int hprt;						/* 0x440 R/W Host Port Control and Status Register */
	volatile unsigned int hreserved2[(0x500-0x444)/4];			/* 0x444 ~ 0x4FC Reserved */
	struct nx_usb_otg_host_channel_reg hcc[16];				/* 0x500 ~ 0x6FC */
	volatile unsigned int hreserved3[(0x800-0x700)/4];			/* 0x700 ~ 0x7FC */
};

struct nx_usb_otg_device_epi_reg {
	volatile unsigned int diepctl;						/* 0xn00 R/W Device Control IN endpoint n Control Register */
	volatile unsigned int dreserved0;					/* 0xn04     Reserved */
	volatile unsigned int diepint;						/* 0xn08 R/W Device Endpoint-n Interrupt Register */
	volatile unsigned int dreserved1;					/* 0xn0C     Reserved */
	volatile unsigned int dieptsiz;						/* 0xn10 R/W Device Endpoint-n Transfer Size Register */
	volatile unsigned int diepdma;						/* 0xn14 R/W Device Endpoint-n DMA Address Register */
	volatile unsigned int dtxfsts;						/* 0xn18 R   Device IN Endpoint Transmit FIFO Status Register */
	volatile unsigned int diepdmab;						/* 0xn1C R   Device Endpoint-n DMA Buffer Address Register */
};

struct nx_usb_otg_device_ep0_reg {
	volatile unsigned int doepctl;						/* 0xn00 R/W Device Control OUT endpoint n Control Register */
	volatile unsigned int dreserved0;					/* 0xn04     Reserved */
	volatile unsigned int doepint;						/* 0xn08 R/W Device Endpoint-n Interrupt Register */
	volatile unsigned int dreserved1;					/* 0xn0C     Reserved */
	volatile unsigned int doeptsiz;						/* 0xn10 R/W Device Endpoint-n Transfer Size Register */
	volatile unsigned int doepdma;						/* 0xn14 R/W Device Endpoint-n DMA Address Register */
	volatile unsigned int dreserved2;					/* 0xn18     Reserved */
	volatile unsigned int doepdmab;						/* 0xn1C R   Device Endpoint-n DMA Buffer Address Register */
};

struct nx_usb_otg_dmcsr_reg {
	volatile unsigned int dcfg;						/* 0x800 R/W Device Configuration Register */
	volatile unsigned int dctl;						/* 0x804 R/W Device Control Register */
	volatile unsigned int dsts;						/* 0x808 R   Device Status Register */
	volatile unsigned int dreserved0;					/* 0x80C     Reserved */
	volatile unsigned int diepmsk;						/* 0x810 R/W Device IN Endpoint Common Interrupt Mask Register */
	volatile unsigned int doepmsk;						/* 0x814 R/W Device OUT Endpoint Common Interrupt Mask Register */
	volatile unsigned int daint;						/* 0x818 R   Device All Endpoints Interrupt Register */
	volatile unsigned int daintmsk;						/* 0x81C R/W Device All Endpoints Interrupt Mask Register */
	volatile unsigned int dreserved1;					/* 0x820     Reserved */
	volatile unsigned int dreserved2;					/* 0x824     Reserved */
	volatile unsigned int dvbusdis;						/* 0x828 R/W Device VBUS Discharge Time Register */
	volatile unsigned int dvbuspulse;					/* 0x82C R/W Device VBUS Pulsing Time Register */
	volatile unsigned int dthrctl;						/* 0x830 R/W Device Threshold Control Register */
	volatile unsigned int diepempmsk;					/* 0x834 R/W Device IN Endpoint FIFO Empty Interrupt Mask Register */
	volatile unsigned int dreserved3;					/* 0x838     Reserved */
	volatile unsigned int dreserved4;					/* 0x83C     Reserved */
	volatile unsigned int dreserved5[0x10];					/* 0x840 ~ 0x87C    Reserved */
	volatile unsigned int dreserved6[0x10];					/* 0x880 ~ 0x8BC    Reserved */
	volatile unsigned int dreserved7[0x10];					/* 0x8C0 ~ 0x8FC    Reserved */
	struct nx_usb_otg_device_epi_reg depir[16];				/* 0x900 ~ 0xAFC */
	struct nx_usb_otg_device_ep0_reg depor[16];				/* 0xB00 ~ 0xCFC */
};

struct nx_usb_otg_phyctrl_reg {
	volatile unsigned int pcreserved0[0x40/4];				/* 0x00 ~ 0x3C	Reserved */
	volatile unsigned int phypor;						/* 0x40 */
	volatile unsigned int vbusintenb;					/* 0x44 */
	volatile unsigned int vbuspend;						/* 0x48 */
	volatile unsigned int testparm3;					/* 0x4C */
	volatile unsigned int testparm4;					/* 0x50 */
	volatile unsigned int linkctl;						/* 0x54 */
	volatile unsigned int testparm6;					/* 0x58 */
	volatile unsigned int testparm7;					/* 0x5C */
	volatile unsigned int testparm8;					/* 0x60 */
	volatile unsigned int testparm9;					/* 0x64 */
	volatile unsigned int pcreserved4[(0x100-0x68)/4];			/* 0x68 ~ 0xFC	Reserved */
};

struct nx_usb_otg_ifclk_reg {
	volatile unsigned int ifreserved0[0xc0/4];				/* 0x00 ~ 0xBC	Reserved  */
	volatile unsigned int ifclk_mode;					/* 0xC0 */
	volatile unsigned int ifclkgen;						/* 0xC4 */
	volatile unsigned int ifreserved1[(0x100-0xc8)/4];			/* 0xC8 ~ 0xFC */
};

struct nx_usbotg_reg {
	struct nx_usb_otg_gcsr_reg  gcsr;					/* 0x0000 ~ 0x03FC */
	struct nx_usb_otg_hmcsr_reg hcsr;					/* 0x0400 ~ 0x07FC */
	struct nx_usb_otg_dmcsr_reg dcsr;					/* 0x0800 ~ 0x0CFC */
	volatile unsigned int greserved0[(0xe00-0xd00)/4];			/* 0x0D00 ~ 0x0DFC : Reserved */
	volatile unsigned int pcgcctl;						/* 0x0E00 R/W Power and Clock Gating Control Register */
	volatile unsigned int greserved1[(0x1000-0xe04)/4];			/* 0x0E04 ~ 0x0FFC : Reserved */
	volatile unsigned int epfifo[15][1024];					/* 0x1000 ~ 0xFFFC Endpoint Fifo */
//	volatile unsigned int epfifo[16][1024];					/* 0x1000 ~ 0x10FFC Endpoint Fifo */
//	volatile unsigned int greserved2[(0x20000-0x11000)/4];			/* 0x11000 ~ 0x20000 Reserved */
//	volatile unsigned int debugfifo[0x8000];				/* 0x20000 ~ 0x3FFFC Debug Purpose Direct Fifo Acess Register */
};

/*definitions related to CSR setting */
/* USB Global Interrupt Status register(GINTSTS) setting value */
#define WkUpInt				(1u<<31)
#define OEPInt				(1<<19)
#define IEPInt				(1<<18)
#define EnumDone			(1<<13)
#define USBRst				(1<<12)
#define USBSusp				(1<<11)
#define RXFLvl				(1<<4)

/* NX_OTG_GOTGCTL*/
#define B_SESSION_VALID			(0x1<<19)
#define A_SESSION_VALID			(0x1<<18)

/* NX_OTG_GAHBCFG*/
#define PTXFE_HALF			(0<<8)
#define PTXFE_ZERO			(1<<8)
#define NPTXFE_HALF			(0<<7)
#define NPTXFE_ZERO			(1<<7)
#define MODE_SLAVE			(0<<5)
#define MODE_DMA			(1<<5)
#define BURST_SINGLE			(0<<1)
#define BURST_INCR			(1<<1)
#define BURST_INCR4			(3<<1)
#define BURST_INCR8			(5<<1)
#define BURST_INCR16			(7<<1)
#define GBL_INT_UNMASK			(1<<0)
#define GBL_INT_MASK			(0<<0)

/* NX_OTG_GRSTCTL*/
#define AHB_MASTER_IDLE			(1u<<31)
#define CORE_SOFT_RESET			(0x1<<0)

/* NX_OTG_GINTSTS/NX_OTG_GINTMSK core interrupt register */
#define INT_RESUME			(1u<<31)
#define INT_DISCONN			(0x1<<29)
#define INT_CONN_ID_STS_CNG		(0x1<<28)
#define INT_OUT_EP			(0x1<<19)
#define INT_IN_EP			(0x1<<18)
#define INT_ENUMDONE			(0x1<<13)
#define INT_RESET			(0x1<<12)
#define INT_SUSPEND			(0x1<<11)
#define INT_TX_FIFO_EMPTY		(0x1<<5)
#define INT_RX_FIFO_NOT_EMPTY		(0x1<<4)
#define INT_SOF				(0x1<<3)
#define INT_DEV_MODE			(0x0<<0)
#define INT_HOST_MODE			(0x1<<1)

/* NX_OTG_GRXSTSP STATUS*/
#define GLOBAL_OUT_NAK			(0x1<<17)
#define OUT_PKT_RECEIVED		(0x2<<17)
#define OUT_TRNASFER_COMPLETED		(0x3<<17)
#define SETUP_TRANSACTION_COMPLETED	(0x4<<17)
#define SETUP_PKT_RECEIVED		(0x6<<17)

/* NX_OTG_DCTL device control register */
#define NORMAL_OPERATION		(0x1<<0)
#define SOFT_DISCONNECT			(0x1<<1)

/* NX_OTG_DAINT device all endpoint interrupt register */
#define INT_IN_EP0			(0x1<<0)
#define INT_IN_EP1			(0x1<<1)
#define INT_IN_EP3			(0x1<<3)
#define INT_OUT_EP0			(0x1<<16)
#define INT_OUT_EP2			(0x1<<18)
#define INT_OUT_EP4			(0x1<<20)

/* NX_OTG_DIEPCTL0/NX_OTG_DOEPCTL0 */
#define DEPCTL_EPENA			(0x1u<<31)
#define DEPCTL_EPDIS			(0x1<<30)
#define DEPCTL_SNAK			(0x1<<27)
#define DEPCTL_CNAK			(0x1<<26)
#define DEPCTL_STALL			(0x1<<21)
#define DEPCTL_ISO_TYPE			(EP_TYPE_ISOCHRONOUS<<18)
#define DEPCTL_BULK_TYPE		(EP_TYPE_BULK<<18)
#define DEPCTL_INTR_TYPE		(EP_TYPE_INTERRUPT<<18)
#define DEPCTL_USBACTEP			(0x1<<15)

/*ep0 enable, clear nak, next ep0, max 64byte */
#define EPEN_CNAK_EP0_64 (DEPCTL_EPENA|DEPCTL_CNAK|(CONTROL_EP<<11)|(0<<0))

/*ep0 enable, clear nak, next ep0, 8byte */
#define EPEN_CNAK_EP0_8 (DEPCTL_EPENA|DEPCTL_CNAK|(CONTROL_EP<<11)|(3<<0))

/* DIEPCTLn/DOEPCTLn */
#define BACK2BACK_SETUP_RECEIVED	(0x1<<6)
#define INTKN_TXFEMP			(0x1<<4)
#define NON_ISO_IN_EP_TIMEOUT		(0x1<<3)
#define CTRL_OUT_EP_SETUP_PHASE_DONE	(0x1<<3)
#define AHB_ERROR			(0x1<<2)
#define TRANSFER_DONE			(0x1<<0)


typedef struct {
	unsigned char bmRequestType;
	unsigned char bRequest;
	unsigned short wValue;
	unsigned short wIndex;
	unsigned short wLength;
} SetupPacket;

typedef enum {
	USB_HIGH,
	USB_FULL,
	USB_LOW
//	,0xFFFFFFFFUL
} USB_SPEED;

typedef enum {
	EP_TYPE_CONTROL,
	EP_TYPE_ISOCHRONOUS,
	EP_TYPE_BULK,
	EP_TYPE_INTERRUPT
} EP_TYPE;

/*------------------------------------------------*/
/* EP0 state */
enum EP0_STATE {
	EP0_STATE_INIT			= 0,
	EP0_STATE_GET_DSCPT		= 1,
	EP0_STATE_GET_INTERFACE		= 2,
	EP0_STATE_GET_CONFIG		= 3,
	EP0_STATE_GET_STATUS		= 4
};

typedef struct __attribute__((aligned(sizeof(unsigned char*)))) tag_usbboot_status {
	unsigned char		*rx_addr;
	unsigned char		*rx_addr_save;
	const unsigned char	*dev_desc;
	const unsigned char	*config_desc;

	unsigned char		*current_ptr;
	unsigned char		*up_ptr;
	unsigned char		*up_addr;

	volatile int		b_downloading;
	int			b_header_received;

//	unsigned int		rx_size;
	int			rx_size;
	unsigned int 		rx_size_save;
	unsigned int		rx_header_size;

	unsigned int		ep0_state;
	USB_SPEED		speed;
	unsigned int		ctrl_max_pktsize;
	unsigned int		bulkin_max_pktsize;
	unsigned int		bulkout_max_pktsize;

	unsigned int		current_fifo_size;
	unsigned int		remain_size;
	unsigned int		up_size;

//	unsigned int		aescbc_iv[4];

	unsigned char		cur_config;
	unsigned char		cur_interface;
	unsigned char		cur_setting;
	unsigned char		reserved;

	unsigned char __attribute__ ((aligned(4)))
		hs_devdesc[DEVICE_DESCRIPTOR_SIZE];
	unsigned char __attribute__ ((aligned(4)))
		fs_devdesc[DEVICE_DESCRIPTOR_SIZE];

	unsigned int		rx_image_size;
	int			b_image_received;

} usbboot_status;

/* Function Define */
 void usb_int_bulkout(usbboot_status *pusbbt_st, struct nx_bootmanager *pbm,
		 unsigned int fifo_cnt_byte);
 int usbboot(struct nx_bootmanager *pbm);

#endif /* #ifndef __USBBOOT_H__ */
