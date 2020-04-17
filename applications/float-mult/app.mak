# Do not modify the lines below
APP_FLOAT_MULT_NAME  := float-mult
APP_FLOAT_MULT_DIR   := ./applications/$(APP_FLOAT_MULT_NAME)
APP_FLOAT_MULT_SRC   := $(APP_FLOAT_MULT_DIR)/src
APP_FLOAT_MULT_INC   := $(APP_FLOAT_MULT_DIR)/include
APP_FLOAT_MULT_LIB   := app-$(APP_FLOAT_MULT_NAME).a

INC_DIRS += -I$(APP_FLOAT_MULT_INC)
 
CFLAGS += 

# Update these lines with your source code
APP_FLOAT_MULT_SRCS := $(wildcard $(APP_FLOAT_MULT_SRC)/*.c)
APP_FLOAT_MULT_OBJS :=  $(APP_FLOAT_MULT_SRCS:.c=.o)

$(APP_FLOAT_MULT_LIB) : $(APP_FLOAT_MULT_OBJS)
	$(Q)$(AR) rcs $(APP_FLOAT_MULT_LIB) $(APP_FLOAT_MULT_OBJS) 
