# Do not modify the lines below
APP_ANN_NAME  := ann
APP_ANN_DIR   := ./applications/$(APP_ANN_NAME)
APP_ANN_SRC   := $(APP_ANN_DIR)/src
APP_ANN_INC   := $(APP_ANN_DIR)/include
APP_ANN_LIB   := app-$(APP_ANN_NAME).a

INC_DIRS += -I$(APP_ANN_INC)

CFLAGS += 

# Update these lines with your source code
APP_ANN_SRCS := $(wildcard $(APP_ANN_SRC)/*.c)
APP_ANN_OBJS :=  $(APP_ANN_SRCS:.c=.o)

$(APP_ANN_LIB) : $(APP_ANN_OBJS)
	$(Q)$(AR) rcs $(APP_ANN_LIB) $(APP_ANN_OBJS) 
