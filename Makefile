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

# get the OS depedent parameters. it also defines the OS_STATIC_LIB rule to compile the OS
include ./os/$(ORCA_OS)/Configuration.mk

#compile only the requested tasks 
$(foreach module,$(ORCA_APPLICATIONS),$(eval include applications/$(module)/app.mak))
#compile only the requested extensions 
$(foreach module,$(ORCA_EXTENSIONS),$(eval include extensions/$(module)/ext.mak))

#phonies
.PHONY: clean

# get the platform depedent parameters. 
-include $(ORCA_SIM_DIR)/platforms/$(ORCA_PLATFORM)/Configuration.mk

# concat the required libs and apps to build the image 
$(foreach module,$(ORCA_APPLICATIONS), $(eval APP_STATIC_LIBS := $(APP_STATIC_LIBS) app-$(module).a))
$(foreach module,$(ORCA_EXTENSIONS),   $(eval EXT_STATIC_LIBS := $(EXT_STATIC_LIBS) ext-$(module).a))
STATIC_LIBS := $(OS_STATIC_LIB) $(APP_STATIC_LIBS) $(EXT_STATIC_LIBS)

# general rule to compile all .c software
.c.o:
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

# general rule to compile all .cpp software
.cpp.o:
	$(Q)$(CPP) -c $(CXXFLAGS) -o $@ $<

# general rule to compile all .s software
.s.o:
	$(Q)$(AS) -c $(ASMFLAGS) -o $@ $<

ext: ext_banner $(EXT_STATIC_LIBS)

ext_banner:
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Extensions ...           \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	
app: app_banner $(APP_STATIC_LIBS)

app_banner:
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Applications ..          \e[0m"
	@echo "$'\e[7m==================================\e[0m"

$(IMAGE_NAME).bin: $(OS_STATIC_LIB) ext app
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Linking Software ...            \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)$(LD) $(BARE_METAL_OBJS) --start-group *.a --end-group $(LDFLAGS) -T$(LINKER_SCRIPT) -o $(IMAGE_NAME).elf 
	#$(Q)$(CC)  $(CFLAGS) -Wl,--gc-sections -Wl,--print-memory-usage -Wl,-Map=minimal.map -Wl,-T$(LINKER_SCRIPT) $(BARE_METAL_SRC)  -Wl,--start-group *.a -lm -lc_nano -lgcc -lstdc++ -lnosys -Wl,--end-group    -o $(IMAGE_NAME).elf 
	#$(Q)$(LD)  -melf32lriscv  --gc-sections --print-memory-usage -Map=minimal.map -T$(LINKER_SCRIPT) -L/opt/gcc-riscv/lib/gcc/riscv64-unknown-elf/8.3.0/rv32im/ilp32/  -L/opt/gcc-riscv/riscv64-unknown-elf/lib/rv32im/ilp32/ $(BARE_METAL_OBJS)  --start-group *.a -lm -lc_nano -lgcc -lstdc++ -lnosys --end-group    -o $(IMAGE_NAME).elf 
	$(Q)$(DUMP) --disassemble --reloc $(IMAGE_NAME).elf > $(IMAGE_NAME).lst
	$(Q)$(DUMP) -h $(IMAGE_NAME).elf > $(IMAGE_NAME).sec
	$(Q)$(DUMP) -s $(IMAGE_NAME).elf > $(IMAGE_NAME).cnt
	$(Q)$(OBJ) -O binary $(IMAGE_NAME).elf $(IMAGE_NAME).bin
	$(Q)$(SIZE) $(IMAGE_NAME).elf
	$(Q)hexdump -v -e '4/1 "%02x" "\n"' $(IMAGE_NAME).bin > $(IMAGE_NAME).txt

clean:
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m          Cleaning up...          \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)rm -rf *.o *~ *.elf *.bin *.cnt *.lst *.sec *.txt *.a
	$(Q)-find . -type f -name '*.su' -delete
	$(Q)-find . -type f -name '*.o' -delete
	$(Q)-find . -type f -name '*.a' -delete
	$(Q)-find . -type f -name '$(IMAGE_NAME).*' -delete
