/*
 * Copyright (C) 2018  Nexell Co., Ltd.
 * Author: deoks <truevirtue@nexell.co.kr>
 *
 * Nexell informs that this code and information is provided "as Is" base
 * and without warranty of any kind, either expressed or implied, including,
 * but not limited to the implied warranties of merchantabulity and/or
 * fitness for a aparticular purpose.
 *
 * This program is copyrighted by Nexell and does not allow modification or
 * distribution. In addition to the author (person in charge), the modifier
 * is responsible for the modification.
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
