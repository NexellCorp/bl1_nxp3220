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
#ifndef __NXP3220_H__
#define __NXP3220_H__

#define SRAM_BASEADDR			0xFFFF0000
#define SRAM_SIZE			(64 * 1024)
#define BL1_SRAM_SIZE			(36 * 1024)
#define	BL1_SVC_STACK_SIZE		(0x400)
#define BL1_MON_STACK_SIZE		(0x600)

/* SDRAM in Boot Loader 0 File Format */
#define HEADER_BASEADDR			(SRAM_BASEADDR + SRAM_SIZE - (1 * 1024))
#define HASHSIGN_BASEADDR		(HEADER_BASEADDR   + 256)
#define BOOTKEY_BASEADDR		(HASHSIGN_BASEADDR + 256)
#define USERKEY_BASEADDR		(BOOTKEY_BASEADDR  + 256)

#define RE_HEADER_BASEADDR		USERKEY_BASEADDR

#define DEV_BLK_SIZE			0x200
#define BL1_DEVICE_ADDR			(34 * DEV_BLK_SIZE)			/*  */
#define BL2_DEVICE_ADDR			(BL1_DEVICE_ADDR + (64 * 1024))
#define SSS_DEVICE_ADDR			(BL2_DEVICE_ADDR + (64 * 1024))

/* Fixed the Function Table Address (BL0) */
#define BL0_FUNCTION_TABLE		0x00001000
#define BL0_FNCT_MBEDTLS_ADDR		(BL0_FUNCTION_TABLE + 0x040)
#define BL0_FNCT_USB_ADDR		(BL0_FUNCTION_TABLE + 0x200)
#define BL0_FNCT_UART_ADDR		(BL0_FUNCTION_TABLE + 0x400)
#define BL0_FNCT_SDMMC_ADDR		(BL0_FUNCTION_TABLE + 0x500)
#define BL0_FNCT_SPI_ADDR		(BL0_FUNCTION_TABLE + 0x600)

#define VENDOR_ID			(USB_VENDOR_ID)
#define PRODUCT_ID			(USB_PRODUCT_ID)

#endif // __NXP3220_H__