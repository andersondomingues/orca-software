#main configuration file
include Configuration.mk

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
export Q := @
# Do not print "Entering directory ...". 
MAKEFLAGS += --no-print-directory
endif

ORCA_SIM_DIR = ../orca-sim
ORCA_SW_DIR = $(shell pwd)

IMAGE_NAME = image

# do not move this from here
all:  $(IMAGE_NAME).bin
	@echo "done"

# include the definitions from the main Configuration.mk
CFLAGS += $(COMPLINE)
CXXFLAGS += $(COMPLINE)

#compile only the requested tasks 
$(foreach module,$(ORCA_APPLICATIONS),$(eval include applications/$(module)/app.mak))
#compile only the requested libraries 
$(foreach module,$(ORCA_LIBS),$(eval include libs/$(module)/ext.mak))

# get the OS depedent parameters. it also defines the OS_STATIC_LIB rule to compile the OS
# this include needs to come after the app's include, otherwise it will not be possible to 
# configure the OS's according to the app's needs. One would need to manually change the OS's configure
include ./os/$(ORCA_OS)/Configuration.mk

#phonies
.PHONY: clean

# get the platform depedent parameters. 
include $(ORCA_SIM_DIR)/platforms/$(ORCA_PLATFORM)/Configuration.mk
# get memory mapped IO used in the platform to use the same in the sw
INC_DIRS += -I$(ORCA_SIM_DIR)/platforms/$(ORCA_PLATFORM)/include
# include the definitions from the main Configuration.mk
CFLAGS += $(PLAT_COMPLINE) $(MODELS_COMPLINE) 
CXXFLAGS += $(PLAT_COMPLINE) $(MODELS_COMPLINE) 

# get the models depedent parameters. 
include $(ORCA_SIM_DIR)/models/Configuration.mk

# concat the required libs and apps to build the image 
$(foreach module,$(ORCA_APPLICATIONS), $(eval APP_STATIC_LIBS := $(APP_STATIC_LIBS) app-$(module).a))
$(foreach module,$(ORCA_LIBS),   $(eval LIB_STATIC_LIBS := $(LIB_STATIC_LIBS) lib-$(module).a))
STATIC_LIBS := $(OS_STATIC_LIB) $(APP_STATIC_LIBS) $(LIB_STATIC_LIBS)

# general rule to compile all .c software
.c.o:
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

# general rule to compile all .cpp software
.cpp.o:
	$(Q)$(CPP) -c $(CXXFLAGS) -o $@ $<

# general rule to compile all .s software
.s.o:
	$(Q)$(AS) -c $(ASMFLAGS) -o $@ $<

lib: lib_banner $(LIB_STATIC_LIBS)

lib_banner:
	@echo "$'\033[7m==================================\033[0m"
	@echo "$'\033[7m  Making Libraries ...            \033[0m"
	@echo "$'\033[7m==================================\033[0m"
	
app: app_banner $(APP_STATIC_LIBS)

app_banner:
	@echo "$'\033[7m==================================\033[0m"
	@echo "$'\033[7m  Making Applications ..          \033[0m"
	@echo "$'\033[7m==================================\033[0m"

$(IMAGE_NAME).bin: $(OS_STATIC_LIB) lib app
	@echo "$'\033[7m==================================\033[0m"
	@echo "$'\033[7m  Linking Software ...            \033[0m"
	@echo "$'\033[7m==================================\033[0m"
	$(Q)$(LD) $(OS_OBJS) --start-group *.a --end-group $(LDFLAGS) -T$(LINKER_SCRIPT) -o $(IMAGE_NAME).elf 
	$(Q)$(DUMP) --disassemble --reloc $(IMAGE_NAME).elf > $(IMAGE_NAME).lst
	$(Q)$(DUMP) -h $(IMAGE_NAME).elf > $(IMAGE_NAME).sec
	$(Q)$(DUMP) -s $(IMAGE_NAME).elf > $(IMAGE_NAME).cnt
	$(Q)$(OBJ) -O binary $(IMAGE_NAME).elf $(IMAGE_NAME).bin
	$(Q)$(SIZE) $(IMAGE_NAME).elf
	$(Q)hexdump -v -e '4/1 "%02x" "\n"' $(IMAGE_NAME).bin > $(IMAGE_NAME).txt

clean:
	@echo "$'\033[7m==================================\033[0m"
	@echo "$'\033[7m          Cleaning up...          \033[0m"
	@echo "$'\033[7m==================================\033[0m"
	$(Q)rm -rf *.o *~ *.elf *.bin *.cnt *.lst *.sec *.txt *.a
	$(Q)-find . -type f -name '*.su' -delete
	$(Q)-find . -type f -name '*.o' -delete
	$(Q)-find . -type f -name '*.a' -delete
	$(Q)-find . -type f -name '$(IMAGE_NAME).*' -delete
