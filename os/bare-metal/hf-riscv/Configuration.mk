# bare metal configuration for hf-riscv processor, single-core platform

# name of the lib
export OS_STATIC_LIB := bare-metal.a

# issue an error if trying to compile with an incompatible platform.
# bare metal apps only work with single-core platform
ifneq ($(PLATFORM), single-core)
$(error ERROR: PLATFORM = $(PLATFORM) is not compatible with bare metal apps)
endif

CURR_DIR = $(ORCA_SW_DIR)/os/$(ORCA_OS)

INC_DIRS += -I$(CURR_DIR)

F_CLK=25000000

## fin the lib dir inside the toolchain
DIR_LIB_GCC=$(shell riscv64-unknown-elf-gcc  -march=rv32im -mabi=ilp32 --print-file-name=libgcc.a)
DIR_LIB_C=$(shell riscv64-unknown-elf-gcc  -march=rv32im -mabi=ilp32 --print-file-name=libm.a)
# get absolute path
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_GCC))))
LIB_DIR_LIST +=  $(dir $(abspath $(lastword $(DIR_LIB_C))))

#LDFLAGS += -L/opt/gcc-riscv64/riscv-gcc-8.3.0-sifive/riscv64-unknown-elf/lib/rv32im/ilp32
LIB_DIR  = $(patsubst %, -L%, $(LIB_DIR_LIST))

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LINKER_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
# remember the kernel, as well as the application, will be compiled using the *same* compiler and flags!
ASMFLAGS = -march=rv32im -mabi=ilp32 -fPIC
CFLAGS = -Wall -march=rv32im -mabi=ilp32 -c -O2 -ffreestanding -nostdlib -ffixed-s10 -ffixed-s11 \
		-fomit-frame-pointer -fdata-sections -ffunction-sections --specs=nosys.specs  \
		-DCPU_SPEED=${F_CLK} $(INC_DIRS)
#-nostdlib -mstrict-align  -DLITTLE_ENDIAN $(CFLAGS_STRIP) -DDEBUG_PORT

LDFLAGS = -melf32lriscv -print-gc-sections $(LIB_DIR) -lc_nano -lm -lc_nano -lgcc
export LINKER_SCRIPT = $(CURR_DIR)/hf-risc.ld

CC    = riscv64-unknown-elf-gcc
CPP   = riscv64-unknown-elf-g++
AS    = riscv64-unknown-elf-as
AR    = riscv64-unknown-elf-ar
LD    = riscv64-unknown-elf-ld
DUMP  = riscv64-unknown-elf-objdump -Mno-aliases
READ  = riscv64-unknown-elf-readelf
OBJ   = riscv64-unknown-elf-objcopy
SIZE  = riscv64-unknown-elf-size

BARE_METAL_SRC = $(CURR_DIR)/crt0.s $(CURR_DIR)/interrupt.c $(CURR_DIR)/syscalls_usart.c
BARE_METAL_OBJS :=  $(BARE_METAL_SRC:.c=.o)
BARE_METAL_OBJS +=  $(BARE_METAL_SRC:.s=.o)

$(OS_STATIC_LIB): $(BARE_METAL_OBJS)
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making bare metal  ...          \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)$(AR) rcs $(OS_STATIC_LIB) $(CURR_DIR)/*.o