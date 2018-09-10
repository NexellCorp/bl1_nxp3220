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
#ifndef __SDXCBOOT_H__
#define __SDXCBOOT_H__

#define SDXC_CLKSRC_ID			(12)	/* PLL12 = 24 MHz */
#define	SDXC_DIVIDER_400KHZ		(15)	/* PLL12 / 15 / 4 = 400KHz */

#define SDXC_CLKSRC_REAL		(7)	/* PLL7 = 200 MHz */
#define SDXC_CLKDIV_LOW			(2)	/* PLL7 / 2 / 4 = 25 MHz */
#define SDXC_CLKDIV_HIGH		(1)	/* PLL7 / 1 / 4 = 50 MHz */

#define SDMMC_BLOCK_LENGTH		(0x200)

#define SDMMC_TIMEOUT			(0x100000)
#define SDMMC_TIMEOUT_IDENTIFY		(0x100000)

#define	SDMMC_STATUS_NOERROR		0
#define	SDMMC_STATUS_ERROR		(1U << 31)
#define	SDMMC_STATUS_CMDBUSY		(SDMMC_STATUS_ERROR | (1U << 0))
#define	SDMMC_STATUS_CMDTOUT		(SDMMC_STATUS_ERROR | (1U << 1))
#define	SDMMC_STATUS_RESCRCFAIL		(SDMMC_STATUS_ERROR | (1U << 2))
#define	SDMMC_STATUS_RESERROR		(SDMMC_STATUS_ERROR | (1U << 3))
#define	SDMMC_STATUS_RESTOUT		(SDMMC_STATUS_ERROR | (1U << 4))
#define SDMMC_STATUS_UNKNOWNCMD		(SDMMC_STATUS_ERROR | (1U << 5))
#define	SDMMC_STATUS_DATABUSY		(SDMMC_STATUS_ERROR | (1U << 6))

#define	SDMMC_RSPIDX_NONE		(0)
#define	SDMMC_RSPIDX_R1			(1)
#define	SDMMC_RSPIDX_R1B		(1 | 0x80)
#define	SDMMC_RSPIDX_R2			(2)
#define	SDMMC_RSPIDX_R3			(3)
#define	SDMMC_RSPIDX_R4			(4)
#define	SDMMC_RSPIDX_R5			(5)
#define	SDMMC_RSPIDX_R6			(6)
#define	SDMMC_RSPIDX_R7			(7)

/* Command */
#define GO_IDLE_STATE		(( 0) | ((SDMMC_RSPIDX_NONE	) << 8))
#define SEND_OP_COND		(( 1) | ((SDMMC_RSPIDX_R3	) << 8))	/* for MMC */
#define ALL_SEND_CID		(( 2) | ((SDMMC_RSPIDX_R2	) << 8))
#define SET_RELATIVE_ADDR	(( 3) | ((SDMMC_RSPIDX_R1	) << 8))	/* for MMC */
#define SEND_RELATIVE_ADDR	(( 3) | ((SDMMC_RSPIDX_R6	) << 8))	/* for SD */
#define	SWITCH_FUNC		(( 6) | ((SDMMC_RSPIDX_R1B	) << 8))	/* for MMC */
#define SELECT_CARD		(( 7) | ((SDMMC_RSPIDX_R1B	) << 8))
#define SEND_IF_COND		(( 8) | ((SDMMC_RSPIDX_R7	) << 8))	/* for SD */
#define SEND_EXT_CSD		(( 8) | ((SDMMC_RSPIDX_R1	) << 8))	/* for MMC */
#define SEND_CSD		(( 9) | ((SDMMC_RSPIDX_R2	) << 8))
#define SEND_CID		((10) | ((SDMMC_RSPIDX_R2	) << 8))
#define STOP_TRANSMISSION	((12) | ((SDMMC_RSPIDX_R1B	) << 8))
#define SEND_STATUS		((13) | ((SDMMC_RSPIDX_R1	) << 8))
#define SET_BLOCKLEN		((16) | ((SDMMC_RSPIDX_R1	) << 8))
#define READ_SINGLE_BLOCK	((17) | ((SDMMC_RSPIDX_R1	) << 8))
#define READ_MULTIPLE_BLOCK	((18) | ((SDMMC_RSPIDX_R1	) << 8))
#define WRITE_BLOCK		((24) | ((SDMMC_RSPIDX_R1	) << 8))
#define WRITE_MULTIPLE_BLOCK	((25) | ((SDMMC_RSPIDX_R1	) << 8))
#define SELECT_PARTITION	((43) | ((SDMMC_RSPIDX_R1B	) << 8))	/* for eSD */
#define APP_CMD			((55) | ((SDMMC_RSPIDX_R1	) << 8))

