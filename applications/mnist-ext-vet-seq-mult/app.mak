# Do not modify the lines below
APP_MNIST_EXT_VET_SEQ_MULT_NAME  := mnist-ext-vet-seq-mult
APP_MNIST_EXT_VET_SEQ_MULT_DIR   := ./applications/$(APP_MNIST_EXT_VET_SEQ_MULT_NAME)
APP_MNIST_EXT_VET_SEQ_MULT_SRC   := $(APP_MNIST_EXT_VET_SEQ_MULT_DIR)/src
APP_MNIST_EXT_VET_SEQ_MULT_INC   := $(APP_MNIST_EXT_VET_SEQ_MULT_DIR)/include
APP_MNIST_EXT_VET_SEQ_MULT_LIB   := app-$(APP_MNIST_EXT_VET_SEQ_MULT_NAME).a

INC_DIRS += -I$(APP_MNIST_EXT_VET_SEQ_MULT_INC)

CFLAGS += 

# Update these lines with your source code
APP_MNIST_EXT_VET_SEQ_MULT_SRCS := $(wildcard $(APP_MNIST_EXT_VET_SEQ_MULT_SRC)/*.c)
APP_MNIST_EXT_VET_SEQ_MULT_OBJS :=  $(APP_MNIST_EXT_VET_SEQ_MULT_SRCS:.c=.o)

$(APP_MNIST_EXT_VET_SEQ_MULT_LIB) : $(APP_MNIST_EXT_VET_SEQ_MULT_OBJS)
	$(Q)$(AR) rcs $(APP_MNIST_EXT_VET_SEQ_MULT_LIB) $(APP_MNIST_EXT_VET_SEQ_MULT_OBJS) 
