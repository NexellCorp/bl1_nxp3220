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
#include <sysheader.h>
#include <checker.h>
#include <sss.h>

/* @brief: It is a function to decrypt only the boot image. */
void aes_cbc_decrypt_bimage(struct nx_bootmanager *pbm, unsigned int iv[])
{
	g_mbedtls->aes_cbc_decrypt_bimage((unsigned int*)pbm->bi.load_addr,
			(unsigned int*)pbm->bi.load_addr, (unsigned int *)iv,
			pbm->bi.load_size);

	NOTICE("Image decrypt complete!! \r\n");

	/* @brief: software reset the sss controller */
	sssc_reset(TRUE);
}

/* @brief: Generate hashes using common data */
void sss_generate_hash(unsigned int base,
		unsigned int size, unsigned char *phash)
{
	struct nx_hrdmadesc desc;
	desc.hrdmac =  ((0 << 14) |						/* burst length */
			(0 <<  9) |						/* ARUSER	*/
			(0 <<  2) |						/* ARPROT	*/
			(0 <<  1));						/* byte swap	*/
	desc.hrdmash = 0;							/* start addr high */
	desc.hrdmas = ((unsigned int)base);					/* start addr low  */
	desc.hrdmal = size;

	g_crypto->get_hash(&desc, 1, (unsigned int *)phash, size);
}

/* @brief: Generate hashes using header and boot-images. */
void bimage_generate_hash(struct nx_bootmanager *pbm,
			unsigned char *rsa_public_key, unsigned char* phash)
{
	struct nx_hrdmadesc desc[3];
	int hsize = (int)sizeof(struct sbi_header);
	int keysize = 256;
	int bsize = (pbm->bi.load_size);

	desc[0].hrdmac = ((0 << 14) |						/* burst length */
			  (0 <<  9) |						/* ARUSER	*/
			  (0 <<  2) |						/* ARPROT	*/
			  (0 <<  1));						/* byte swap	*/
	desc[0].hrdmash = 0;							/* start addr high */
	desc[0].hrdmas = ((unsigned int)&pbm->bi);				/* start addr low  */
	desc[0].hrdmal = hsize;

	desc[1].hrdmac = ((0 << 14) |						/* burst length */
			  (0 <<  9) |						/* ARUSER	*/
			  (0 <<  2) |						/* ARPROT	*/
			  (0 <<  1));						/* byte swap	*/
	desc[1].hrdmash = 0;							/* start addr high */
	desc[1].hrdmas = ((unsigned int)rsa_public_key);			/* start addr low  */
	desc[1].hrdmal = keysize;

	desc[2].hrdmac = ((0 << 14) |						/* burst length */
			  (0 <<  9) |						/* ARUSER	*/
			  (0 <<  2) |						/* ARPROT	*/
			  (0 <<  1));						/* byte swap	*/
	desc[2].hrdmash = 0;							/* start addr high */
	desc[2].hrdmas = ((unsigned int)pbm->bi.load_addr);			/* start addr low  */
	desc[2].hrdmal = bsize;

	g_crypto->get_hash(desc, 3, (unsigned int *)phash,
					(hsize + keysize + bsize));
//	NOTICE("Hash Generated!! \r\n");

	return;
}

int authenticate_image(unsigned char *pbootkey,
	unsigned char *phash, unsigned char *psign)
{
	st_rsa_pubkey	st_rsa_pubkey;
	st_octet_string	st_digest;
	st_rsa_sign	st_sign;
	int		ret;

	unsigned char __attribute__ ((aligned(4))) e_value[4] =
		{0x00, 0x00, 0x00, 0x03};

	st_digest.u32_data_byte_len = 32;
	st_digest.pu08_data = phash;

	st_rsa_pubkey.u32_key_type = 0xFFFFFFFF;
	st_rsa_pubkey.st_bignum_n.u32_data_byte_len = 256;
	st_rsa_pubkey.st_bignum_n.pu08_data = pbootkey;
	st_rsa_pubkey.st_bignum_e.u32_data_byte_len = 4;
	st_rsa_pubkey.st_bignum_e.pu08_data = e_value;

	st_sign.u32_mgf_hash_oid = OID_SHA2_256;
	st_sign.st_signature.pu08_data = psign;
	st_sign.st_signature.u32_data_byte_len = 256;
	st_sign.u32_salt_blen = 32;

	ret = g_bl1_fn->crypt_fn.sss_rsassa_pss_verify_digest(
			&st_rsa_pubkey, &st_digest, &st_sign);

	if (ret != SSSR_SUCCESS) {
		ERROR("\r\nImage has been modified. %08X\r\n", ret);
		return -1;
	}

	NOTICE("\r\n Image Signature is Valid. \r\n");

	return 0;
}

int authenticate_bimage(struct nx_bootmanager *pbm,
	unsigned char *rsa_public_key, unsigned int verify_enb)
{
	unsigned int hash[32/4];
	int ret = 0;

	if (verify_enb) {
		/* @brief: generate the hash data (input: header + original image) */
		bimage_generate_hash(pbm, rsa_public_key, (unsigned char*)hash);

		/*
		 * @brief: perform authentication procedures
		 * for integrity on file-images. (input: header + original image)
		 */
		ret = authenticate_image(
			(unsigned char*)rsa_public_key,
			(unsigned char*)hash,
			(unsigned char*)pbm->rsa_encrypted_sha256_hash);

		/* @brief: software reset the sss controller */
		sssc_reset(TRUE);
	}

	return ret;
}
