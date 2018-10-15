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

/* Mailbox Hardware */
struct mb_reg {
	volatile unsigned int mb_status;		/* 000 */
	volatile unsigned int cm0_hreset;		/* 004 */
	volatile unsigned int cm0_sram_access_control;	/* 008 */
	volatile unsigned int _rev0;			/* 00C */
	volatile unsigned int lockup_sysrstreq_con;	/* 010 */
	volatile unsigned int cm0_fw_status;		/* 014 */
	volatile unsigned int _rev1[2];			/* 018 */
	volatile unsigned int mbintstat;		/* 020 */
	volatile unsigned int mbintenset;		/* 024 */
	volatile unsigned int mbintenclr;		/* 028 */
	volatile unsigned int mbintpend;		/* 02C */
	volatile unsigned int cryptoe_status;		/* 030 */
	volatile unsigned int cm0_debug_control;	/* 034 */
	volatile unsigned int cm0_anti_rbcnt;		/* 038 */
	volatile unsigned int _rev2;			/* 03C */
	volatile unsigned int cm0_secure_boot_key[16];	/* 040 */
	volatile unsigned int idma_mode_sel;		/* 080 */
	volatile unsigned int _rev3[3];			/* 084 */
	volatile unsigned int idma_rx_addr;		/* 090 */
	volatile unsigned int idma_rx_length;		/* 094 */
	volatile unsigned int idma_rx_control;		/* 098 */
	volatile unsigned int idma_rx_status;		/* 09C */
	volatile unsigned int idma_tx_addr;		/* 0A0 */
	volatile unsigned int idma_tx_length;		/* 0A4 */
	volatile unsigned int idma_tx_control;		/* 0A8 */
	volatile unsigned int idma_tx_status;		/* 0AC */
	volatile unsigned int _rev4[20];		/* 0B0 */
	volatile unsigned int mb_data[64];		/* 100 */
	volatile unsigned char _rev10[0x400 - 128 * 4];
};

/* Key Manager Engine Hardware */
struct kmr_reg {
	volatile unsigned int km_command;		/* 000 */
	volatile unsigned int km_status;		/* 004 */
	volatile unsigned int km_ctrl;			/* 008 */
	volatile unsigned int km_key_init;		/* 00C */
	volatile unsigned int km_intstat;		/* 010 */
	volatile unsigned int km_intenset;		/* 014 */
	volatile unsigned int km_intenclr;		/* 018 */
	volatile unsigned int km_intenpend;		/* 01C */
	volatile unsigned int _rev0[24];		/* 020 */
	volatile unsigned int bk_data[16];		/* 080 */
	volatile unsigned int kdf_input[15];		/* 100 */
	volatile unsigned int _rev1;			/* 13C */
	volatile unsigned int kdf_input_length;		/* 140 */
	volatile unsigned int _rev2[47];		/* 144 */
	volatile unsigned int km_bek_control;		/* 200 */
	volatile unsigned char _rev10[0x400 - 129 * 4];
};


/* Feeder Hardware */
struct feeder_reg {
	volatile unsigned int intstat;		/* 000 */
	volatile unsigned int intenset;		/* 004 */
	volatile unsigned int intenclr;		/* 008 */
	volatile unsigned int intpend;		/* 00C */
	volatile unsigned int fifostat;		/* 010 */
	volatile unsigned int fifoctrl;		/* 014 */
	volatile unsigned int global;		/* 018 */
	volatile unsigned int cm0_lp_con;	/* 01C */
	volatile unsigned int brdmas;		/* 020 */
	volatile unsigned int brdmal;		/* 024 */
	volatile unsigned int brdmac;		/* 028 */
	volatile unsigned int brdmash;		/* 02C */
	volatile unsigned int btdmas;		/* 030 */
	volatile unsigned int btdmal;		/* 034 */
	volatile unsigned int btdmac;		/* 038 */
	volatile unsigned int btdmash;		/* 03C */
	volatile unsigned int hrdmas;		/* 040 */
	volatile unsigned int hrdmal;		/* 044 */
	volatile unsigned int hrdmac;		/* 048 */
	volatile unsigned int hrdmash;		/* 04C */
	volatile unsigned int brdmastat;	/* 050 */
	volatile unsigned int btdmastat;	/* 054 */
	volatile unsigned int hrdmastat;	/* 058 */
	volatile unsigned int bdrdmastat;	/* 05C */
	volatile unsigned int hdrdmastat;	/* 060 */
	volatile unsigned int _rev0[3];		/* 064 */
	volatile unsigned int sss_version;	/* 070 */
	volatile unsigned int _rev1[3];		/* 074 */
	volatile unsigned int bdrdmas;		/* 080 */
	volatile unsigned int bdrdmal;		/* 084 */
	volatile unsigned int bdrdmac;		/* 088 */
	volatile unsigned int bdrdmash;		/* 08C */
	volatile unsigned int hdrdmas;		/* 090 */
	volatile unsigned int hdrdmal;		/* 094 */
	volatile unsigned int hdrdmac;		/* 098 */
	volatile unsigned int hdrdmash;		/* 09C */
	volatile unsigned int ddmactrl;		/* 0A0 */
	volatile unsigned int ddmastatus;	/* 0A4 */
	volatile unsigned char _rev10[0x400 - 42 * 4];
};

