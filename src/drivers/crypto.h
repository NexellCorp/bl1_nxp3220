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
#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#define SSSR_SUCCESS		(0x00000000)
#define SSSR_FAIL		(0xFFFFFFFF)

#define OID_SHA1_160		(0x00001100)
#define OID_SHA2_256		(0x00002300)

typedef struct
{
	unsigned int	u32_data_byte_len;	//! byte length of Data
	unsigned char	*pu08_data;	//! byte array of Data
} st_octet_string;

typedef struct
{
	unsigned int	u32_key_type;
	st_octet_string  st_bignum_n;
	st_octet_string  st_bignum_e;
} st_rsa_pubkey;

typedef struct
{
	unsigned int	u32_mgf_hash_oid; //! Hash OID for MGF
	st_octet_string	st_signature;    //! RSA Signature
	unsigned int	u32_salt_blen;   //! Salt_byte_len for PSS padding
} st_rsa_sign;

#endif /* #ifndef __CRYPTO_H__ */
