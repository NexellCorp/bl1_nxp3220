/*
 * Copyright(C) 2018 Nexell Co., Ltd. All rights reserved.
 *
 * Nexell retain all intellectual property and proprietary rights in and
 * to this software and related documentation. Any use, reproduction,
 * disclosure, or distribution of this software and related documentation
 * without an express license agreement from Nexell company is strictly
 * prohibited.
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
