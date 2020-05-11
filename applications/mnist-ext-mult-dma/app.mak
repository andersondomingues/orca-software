# Do not modify the lines below
APP_MNIST_EXT_MULT_DMA_NAME  := mnist-ext-mult-dma
APP_MNIST_EXT_MULT_DMA_DIR   := ./applications/$(APP_MNIST_EXT_MULT_DMA_NAME)
APP_MNIST_EXT_MULT_DMA_SRC   := $(APP_MNIST_EXT_MULT_DMA_DIR)/src
APP_MNIST_EXT_MULT_DMA_INC   := $(APP_MNIST_EXT_MULT_DMA_DIR)/include
APP_MNIST_EXT_MULT_DMA_LIB   := app-$(APP_MNIST_EXT_MULT_DMA_NAME).a

ifneq ($(ORCA_PLATFORM), single-core-nn)
$(error ERROR: this application requires the single-core-nn platform)
endif

INC_DIRS += -I$(APP_MNIST_EXT_MULT_DMA_INC)

CFLAGS += 

ifeq ($(ORCA_OS), bare-metal/hf-riscv)
# define/undefine this symbol to turn newlib on/off
USE_LIBC = 1
# define/undefine this symbol to turn cpp on/off
#USE_CPP = 1
LDFLAGS += --defsym=RAM_SIZE=9M
LINKER_SCRIPT = $(APP_MNIST_EXT_MULT_DMA_DIR)/hf-riscv-bare-metal.ld
endif

ifeq ($(ORCA_OS), hellfireos)
LINKER_SCRIPT = $(APP_MNIST_EXT_MULT_DMA_DIR)/hf-riscv-hellfireos.ld
endif

# Update these lines with your source code
APP_MNIST_EXT_MULT_DMA_SRCS := $(wildcard $(APP_MNIST_EXT_MULT_DMA_SRC)/*.c)
APP_MNIST_EXT_MULT_DMA_OBJS :=  $(APP_MNIST_EXT_MULT_DMA_SRCS:.c=.o)

$(APP_MNIST_EXT_MULT_DMA_LIB) : $(APP_MNIST_EXT_MULT_DMA_OBJS)
	$(Q)$(AR) rcs $(APP_MNIST_EXT_MULT_DMA_LIB) $(APP_MNIST_EXT_MULT_DMA_OBJS) 
