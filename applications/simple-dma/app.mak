# Do not modify the lines below
APP_SIMPLE_DMA_NAME  := simple-dma
APP_SIMPLE_DMA_DIR   := ./applications/$(APP_SIMPLE_DMA_NAME)
APP_SIMPLE_DMA_SRC   := $(APP_SIMPLE_DMA_DIR)/src
APP_SIMPLE_DMA_INC   := $(APP_SIMPLE_DMA_DIR)/include
APP_SIMPLE_DMA_LIB   := app-$(APP_SIMPLE_DMA_NAME).a

INC_DIRS += -I$(APP_SIMPLE_DMA_INC)

CFLAGS += 

LINKER_SCRIPT = $(APP_SIMPLE_DMA_DIR)/hf-riscv.ld

# Update these lines with your source code
APP_SIMPLE_DMA_SRCS := $(wildcard $(APP_SIMPLE_DMA_SRC)/*.c)
APP_SIMPLE_DMA_OBJS :=  $(APP_SIMPLE_DMA_SRCS:.c=.o)

$(APP_SIMPLE_DMA_LIB) : $(APP_SIMPLE_DMA_OBJS)
	$(Q)$(AR) rcs $(APP_SIMPLE_DMA_LIB) $(APP_SIMPLE_DMA_OBJS) 
