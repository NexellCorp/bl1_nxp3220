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
#ifndef __BL0_FNPTR_H__
#define __BL0_FNPTR_H__

#include <usbboot.h>
#include <xyz_modem.h>
#include <sdxcboot.h>
#include <nandboot.h>
#include <sss.h>
#include <crypto.h>

typedef struct
{
    int nr;									/*!<  number of rounds  */
    unsigned int *rk;								/*!<  AES round keys    */
    unsigned int buf[68];							/*!<  unaligned data    */
} mbedtls_aes_context;

typedef struct
{
	unsigned int total[2];							/*!< number of bytes processed  */
	unsigned int state[8];							/*!< intermediate digest state  */
	unsigned char buffer[64];						/*!< data block being processed */
	int is224;								/*!< 0 => SHA-256, else SHA-224 */
} mbedtls_sha256_context;


struct nx_mbedtls_fnptr {
	unsigned int (*aes_cbc_decrypt_bimage)(unsigned int *src,
		unsigned int *dst, unsigned int iv[], unsigned int size);
};
#include <sysheader.h>
#include <usbboot.h>
struct nx_usb_fnptr {
	const unsigned char (*gs_devdesc_fs)[];
	const unsigned char (*gs_devdesc_hs)[];
	const unsigned char (*gs_config_desc_fs)[];
	const unsigned char (*gs_config_desc_hs)[];
	void (*usb_write_in_fifo)(unsigned int, unsigned char *, int);
	void (*usb_read_out_fifo)(unsigned int, unsigned char *, int);
	void (*usb_ep0_int_hndlr)(usbboot_status *);
	void (*usb_transfer_ep0)(usbboot_status *);
	void (*usb_int_bulkin)(usbboot_status *);
//	void (*usb_int_bulkout)(usbboot_status *, unsigned int, unsigned int);
	void (*usb_int_bulkout)(usbboot_status *, struct nx_bootmanager *, unsigned int);

	void (*usb_reset)(usbboot_status *);
	 int (*usb_set_init)(usbboot_status *);
	void (*usb_pkt_receive)(usbboot_status *, unsigned int);
	void (*usb_transfer)(usbboot_status *);
	void (*udc_int_hndlr)(usbboot_status *, unsigned int);
	 int (*usbboot)(unsigned int);
};

struct nx_sdmmc_fnptr {
	int (*sdmmc_setclock)(sdxcboot_status *, int, unsigned int);
	unsigned int (*sdmmc_sendcommandinternal)(sdxcboot_status *, sdmmc_command *);
	unsigned int (*sdmmc_sendstatus)(sdxcboot_status *);
	unsigned int (*sdmmc_sendcommand)(sdxcboot_status *, sdmmc_command *);
	unsigned int (*sdmmc_sendappcommand)(sdxcboot_status *, sdmmc_command *);
	int (*sdmmc_identifycard)(sdxcboot_status *);
	int (*sdmmc_selectcard)(sdxcboot_status *);
	int (*sdmmc_setcarddetectpullup)(sdxcboot_status *, int);
	int (*sdmmc_setbuswidth)(sdxcboot_status *, unsigned int);
	int (*sdmmc_setblocklength)(sdxcboot_status *, unsigned int);
	int (*sdmmc_init)(sdxcboot_status *);
	int (*sdmmc_terminate)(sdxcboot_status *);
	int (*sdmmc_open)(sdxcboot_status *, unsigned int);
	int (*sdmmc_close)(sdxcboot_status *);
	int (*sdmmc_read_sectordata)(sdxcboot_status *, unsigned int, unsigned int *);
	int (*sdmmc_read_sectors)(sdxcboot_status *, unsigned int, unsigned int, unsigned int *);
	int (*sdmmc_readbootsector)(sdxcboot_status *, unsigned int, unsigned int *);
	int (*emmcboot_read)(sdxcboot_status *, unsigned int);
	int (*emmcboot_normal)(sdxcboot_status *, unsigned int);
	int (*emmcboot_alt)(sdxcboot_status *, unsigned int);
	int (*sdmmcboot)(sdxcboot_status *, unsigned int);
	void (*sdpad_setalt)(unsigned int);
	void (*sdpad_setgpio)(unsigned int);
	unsigned int (*sdxcboot)(unsigned int);
	int (*emmcboot)(unsigned int);
};

struct nx_sdfs_fnptr {
	int reserved0[17];
	int (*fsboot)(sdxcboot_status *, unsigned int);
	int (*sdmmc_fsboot)(sdxcboot_status *, unsigned int);
	unsigned int (*sdxcfsboot)(unsigned int);
};

