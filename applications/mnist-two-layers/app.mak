# Do not modify the lines below
APP_MNIST_TWO_LAYERS_NAME  := mnist-two-layers
APP_MNIST_TWO_LAYERS_DIR   := ./applications/$(APP_MNIST_TWO_LAYERS_NAME)
APP_MNIST_TWO_LAYERS_SRC   := $(APP_MNIST_TWO_LAYERS_DIR)/src
APP_MNIST_TWO_LAYERS_INC   := $(APP_MNIST_TWO_LAYERS_DIR)/include
APP_MNIST_TWO_LAYERS_LIB   := app-$(APP_MNIST_TWO_LAYERS_NAME).a

#$(info $$PWD = [$(shell pwd)])

INC_DIRS += -I$(APP_MNIST_TWO_LAYERS_INC)
 
CFLAGS += 

ifeq ($(ORCA_OS), bare-metal/hf-riscv)
# define/undefine this symbol to turn newlib on/off
USE_LIBC = 1
# define/undefine this symbol to turn cpp on/off
#USE_CPP = 1
endif

# Update these lines with your source code
APP_MNIST_TWO_LAYERS_SRCS := $(wildcard $(APP_MNIST_TWO_LAYERS_SRC)/*.c)
APP_MNIST_TWO_LAYERS_OBJS :=  $(APP_MNIST_TWO_LAYERS_SRCS:.c=.o)

$(APP_MNIST_TWO_LAYERS_LIB) : $(APP_MNIST_TWO_LAYERS_OBJS)
	$(Q)$(AR) rcs $@ $^