struct aes_reg {
	volatile unsigned int control;		/* 000 */
	volatile unsigned int status;		/* 004 */
	volatile unsigned int sizelow;		/* 008 */
	volatile unsigned int sizehigh;		/* 00C */
	volatile unsigned int indata[4];	/* 010 */
	volatile unsigned int outdata[4];	/* 020 */
	volatile unsigned int ivdata[4];	/* 030 */
	volatile unsigned int cntdata[4];	/* 040 */
	volatile unsigned int _rev0[4];		/* 050 */
	volatile unsigned int keydatae[8];	/* 060 */
	volatile unsigned int keydata[8];	/* 080 */
	volatile unsigned char _rev1[0x400 - 40 * 4];
};

struct tdes_reg {
	volatile unsigned int conf;		/* 000 */
	volatile unsigned int stat;		/* 004 */
	volatile unsigned int _rev0[2];
	volatile unsigned int key[2][3];	/* 010 */
	volatile unsigned int iv[2];		/* 028 */
	volatile unsigned int input[2];		/* 030 */
	volatile unsigned int output[2];	/* 038 */
	volatile unsigned char _rev10[0x400 - 16 * 4];
};

struct rc4_reg {
	volatile unsigned int control;		/* 000 */
	volatile unsigned int byte_swap;	/* 004 */
	volatile unsigned int key_len;		/* 008 */
	volatile unsigned int status;		/* 00C */
	volatile unsigned int msg_size_low;	/* 010 */
	volatile unsigned int msg_size_high;	/* 014 */
	volatile unsigned int _rev0[2];		/* 018 */
	volatile unsigned int data_in;		/* 020 */
	volatile unsigned int data_out;		/* 024 */
	volatile unsigned int _rev1[2];		/* 028 */
	volatile unsigned int key[4];		/* 030 */
	volatile unsigned char _rev10[0x400 - 16 * 4];
};

struct hash_reg {
	volatile unsigned int control[2];		/* 000 */
	volatile unsigned int fifo_mode;		/* 008 */
	volatile unsigned int byte_swap;		/* 00C */
	volatile unsigned int status;			/* 010 */
	volatile unsigned int seed_config;		/* 014 */
	volatile unsigned int _rev0[2];			/* 018 */
	volatile unsigned int msg_size_low;		/* 020 */
	volatile unsigned int msg_size_high;		/* 024 */
	volatile unsigned int msg_size_ext_low;		/* 028 */
	volatile unsigned int msg_size_ext_high;	/* 02C */
	volatile unsigned int pre_msg_size_low;		/* 030 */
	volatile unsigned int pre_msg_size_high;	/* 034 */
	volatile unsigned int pre_msg_size_ext_low;	/* 038 */
	volatile unsigned int pre_msg_size_ext_high;	/* 03C */
	volatile unsigned int data_in[42];		/* 040 */
	volatile unsigned int _rev1[2];			/* 0E8 */
	volatile unsigned int key_inh[4];		/* 0F0 */
	volatile unsigned int key_inl[32];		/* 100 */
	volatile unsigned int iv[16];			/* 180 */
	volatile unsigned int result[16];		/* 1C0 */
	volatile unsigned int reseed_counter;		/* 200 */
	volatile unsigned int entropy[16];		/* 204 */
	volatile unsigned int nonce[8];			/* 244 */
	volatile unsigned int prng[8];			/* 264 */
	volatile unsigned int prng_v[14];		/* 284 */
	volatile unsigned int prng_c[14];		/* 2BC */
	volatile unsigned int reseed_status;		/* 2F4 */
	volatile unsigned int prng_test_con;		/* 2F8 */
	volatile unsigned char _rev10[0x400 - 191 * 4];
};