struct nx_spi_fntptr {
	void (*spipad_setalt)(unsigned int);
	void (*spipad_setdealt)(unsigned int);
	void (*spi_init)(unsigned int);
	void (*spi_deinit)(unsigned int);
	void (*read_flash)(int, unsigned char *, unsigned int, int, int);
	unsigned int (*spiboot)(unsigned int);
};

struct nx_uart_fnptr {
	void (*serial_init)(unsigned int);
	void (*serial_deinit)(unsigned int);
	char (*get_ch)(int);
	void (*put_ch)(int, char);
	 int (*is_rx_ready)(int);
	 int reserved1[5];
	 int (*xyzModem_getc)(int);
	void (*xyzModem_flush)(int);
	 int (*xyzModem_get_hdr)(xyzsts *);
	 int (*xyzModem_stream_open)(connection_info_t*, xyzsts*, int*);
	 int (*xyzModem_stream_read)(xyzsts*, char*, int, int*);
	void (*xyzModem_stream_close)(int*);
	void (*xyzModem_stream_terminate)(xyzsts *, bool, int (*getc)(int));
	 int (*xyzModem_error)(int);
	 int rsv1;
};

struct nx_nand_fnptr {
	void (*nandc_init)(struct nandbootinfo *, unsigned int);
	void (*nandc_deinit)(void);
	 int (*nand_reset)(void);
	 int (*nand_read)(struct nandbootinfo*, void*, int);
	 int (*nandboot)(unsigned int);
};

struct nx_crypto_fntpr {
	void (*get_hash)(struct nx_hrdmadesc*, int, unsigned int *, int);
	void (*get_keyhash)(unsigned int *);
	void (*aes_decrypt)(struct nx_brtdmadesc*, int, unsigned int *);
	unsigned int (*sss_rsassa_pss_verify_digest)(
			const st_rsa_pubkey	*pst_rsa_pubkey,
			const st_octet_string	*pst_digest,
			const st_rsa_sign	*pst_rsa_sign);
	 int (*header_check)(unsigned int iv[], int);
	 int (*key_check)(unsigned int iv[], int, int);
	void (*image_decrypt)(unsigned int iv[]);
};

struct nx_verific_fntpr {
//	int reserved0[10];//emul
	void (*get_image_hash)(unsigned int *);
	int (*authenticate_image)(unsigned char* , unsigned char *,
		       unsigned char *);
};

struct nx_gpt_fntpr {
	const unsigned int (*gpt_crc_table)[];
	unsigned int (*crc32_no_comp)(unsigned int crc,
	               const unsigned char *buf, unsigned int len);
	unsigned int (*crc32)(unsigned int crc,
	               const unsigned char *p, unsigned int len);
	int (*is_gpt_valid)(unsigned char *psector);
	unsigned int (*get_first_lba)(unsigned char *psector);
	unsigned int (*get_first_empty_lba)(unsigned char *psector);
	unsigned int (*is_gpt_part)(unsigned char *mbr);
};

struct nx_lib_fnptr {
	void (*printchar)(char **, int);
	int (*prints)(char **, const char *, int, int);
	int (*printi)(char **, int, int, int, int, int, int);
	int (*print)(char **, const char *, va_list);
	int (*printf)(const char *, ...);
	int reserved0[4];
	void *(*memcpy)(void *dest, const void *src, size_t n);
	void *(*memset)(void *str, int c, size_t n);
	int (*memcmp)(const void* s1, const void* s2, size_t n);

	void (*block_rev)(unsigned int *dst, unsigned int *src, unsigned int size);
	void (*sss_swap_byte)(unsigned int *dst, unsigned int *src, unsigned int size);
	void (*sss_swap_dword)(unsigned int *dst, unsigned int *src, unsigned int size);
};

struct nx_bl0_fnptr {
	int reserved0;
	struct nx_lib_fnptr lib_fn;
	int reserved1[15];

	struct nx_crypto_fntpr crypt_fn;
	struct nx_verific_fntpr ver_fn;
	struct nx_nand_fnptr nand_fn;
	struct nx_usb_fnptr usb_fn;
	struct nx_gpt_fntpr gpt_fn;
	struct nx_sdmmc_fnptr sdmmc_fn;
	struct nx_sdfs_fnptr sdfs_fn;
	struct nx_spi_fntptr spi_fn;
	struct nx_uart_fnptr uart_fn;

	const unsigned int (*iv)[];
	const unsigned int (*(*pbootkeyhash))[4][8];
	unsigned int (*romboot)(unsigned int);

	struct nx_mbedtls_fnptr mbedtls_fn;
};

#endif /* #ifndef __BL0_FNPTR_H__ */
