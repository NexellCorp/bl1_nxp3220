/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
 */
#ifndef __NANDBOOT_H__
#define __NANDBOOT_H__

struct nandbootinfo {
	unsigned char *buf;		// read buffer
	unsigned int zeroimage_page;	// zero base nand offset
	unsigned int copyimage_page;	// if nand data cannot error correction
	unsigned int sector_ptr;	// error correct sector ptr
	unsigned short pagesize;	// nand page size
	unsigned short sectorsize;	// 512 or 1024
	unsigned short datasize;	// unit byte (sectorsize - eccsize)
	unsigned short eccsize;		// unit byte
	unsigned short remainbufsize;
	unsigned char state;
	unsigned char pagecnt;
	unsigned char sectorptr;
	unsigned char address_step;
	char nextretry;			// if none correctable error
	unsigned char imageblockcnt;
	unsigned char checkimagecnt;
};

/* Function Define */
int nandboot(struct nx_bootmanager *pbm, unsigned int option);

#endif /* #ifndef __NANDBOOT_H__  */
