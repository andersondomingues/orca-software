# Do not modify the lines below
APP_MNIST_NAME  := mnist
APP_MNIST_DIR   := ./applications/$(APP_MNIST_NAME)
APP_MNIST_SRC   := $(APP_MNIST_DIR)/src
APP_MNIST_INC   := $(APP_MNIST_DIR)/include
APP_MNIST_LIB   := app-$(APP_MNIST_NAME).a

$(info $$PWD = [$(shell pwd)])

INC_DIRS += -I$(APP_MNIST_INC)
 
CFLAGS += 

# Update these lines with your source code
APP_MNIST_SRCS := $(wildcard $(APP_MNIST_SRC)/*.c)
APP_MNIST_OBJS :=  $(APP_MNIST_SRCS:.c=.o)

$(APP_MNIST_LIB) : $(APP_MNIST_OBJS)
	$(Q)$(AR) rcs $(APP_MNIST_LIB) $(APP_MNIST_OBJS) 