/* Application Command */
#define SET_BUS_WIDTH		(( 6) | ((SDMMC_RSPIDX_R1	) << 8) | (APP_CMD << 16))
#define SD_STATUS		((13) | ((SDMMC_RSPIDX_R1	) << 8) | (APP_CMD << 16))
#define	SD_SEND_OP_COND		((41) | ((SDMMC_RSPIDX_R3	) << 8) | (APP_CMD << 16))
#define	SET_CLR_CARD_DETECT	((42) | ((SDMMC_RSPIDX_R1	) << 8) | (APP_CMD << 16))
#define SEND_SCR		((51) | ((SDMMC_RSPIDX_R1	) << 8) | (APP_CMD << 16))

/* EXT_CSD Fields */
#define EXT_CSD_PARTITIONING_SUPPORT		160				/* RO */
#define EXT_CSD_ERASE_GROUP_DEF			175				/* R/W */
#define EXT_CSD_PART_CONF			179				/* R/W */
#define EXT_CSD_BUS_WIDTH			183				/* R/W */
#define EXT_CSD_HS_TIMING			185				/* R/W */
#define EXT_CSD_REV				192				/* RO */
#define EXT_CSD_CARD_TYPE			196				/* RO */
#define EXT_CSD_SEC_CNT				212				/* RO, 4 bytes */
#define EXT_CSD_HC_ERASE_GRP_SIZE		224				/* RO */
#define EXT_CSD_BOOT_MULT			226				/* RO */

#define EXT_CSD_BOOT_CONFIG 			179				/* R/W */
#define EXT_CSD_BOOT_BUS_WIDTH  		177				/* R/W */

/* EXT_CSD field definitions */
#define EXT_CSD_CMD_SET_NORMAL			(1 << 0)
#define EXT_CSD_CMD_SET_SECURE			(1 << 1)
#define EXT_CSD_CMD_SET_CPSECURE		(1 << 2)

#define EXT_CSD_CARD_TYPE_26			(1  <<  0)			/* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52			(1  <<  1)			/* Card can run at 52MHz */

#define EXT_CSD_BUS_WIDTH_1			0				/* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4			1				/* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8			2				/* Card is in 8 bit mode */

#define EXT_CSD_NO_BOOT				(0 << 6)
//#define EXT_CSD_BOOT_ACK			(1 << 6)

#define EXT_CSD_BOOT_PARTITION_NOT_ENABLE	(0x0 << 3)
#define EXT_CSD_BOOT_PARTITION_1_ENABLE		(0x1 << 3)
#define EXT_CSD_BOOT_PARTITION_2_ENABLE		(0x2 << 3)

#define EXT_CSD_BOOT_PARTITION_NO_ACCESS	(0x0)
#define EXT_CSD_BOOT_PARTITION_1_ACCESS		(0x1)
#define EXT_CSD_BOOT_PARTITION_2_ACCESS		(0x2)

#define EXT_CSD_BOOT_ACK(x)			(x << 6)
#define EXT_CSD_BOOT_PART_NUM(x)		(x << 3)
#define EXT_CSD_PARTITION_ACCESS(x)		(x << 0)

#define MMC_SWITCH_MODE_CMD_SET			0x00				/* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS		0x01 				/* Set bits in EXT_CSD byte addressed by index which area1 in value field */
#define MMC_SWITCH_MODE_CLEAR_BITS		0x02				/* Clear bits in EXT_CSD byte addressed by index, which are 1 in value field */
#define MMC_SWITCH_MODE_WRITE_BYTE		0x03				/* Set target byte to value */


typedef struct tag_sdmmc_cmd
{
	unsigned int	cmdidx;
	unsigned int	arg;
	unsigned int	flag;
	unsigned int	status;
	unsigned int	response[4];
} sdmmc_command;

typedef enum
{
	SDMMC_CARDTYPE_MMC,
	SDMMC_CARDTYPE_SDMEM,
	SDMMC_CARDTYPE_SDIO,
	SDMMC_CARDTYPE_UNKNOWN
} SDMMC_CARDTYPE;

