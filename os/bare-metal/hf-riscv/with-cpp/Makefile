#source:  https://github.com/memfault/zero-to-main/tree/master/minimal

PROJECT := main
BUILD_DIR ?= build

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
export Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

CC    = riscv64-unknown-elf-gcc
CPP   = riscv64-unknown-elf-g++
AS    = riscv64-unknown-elf-as
AR    = riscv64-unknown-elf-ar
LD    = riscv64-unknown-elf-ld
DUMP  = riscv64-unknown-elf-objdump -Mno-aliases
READ  = riscv64-unknown-elf-readelf
OBJ   = riscv64-unknown-elf-objcopy
SIZE  = riscv64-unknown-elf-size
MKDIR=mkdir

## find the lib dir inside the toolchain
DIR_LIB_GCC=$(shell $(CC) -march=rv32im -mabi=ilp32 --print-file-name=libgcc.a)
DIR_LIB_C=$(shell   $(CC) -march=rv32im -mabi=ilp32 --print-file-name=libm.a)
# get absolute path
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_GCC))))
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_C))))

#LDFLAGS += -L/opt/gcc-riscv64/riscv-gcc-8.3.0-sifive/riscv64-unknown-elf/lib/rv32im/ilp32
LIB_DIR  = $(patsubst %, -L%, $(LIB_DIR_LIST))

#
#	-Wl,--build-id \

# use -DHEAP_SIZE and -DSTACK_SIZE to chage the memory size
# used -DCONFIG_CPLUSPLUS when using c++


# these are the flags that work for both C and C++
C_CPP_FLAGS += \
	-march=rv32im -mabi=ilp32 \
	-Wall \
	-O2 \
	-Os \
	-D_DEBUG \
	-DCONFIG_CPLUSPLUS \
	-ffreestanding \
	-ffunction-sections \
	-fdata-sections \
	-nostdlib

# these are the flags only for C
CFLAGS += $(C_CPP_FLAGS) \
	-std=c11 

# c++ related flags
#   -fno-exceptions
#	-fno-unwind-tables, removes the .eh_frame section
#   -fno-rtti \  
CPPFLAGS += $(C_CPP_FLAGS) \
    -fno-exceptions \
	-fno-unwind-tables \
    -fno-rtti 

LDFLAGS += \
	-specs=nano.specs \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	-Wl,-Map=minimal.map \
	$(LIB_DIR) \
	-fno-math-errno \
	-Wl,--start-group -lm -lc_nano -lgcc -lstdc++ -Wl,--end-group \
	-T hf-risc.ld


INCLUDES = .

SRCS = \
	with-libc.c \
	crt0.s \
	startup.c \
	mat.cpp \
	test_class.c
# replace by libm_test.c to test libm
# replace by libc_test.c to test libc_nano

OBJ_DIR = $(BUILD_DIR)/objs
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean
all: $(BUILD_DIR)/$(PROJECT).bin

$(BUILD_DIR):
	$(Q)$(MKDIR) -p $(BUILD_DIR)

$(OBJ_DIR):
	$(Q)$(MKDIR) -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	@echo "Compiling $<"
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)/%.o: %.cpp $(OBJ_DIR)
	@echo "Compiling $<"
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CPP) -c -o $@ $< $(CPPFLAGS)


$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).lst $(BUILD_DIR)/$(PROJECT).read
	$(OBJ) $< $@ -O binary
	$(Q)hexdump -v -e '4/1 "%02x" "\n"' $(BUILD_DIR)/$(PROJECT).bin > $(BUILD_DIR)/$(PROJECT).txt
	$(SIZE) $<

$(BUILD_DIR)/$(PROJECT).lst: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)
	$(DUMP) -D $< > $@

$(BUILD_DIR)/$(PROJECT).read: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)
	$(READ) -a $< > $@

$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	@echo "Linking $@"
	$(Q)$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@	

clean:
	@echo "Cleaning $<"
	$(Q)rm -rf build logs/*.log *.map