struct trng_reg {
	volatile unsigned int clkdiv;		/* 000 */
	volatile unsigned int _rev0[3];		/* 004 */
	volatile unsigned int aen;		/* 010 */
	volatile unsigned int _rev1[3];		/* 014 */
	volatile unsigned int ctrl;		/* 020 */
	volatile unsigned int _rev2[3];		/* 024 */
	volatile unsigned int post_ctrl;	/* 030 */
	volatile unsigned int _rev3[3];		/* 034 */
	volatile unsigned int test_ctrl;	/* 040 */
	volatile unsigned int test_stat;	/* 044 */
	volatile unsigned int ht_cutoff;	/* 048 */
	volatile unsigned int startup_ctrl;	/* 04C */
	volatile unsigned int fifo_ctrl;	/* 050 */
	volatile unsigned int bt_threshold;	/* 054 */
	volatile unsigned int ot_threshold;	/* 058 */
	volatile unsigned int mb_threshold;	/* 05C */
	volatile unsigned int test_done;	/* 060 */
	volatile unsigned int _rev4[7];		/* 064 */
	volatile unsigned int fifo[8];		/* 080 */
	volatile unsigned char _rev10[0x400 - 40 * 4];
};

struct pka_reg {
	volatile unsigned int tcr[6];			/* 000 */
	volatile unsigned char _rev10[0x400 - 6 * 4];
};

struct wdt_reg {
	volatile unsigned int wt_con;			/* 000 */
	volatile unsigned int wt_dat;			/* 004 */
	volatile unsigned int wt_cnt;			/* 008 */
	volatile unsigned int wt_clrint;		/* 00C */
	volatile unsigned char _rev10[0x200 - 4 * 4];
};

struct sss_reg {
	struct feeder_reg feeder;				/* 0x0000 Feeder*/
	struct aes_reg aes;					/* 0x0400 AES	*/
	struct tdes_reg tdes;					/* 0x0800 TDES	*/
	struct rc4_reg arc4;					/* 0x0C00 ARC4	*/
	struct hash_reg hash;					/* 0x1000 HASH	*/
	struct trng_reg trng;					/* 0x1400 TRNG	*/
	struct wdt_reg wdt[2];					/* 0x1800 Secure WDT */
	unsigned char _rev0[0x4000 - 0x1800 - 0x0400];
	struct pka_reg pka;					/* 0x4000 PKA */
	volatile unsigned char pka_reg_sram[2880];		/* 0x4400 2880 bytes */
	unsigned char _rev1[0x8000 - 0x4400 - 2880];
	union {
		volatile unsigned char b_cm0sram[32768];	/* 0x8000 32KB */
		volatile unsigned int w_cm0sram[32768 / 4];
	};
	volatile unsigned char sramdata[4];			/* 0x10000 */
};

struct skm_reg {
	volatile unsigned int ctrl;
	volatile unsigned int mode;
};


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

#define AES_ENC				(0 << 0)
#define AES_DEC				(1 << 0)
#define AES_ECB				(0 << 1)
#define AES_CBC				(1 << 1)
#define AES_CTR				(2 << 1)
#define AES_XTS				(3 << 1)
#define AES_CMAC			(1 << 14 | 0 << 1)
#define AES_CCM				(1 << 14 | 1 << 1)
#define AES_GCM				(1 << 14 | 2 << 1)
#define AES_CHAIN_MODE			(1 << 14 | 3 << 1)
#define AES_FIFO			(1 << 3)
#define AES_CPU				(0 << 3)
#define AES_128				(0 << 4)
#define AES_192				(1 << 4)
#define AES_256				(2 << 4)
#define AES_KEY_SIZE			(3 << 4)
#define AES_NOSWAP			(0 << 7)
#define AES_SWAP			(0x1F <<7)
#define AES_SWAP_KEY			(1 << 7)
#define AES_SWAP_CNT			(1 << 8)
#define AES_SWAP_IV			(1 << 9)
#define AES_SWAP_DO			(1 << 10)
#define AES_SWAP_DI			(1 << 11)
#define AES_CNT_128			(0 << 12)
#define AES_CNT_64			(1 << 12)
#define AES_CNT_32			(2 << 12)
#define AES_CNT_16			(3 << 12)
#define AES_CMACOUT_FIFO		(1 << 15)
#define AES_CCM_SIZE_L			(7 << 16)
#define CCM_QLEN_2B			(1 << 16)
#define CCM_QLEN_3B			(2 << 16)
#define CCM_QLEN_4B			(3 << 16)
#define CCM_QLEN_5B			(4 << 16)
#define CCM_QLEN_6B			(5 << 16)
#define CCM_QLEN_7B			(6 << 16)
#define CCM_QLEN_8B			(7 << 16)
#define AES_CCM_SIZE_T			(7 << 19)
#define CCM_TAG_4B			(1 << 19)
#define CCM_TAG_6B			(2 << 19)
#define CCM_TAG_8B			(3 << 19)
#define CCM_TAG_10B			(4 << 19)
#define CCM_TAG_12B			(5 << 19)
#define CCM_TAG_14B			(6 << 19)
#define CCM_TAG_16B			(7 << 19)