typedef struct __attribute__ ((aligned(4))) tag_sdxc_boot_st
{
	unsigned int		rca;	/* relative card address of device */
	int			bhigh_capacity;
	unsigned int		sd_port;
	int			bhigh_speed;
	SDMMC_CARDTYPE		card_type;
} sdxcboot_status;

#define SDXC_CTRL_USEINDMAC			(1U << 25)
#define SDXC_CTRL_READWAIT			(1U <<	6)
#define SDXC_CTRL_DMAMODE_EN 			(1U <<	5)
#define SDXC_CTRL_DMARST			(1U <<	2)
#define SDXC_CTRL_FIFORST			(1U <<	1)
#define SDXC_CTRL_CTRLRST			(1U <<	0)

#define SDXC_CLKENA_LOWPWR			(1U << 16)
#define SDXC_CLKENA_CLKENB			(1U <<	0)

#define SDXC_STATUS_FIFOCOUNT			(0x1FFFU << 17)
#define SDXC_STATUS_FSMBUSY			(1U << 10)
#define SDXC_STATUS_DATABUSY 			(1U <<	9)
#define SDXC_STATUS_FIFOFULL 			(1U <<	3)
#define SDXC_STATUS_FIFOEMPTY			(1U <<	2)

#define SDXC_CMDFLAG_STARTCMD			(1U << 31)
#define SDXC_CMDFLAG_USE_HOLD_REG		(1U << 29)
#define SDXC_CMDFLAG_VOLT_SWITCH		(1U << 28)
#define SDXC_CMDFLAG_BOOT_MODE			(1U << 27)
#define SDXC_CMDFLAG_DISABLE_BOOT		(1U << 26)
#define SDXC_CMDFLAG_EXPECTBOOTACK		(1U << 25)
#define SDXC_CMDFLAG_ENABLE_BOOT		(1U << 24)
#define SDXC_CMDFLAG_CCS_EXPECTED		(1U << 23)
#define SDXC_CMDFLAG_READCEATADEVICE 		(1U << 22)
#define SDXC_CMDFLAG_UPDATECLKONLY		(1U << 21)
#define SDXC_CMDFLAG_SENDINIT			(1U << 15)
#define SDXC_CMDFLAG_STOPABORT			(1U << 14)
#define SDXC_CMDFLAG_WAITPRVDAT			(1U << 13)
#define SDXC_CMDFLAG_SENDAUTOSTOP		(1U << 12)
#define SDXC_CMDFLAG_BLOCK			(0U << 11)
#define SDXC_CMDFLAG_STREAM			(1U << 11)
#define SDXC_CMDFLAG_TXDATA			(3U <<	9)
#define SDXC_CMDFLAG_RXDATA			(1U <<	9)
#define SDXC_CMDFLAG_CHKRSPCRC			(1U <<	8)
#define SDXC_CMDFLAG_SHORTRSP			(1U <<	6)
#define SDXC_CMDFLAG_LONGRSP 			(3U <<	6)

#define SDXC_RINTSTS_SDIO			(1U << 16)
#define SDXC_RINTSTS_EBE			(1U << 15)
#define SDXC_RINTSTS_ACD			(1U << 14)
#define SDXC_RINTSTS_SBE			(1U << 13)
#define SDXC_RINTSTS_HLE			(1U << 12)
#define SDXC_RINTSTS_FRUN			(1U << 11)
#define SDXC_RINTSTS_HTO			(1U << 10)
#define SDXC_RINTSTS_DRTO			(1U <<	9)
#define SDXC_RINTSTS_RTO			(1U <<	8)
#define SDXC_RINTSTS_DCRC			(1U <<	7)
#define SDXC_RINTSTS_RCRC			(1U <<	6)
#define SDXC_RINTSTS_RXDR			(1U <<	5)
#define SDXC_RINTSTS_TXDR			(1U <<	4)
#define SDXC_RINTSTS_DTO			(1U <<	3)
#define SDXC_RINTSTS_CD				(1U <<	2)
#define SDXC_RINTSTS_RE				(1U <<	1)

/* Function Define */
unsigned int sdxcboot(struct nx_bootmanager *pbm, unsigned int option);
int emmcboot(struct nx_bootmanager *pbm, unsigned int option);

#endif /* #ifndef __SDXCBOOT_H__ */
