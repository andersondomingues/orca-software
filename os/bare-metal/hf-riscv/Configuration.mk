# Bare metal configuration for hf-riscv processor, single-core platform
# These are the defines the app's makefile can change:
# - INC_DIRS - use it to define the app's include dirs
# - LIB_DIR	 - use it to define the app's lib dirs
# - LIBS     - use it to define the app's libs. 
# - DEFINES  - use it to create the app's defines.
# - ASMFLAGS - asm related flags
# - CFLAGS   - gcc related flags
# - CXXFLAGS - g++ related flags
# - LDFLAGS  - linker related flags
# - USE_LIBC - define/undefine this symbol to turn newlib on/off
# - USE_CPP  - define/undefine this symbol to turn cpp on/off

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

## fin the lib dir inside the toolchain
DIR_LIB_GCC=$(shell $(CC)  -march=rv32im -mabi=ilp32 --print-file-name=libgcc.a)
DIR_LIB_C=$(shell riscv64-unknown-elf-gcc  -march=rv32im -mabi=ilp32 --print-file-name=libm.a)
# get absolute path
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_GCC))))
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_C))))

#LDFLAGS += -L/opt/gcc-riscv/riscv64-unknown-elf/lib/rv32im/ilp32/
LIB_DIR  += $(patsubst %, -L%, $(LIB_DIR_LIST))

# if they were not defined by the app's make, then define it
INC_DIRS ?= 
DEFINES ?= 
LIBS ?= 

ifeq ($(USE_LIBC),)
  $(info LIBC support is disabled)
  BARE_METAL_SRC = $(CURR_DIR)/crt0.s $(CURR_DIR)/startup.c
else
  $(info LIBC support is enabled)
  DEFINES += -DUSE_LIBC
  LIBS += -lm -lc_nano -lgcc -lnosys
  BARE_METAL_SRC = $(CURR_DIR)/crt0.s $(CURR_DIR)/startup.c $(CURR_DIR)/syscalls.c
endif

ifeq ($(USE_CPP),)
  $(info CPP support is disabled)
else
  $(info CPP support is enabled)
  DEFINES += -DUSE_CPP
  LIBS += -lstdc++
endif

# the application will be compiled using the *same* compiler and flags!
ASMFLAGS += -march=rv32im -mabi=ilp32 -fPIC

# DBARE_METAL and DBARE_METAL_HF_RISC might be used in the software
# these are the flags that work for both C and C++
C_CPP_FLAGS += \
	-march=rv32im -mabi=ilp32 \
	-Wall \
	-O2 \
	-Os \
	$(DEFINES) \
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
	--start-group $(LIBS) --end-group 

export LINKER_SCRIPT = $(CURR_DIR)/hf-risc.ld

BARE_METAL_OBJS1 :=  $(BARE_METAL_SRC:.c=.o)
BARE_METAL_OBJS  :=  $(BARE_METAL_OBJS1:.s=.o)
OS_OBJS := $(BARE_METAL_OBJS)

$(OS_STATIC_LIB): $(BARE_METAL_OBJS)
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making bare metal  ...          \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)$(AR) rcs $@ $^
