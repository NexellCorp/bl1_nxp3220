# Build Version info
VERINFO				= V001

#########################################################################
# build environment
##########################################################################
CFLAGS				:=
DEBUG				?= n

# chip name
CHIPNAME			?= nxp3220

# supported (thridboot) boot mode
SUPPORT_USB_BOOT		?= y
SUPPORT_SDMMC_BOOT		?= y
SUPPORT_UART_BOOT		?= y
SUPPORT_SPI_BOOT		?= y
SUPPORT_NAND_BOOT		?= y

# system log Message
SYSLOG				?= y
EARLY_SERIAL			?= y

# cross-tool pre-header
ifeq ($(OS),Windows_NT)
CROSS_COMPILE_TOP		?=
CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-none-eabi-
else
CROSS_COMPILE_TOP		?=
#CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-linux-gnueabihf-
CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-none-eabi-
endif

# Top Names
PROJECT_NAME			=
TARGET_NAME			= bl1-${CHIPNAME}
LDS_NAME			= bl1

# Directories
DIR_PROJECT_TOP			=

DIR_OBJOUTPUT			= obj
DIR_TARGETOUTPUT		= out

# Build Environment
ARCH				= armv7-a
CPU				= cortex-a7
CC				= $(CROSS_COMPILE)gcc
LD 				= $(CROSS_COMPILE)ld
AS 				= $(CROSS_COMPILE)as
AR 				= $(CROSS_COMPILE)ar
MAKEBIN				= $(CROSS_COMPILE)objcopy
OBJCOPY				= $(CROSS_COMPILE)objcopy
RANLIB 				= $(CROSS_COMPILE)ranlib

GCC_LIB				= $(shell $(CC) -print-libgcc-file-name)

ifeq ($(DEBUG), y)
CFLAGS				= -DNX_DEBUG -O0
Q				=
else
CFLAGS				= -DNX_RELEASE -Os
Q				= @
endif

# MISC tools for MS-DOS
ifeq ($(OS),Windows_NT)
MKDIR				= mkdir
RM				= del /q /F
MV				= move
CD				= cd
CP				= copy
ECHO				= echo
RMDIR				= rmdir /S /Q
else
MKDIR				= mkdir
RM				= rm -f
MV				= mv
CD				= cd
CP				= cp
ECHO				= echo
RMDIR				= rm -rf
endif

#########################################################################
# flags variables
#########################################################################
# FLAGS
ARFLAGS				= rcs
ARFLAGS_REMOVE			= -d
ARLIBFLAGS			= -v -s

ASFLAG				= -D__ASSEMBLY__

CFLAGS				+=	-g -Wall					\
					-Wextra -ffreestanding -fno-builtin		\
					-mcpu=$(CPU)					\
					-mfloat-abi=hard				\
					-mfpu=vfpv3-d16 				\
					-mlittle-endian					\
					-mstructure-size-boundary=32			\
					-DCHIPID_$(shell echo $(CHIPNAME) | tr a-z A-Z)

ifeq ($(EARLY_SERIAL),y)
CFLAGS				+=	-DEARLY_SERIAL
endif

CFLAGS				+=	-DDBGLOG_ON

# system log messgae
ifeq ($(SYSLOG), y)
CFLAGS				+=	-DSYSLOG_ON
endif