#define AES_OUT_READY			(1 << 0)
#define AES_IN_READY			(1 << 1)
#define AES_BUSY			(1 << 2)
#define AES_MAC_DONE			(1 << 3)

#define HASH_BUFFER_READY		(1 << 0)
#define HASH_SEED_SETTING_DONE		(1 << 1)
#define HASH_PRNG_BUSY			(1 << 2)
#define RESEED_ERROR			(1 << 3)
#define HASH_PARTIAL_DONE		(1 << 4)
#define GENERATE_ERROR			(1 << 5)
#define HASH_MSG_DONE			(1 << 6)
#define HASH_INSTANTIATE_ERROR		(1 << 7)
#define TRNG_SEED_DONE			(1 << 8)
#define TRNG_FSM_WORKING		(1 << 9)
#define PRNG_INSTANTIATE_DONE		(1 << 10)
#define PRNG_RESEED_DONE		(1 << 11)
#define PRNG_GENERATE_DONE		(1 << 12)
#define PRNG_RESEED_COUNT_EXCEED	(1 << 13)

#define HASH_ENGINE_SELECTION		(0xF)
#define ENG_SHA1_HASH			(0x0)
#define ENG_SHA1_HMAC			(0x1)
#define ENG_MD5_HASH			(0x2)
#define ENG_MD5_HMAC			(0x3)
#define ENG_SHA256_HASH			(0x4)
#define ENG_SHA256_HMAC			(0x5)
#define ENG_SHA384_HASH			(0x8)
#define ENG_SHA384_HMAC			(0x9)
#define ENG_SHA512_HASH			(0xa)
#define ENG_SHA512_HMAC			(0xb)
#define ENG_PRNG			(0xF)
#define START_BIT	 		(1 << 4)
#define USER_IV_EN	 		(1 << 5)

#define AESSEL				(0 << 2)
#define DESSEL				(1 << 2)
#define RC4SEL				(2 << 2)
#define BBPSEL				(3 << 2)
#define HASHINSEL			(3 << 0)
#define HASH_ALONE			(0 << 0)	// Hash	input from HRDMA
#define HASH_BC_IN			(1 << 0)	// Hash	input from block cipher	input
#define HASH_BC_OUT			(2 << 0)	// Hash	input from block cipher	output

#define FCBYTESWAP			(1 << 1)
#define BURST_1				(0 << 14)
#define BURST_2				(1 << 14)
#define BURST_4				(2 << 14)
#define BURST_8				(3 << 14)
#define BURST_16			(4 << 14)

#define WAKEUP_INT			(1 << 12)
#define WFI_INT				(1 << 11)
#define BRDMAINT			(1 << 3)
#define BTDMAINT			(1 << 2)
#define HRDMAINT			(1 << 1)
#define PKDMAINT			(1 << 0)

#define BDDMA_DONE			(1 << 16)
#define BDRDMA_DONE			(1 << 15)
#define HDDMA_DONE			(1 << 14)
#define HDRDMA_DONE			(1 << 13)

#define CMAC_DONE			(1 << 10)
#define BTEDMA_DONE			(1 << 9)
#define RNG_ERROR			(1 << 8)
#define PARTIAL_DONE			(1 << 7)
#define PRNG_DONE			(1 << 6)
#define MSG_DONE 			(1 << 5)
#define PRNG_ERROR			(1 << 4)
#define BRDMA_DONE			(1 << 3)
#define BTDMA_DONE			(1 << 2)
#define HRDMA_DONE			(1 << 1)
#define PKA_DONE			(1 << 0)


/* Function Define */
 void sssc_reset(unsigned int enable);
 int sss_load(struct nx_bootmanager *pbm, unsigned int option);

#endif /* #ifndef __SSS_H__ */
