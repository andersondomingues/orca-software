# Do not modify the lines below
APP_MNIST_EXT_MULT_DMA_NAME  := mnist-ext-mult-dma
APP_MNIST_EXT_MULT_DMA_DIR   := ./applications/$(APP_MNIST_EXT_MULT_DMA_NAME)
APP_MNIST_EXT_MULT_DMA_SRC   := $(APP_MNIST_EXT_MULT_DMA_DIR)/src
APP_MNIST_EXT_MULT_DMA_INC   := $(APP_MNIST_EXT_MULT_DMA_DIR)/include
APP_MNIST_EXT_MULT_DMA_LIB   := app-$(APP_MNIST_EXT_MULT_DMA_NAME).a

ifneq ($(ORCA_PLATFORM), single-core-nn)
$(error ERROR: this application requires the single-core-nn platform)
endif

ifneq ($(ORCA_OS), hellfireos)
$(error ERROR: this application requires the hellfire OS)
endif

INC_DIRS += -I$(APP_MNIST_EXT_MULT_DMA_INC)

CFLAGS += 

# TODO it would be nice to make this linker script compatible with bare-metal, removing the depedency of hellfireos
LINKER_SCRIPT = $(APP_MNIST_EXT_MULT_DMA_DIR)/hf-riscv.ld

# Update these lines with your source code
APP_MNIST_EXT_MULT_DMA_SRCS := $(wildcard $(APP_MNIST_EXT_MULT_DMA_SRC)/*.c)
APP_MNIST_EXT_MULT_DMA_OBJS :=  $(APP_MNIST_EXT_MULT_DMA_SRCS:.c=.o)

$(APP_MNIST_EXT_MULT_DMA_LIB) : $(APP_MNIST_EXT_MULT_DMA_OBJS)
	$(Q)$(AR) rcs $(APP_MNIST_EXT_MULT_DMA_LIB) $(APP_MNIST_EXT_MULT_DMA_OBJS) 
