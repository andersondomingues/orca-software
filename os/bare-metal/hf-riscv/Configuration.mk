# bare metal configuration for hf-riscv processor, single-core platform

# name of the lib
export OS_STATIC_LIB := bare-metal.a

CC    = riscv64-unknown-elf-gcc
CPP   = riscv64-unknown-elf-g++
AS    = riscv64-unknown-elf-as
AR    = riscv64-unknown-elf-ar
LD    = riscv64-unknown-elf-ld
DUMP  = riscv64-unknown-elf-objdump -Mno-aliases
READ  = riscv64-unknown-elf-readelf
OBJ   = riscv64-unknown-elf-objcopy
SIZE  = riscv64-unknown-elf-size

# issue an error if trying to compile with an incompatible platform.
# bare metal apps only work with single-core platform
ifneq ($(ORCA_PLATFORM), single-core)
$(error ERROR: ORCA_PLATFORM = $(ORCA_PLATFORM) is not compatible with bare metal apps)
endif

CURR_DIR = $(ORCA_SW_DIR)/os/$(ORCA_OS)

# nothing to include
#INC_DIRS += -I$(CURR_DIR)
INC_DIRS += 

## fin the lib dir inside the toolchain
DIR_LIB_GCC=$(shell $(CC)  -march=rv32im -mabi=ilp32 --print-file-name=libgcc.a)
DIR_LIB_C=$(shell riscv64-unknown-elf-gcc  -march=rv32im -mabi=ilp32 --print-file-name=libm.a)
# get absolute path
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_GCC))))
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_C))))

#LDFLAGS += -L/opt/gcc-riscv/riscv64-unknown-elf/lib/rv32im/ilp32/
LIB_DIR  = $(patsubst %, -L%, $(LIB_DIR_LIST))

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LINKER_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
# remember the kernel, as well as the application, will be compiled using the *same* compiler and flags!
ASMFLAGS = -march=rv32im -mabi=ilp32 -fPIC

# DBARE_METAL and DBARE_METAL_HF_RISC might be used in the software
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
	-fno-math-errno \
	-specs=nano.specs \
	-nostdlib \
	$(INC_DIRS) \
	-DBARE_METAL \
	-DBARE_METAL_HF_RISC

# these are the flags only for C
CFLAGS += $(C_CPP_FLAGS) \
	-std=c11 

# c++ related flags
#   -fno-exceptions
#	-fno-unwind-tables, removes the .eh_frame section
#   -fno-rtti \  
CXXFLAGS += $(C_CPP_FLAGS) \
    -fno-exceptions \
	-fno-unwind-tables \
    -fno-rtti \
	-std=c++11

LDFLAGS += \
	-melf32lriscv \
	--gc-sections \
	--print-memory-usage \
	-Map=$(IMAGE_NAME).map \
	$(LIB_DIR) \
	--start-group -lm -lc_nano -lgcc -lstdc++ -lnosys --end-group 

export LINKER_SCRIPT = $(CURR_DIR)/hf-risc.ld


BARE_METAL_SRC = $(CURR_DIR)/crt0.s $(CURR_DIR)/startup.c $(CURR_DIR)/syscalls.c
BARE_METAL_OBJS1 :=  $(BARE_METAL_SRC:.c=.o)
BARE_METAL_OBJS  :=  $(BARE_METAL_OBJS1:.s=.o)
OS_OBJS := $(BARE_METAL_OBJS)

$(OS_STATIC_LIB): $(BARE_METAL_OBJS)
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making bare metal  ...          \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)$(AR) rcs $@ $^
