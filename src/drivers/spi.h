/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __SPI_H__
#define __SPI_H__

struct nx_spi_reg {
	volatile unsigned int ctrlr0;		/* 0x00 : control					*/
	volatile unsigned int ctrlr1;		/* 0x04 : control for only master			*/
	volatile unsigned int ssienr;		/* 0x08 : enable					*/
	volatile unsigned int mwcr;		/* 0x0C : control for microwave serial protocol	*/
	volatile unsigned int ser;		/* 0x10 : slave enable for only master			*/
	volatile unsigned int baudr;		/* 0x14 : baud rate select				*/
	volatile unsigned int txftlr;		/* 0x18 : transmit fifo threshold level		*/
	volatile unsigned int rxftlr;		/* 0x1C : receive fifo threshold level			*/
	volatile unsigned int txflr;		/* 0x20 : transmit fifo level				*/
	volatile unsigned int rxflr;		/* 0x24 : receive fifo level				*/
	volatile unsigned int sr;		/* 0x28 : status					*/
	volatile unsigned int imr;		/* 0x2C : interrupt mask				*/
	volatile unsigned int isr;		/* 0x30 : interrupt status				*/
	volatile unsigned int risr;		/* 0x34 : raw interrupt status				*/
	volatile unsigned int txoicr;		/* 0x38 : transmit fifo overflow interrupt clear	*/
	volatile unsigned int rxoicr;		/* 0x3C : receive fifo overflow interrupt clear	*/
	volatile unsigned int rxuicr;		/* 0x40 : receive fifo underflow interrupt clear	*/
	volatile unsigned int msticr;		/* 0x44 : multi-master interrupt clear			*/
	volatile unsigned int icr;		/* 0x48 : interrupt clear				*/
	volatile unsigned int dmacr;		/* 0x4C : dma control					*/
	volatile unsigned int dmatdlr;		/* 0x50 : dma transmit data level			*/
	volatile unsigned int dmardlr;		/* 0x54 : dma receive data level			*/
	volatile unsigned int idr;		/* 0x58 : idendification				*/
	volatile unsigned int ssi_version_id;	/* 0x5C : coreKit version ID				*/
	volatile unsigned int dr;		/* 0x60 : data						*/
	volatile unsigned int reserved0[35];	/* 0x64 : reserved					*/
	volatile unsigned int rx_sample_dly;	/* 0xF0 : rx sample delay				*/
	volatile unsigned int spi_ctrlr0;	/* 0xF4 : spi control					*/
	volatile unsigned int txd_drive_edge;	/* 0xF8 : transmit drive edge				*/
	volatile unsigned int reserved1;	/* 0xFC : reserved					*/
};

#endif /* #ifndef __SPI_H__ */
