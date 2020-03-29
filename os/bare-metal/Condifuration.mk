

CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = -print-gc-sections #--gc-sections

# this is stuff used everywhere - compiler and flags should be declared (ASFLAGS, CFLAGS, LDFLAGS, LINKER_SCRIPT, CC, AS, LD, DUMP, READ, OBJ and SIZE).
# remember the kernel, as well as the application, will be compiled using the *same* compiler and flags!
ASFLAGS = -march=rv32im -mabi=ilp32 -fPIC
CFLAGS = -Wall -march=rv32im -mabi=ilp32 -c -O2 -ffreestanding -nostdlib -ffixed-s10 -ffixed-s11 \
		 -fomit-frame-pointer $(INC_DIRS) 
		 
LDFLAGS = -melf32lriscv $(LDFLAGS_STRIP)
LINKER_SCRIPT = $(ARCH_DIR)/hf-riscv.ld

CC    = riscv64-unknown-elf-gcc
CPP   = riscv64-unknown-elf-g++
AS    = riscv64-unknown-elf-as
AR    = riscv64-unknown-elf-ar
LD    = riscv64-unknown-elf-ld
DUMP  = riscv64-unknown-elf-objdump -Mno-aliases
READ  = riscv64-unknown-elf-readelf
OBJ   = riscv64-unknown-elf-objcopy
SIZE  = riscv64-unknown-elf-size
