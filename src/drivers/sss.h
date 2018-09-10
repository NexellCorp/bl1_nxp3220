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
#ifndef __SSS_H__
#define __SSS_H__

struct nx_hrdmadesc {
	unsigned int hrdmac;							/* 00 control of hash receiving DMA	*/
	unsigned int hrdmash;							/* 04 start address high		*/
	unsigned int hrdmas;							/* 08 start address low			*/
	unsigned int hrdmal;							/* 0C length				*/
};

struct nx_brtdmadesc {
	unsigned int brdmac;							/* 00 control of block cipher receiving DMA	*/
	unsigned int brdmash;							/* 04 start address high			*/
	unsigned int brdmas;							/* 08 start address low				*/
	unsigned int brdmal;							/* 0C length					*/
	unsigned int btdmac;							/* 10 control fo block ciper transmitting DMA	*/
	unsigned int btdmash;							/* 14 start address high			*/
	unsigned int btdmas;							/* 18 start address low				*/
	unsigned int btdmal;							/* 1C length					*/
};

/* Function Define */
 void sssc_reset(unsigned int enable);
 int sss_load(struct nx_bootmanager *pbm, unsigned int option);

#endif /* #ifndef __SSS_H__ */
