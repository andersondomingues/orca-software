# Do not modify the lines below
APP_SIMPLE_DMA_NAME  := simple-dma
APP_SIMPLE_DMA_DIR   := ./applications/$(APP_SIMPLE_DMA_NAME)
APP_SIMPLE_DMA_SRC   := $(APP_SIMPLE_DMA_DIR)/src
APP_SIMPLE_DMA_INC   := $(APP_SIMPLE_DMA_DIR)/include
APP_SIMPLE_DMA_LIB   := app-$(APP_SIMPLE_DMA_NAME).a

ifneq ($(ORCA_PLATFORM), single-core-nn)
$(error ERROR: this application requires the single-core-nn platform)
endif

ifneq ($(ORCA_OS), hellfireos)
$(error ERROR: this application requires the hellfire OS)
endif

INC_DIRS += -I$(APP_SIMPLE_DMA_INC)

CFLAGS += 

# TODO it would be nice to make this linker script compatible with bare-metal, removing the depedency of hellfireos
LINKER_SCRIPT = $(APP_SIMPLE_DMA_DIR)/hf-riscv.ld

# Update these lines with your source code
APP_SIMPLE_DMA_SRCS := $(wildcard $(APP_SIMPLE_DMA_SRC)/*.c)
APP_SIMPLE_DMA_OBJS :=  $(APP_SIMPLE_DMA_SRCS:.c=.o)

$(APP_SIMPLE_DMA_LIB) : $(APP_SIMPLE_DMA_OBJS)
	$(Q)$(AR) rcs $@ $^
