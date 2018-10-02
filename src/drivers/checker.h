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
#ifndef __CHECKER_H__
#define __CHECKER_H__

/* define the function */
void aes_cbc_decrypt_bimage(struct nx_bootmanager *pbm, unsigned int iv[]);
void sss_generate_hash(unsigned int base, unsigned int size, unsigned char *phash);

 void bimage_generate_hash(struct nx_bootmanager *pbm, unsigned char* phash);
 int authenticate_image(unsigned char *pbootkey,
	 unsigned char *phash, unsigned char *psign);

 int authenticate_bimage(struct nx_bootmanager *pbm,
	 unsigned char *rsa_public_key, unsigned int verify_enb);
#endif /* #ifndef __CHECKER_H__ */
