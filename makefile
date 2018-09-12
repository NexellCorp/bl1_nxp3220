include config.mak

LDFLAGS		=	-Bstatic							\
			-Wl,-Map=$(DIR_TARGETOUTPUT)/$(TARGET_NAME).map,--cref		\
			-T$(LDS_NAME).lds						\
			-Wl,--start-group						\
			-Lsrc/$(DIR_OBJOUTPUT)						\
			-Wl,--end-group							\
			-Wl,--build-id=none						\
			-lgcc								\
			-nostdlib

SYS_OBJS	+=	startup.o libnx.o libarmv7.o armv7_pmu.o delay.o cpu_delay.o pll.o cmu.o printf.o pmu.o		\
			serial.o plat_pm.o gic.o gpio.o efuse.o checker.o libplat.o plat_load.o tz.o cpupmu.o subcpu.o  \
			secure_rw.o sss.o sys.o build_info.o main.o

# Dispatcher (SMC Call)
SYS_OBJS	+=	smc_entry.o smc_handler.o sip_main.o std_svc_setup.o			\
			arm_topology.o psci_system_off.o psci_off.o psci_on.o 			\
			psci_suspend.o psci_common.o psci_main.o

ifeq ($(SUPPORT_USB_BOOT),y)
SYS_OBJS	+=	usbboot.o
endif
ifeq ($(SUPPORT_UART_BOOT),y)
SYS_OBJS	+=	uartboot.o
endif
ifeq ($(SUPPORT_SDMMC_BOOT),y)
SYS_OBJS	+=	sdxcboot.o
endif
ifeq ($(SUPPORT_SPI_BOOT),y)
SYS_OBJS	+=	spiboot.o
endif
ifeq ($(SUPPORT_NAND_BOOT),y)
SYS_OBJS	+=	nandboot.o
endif


SYS_OBJS_LIST	+=	$(addprefix $(DIR_OBJOUTPUT)/,$(SYS_OBJS))

SYS_INCLUDES	+=	-I src/						\
			-I src/lib					\
			-I src/common					\
			-I src/include					\
			-I src/include/configs				\
			-I src/drivers					\
			-I src/drivers/clock				\
			-I src/drivers/serial				\
			-I src/boot					\
			-I src/services					\
			-I src/services/std_svc/			\
			-I src/services/std_svc/psci

###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/%.s
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################

###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/lib/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/lib/%.s
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
###################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/clock/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/drivers/serial/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################

##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/boot/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################

##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/%.s
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(ASFLAG) $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/std_svc/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################
$(DIR_OBJOUTPUT)/%.o: src/services/std_svc/psci/%.c
	@echo [compile....$<]
	$(Q)$(CC) -MMD $< -c -o $@ $(CFLAGS) $(SYS_INCLUDES)
##################################################################################################

all: mkobjdir $(SYS_OBJS_LIST) link bin

link:
	@echo [link.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf]
	$(Q)$(CC) $(SYS_OBJS_LIST) $(LDFLAGS) -o $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf

bin:
	@echo [binary.... $(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin]
	$(Q)$(MAKEBIN) -O binary $(DIR_TARGETOUTPUT)/$(TARGET_NAME).elf $(DIR_TARGETOUTPUT)/$(TARGET_NAME).bin

mkobjdir:
ifeq ($(OS),Windows_NT)
	@if not exist $(DIR_OBJOUTPUT)			\
		@$(MKDIR) $(DIR_OBJOUTPUT)
	@if not exist $(DIR_TARGETOUTPUT)		\
		@$(MKDIR) $(DIR_TARGETOUTPUT)
else
	@if	[ ! -e $(DIR_OBJOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ ! -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(MKDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(DIR_OBJOUTPUT)			\
		@$(RMDIR) $(DIR_OBJOUTPUT)
	@if exist $(DIR_TARGETOUTPUT)			\
		@$(RMDIR) $(DIR_TARGETOUTPUT)
else
	@if	[ -e $(DIR_OBJOUTPUT) ]; then 		\
		$(RMDIR) $(DIR_OBJOUTPUT);		\
	fi;
	@if	[ -e $(DIR_TARGETOUTPUT) ]; then 	\
		$(RMDIR) $(DIR_TARGETOUTPUT);		\
	fi;
endif

-include $(SYS_OBJS_LIST:.o=.d)
