# Do not modify the lines below
APP_INT_MULT_NAME  := int-mult
APP_INT_MULT_DIR   := ./applications/$(APP_INT_MULT_NAME)
APP_INT_MULT_SRC   := $(APP_INT_MULT_DIR)/src
APP_INT_MULT_INC   := $(APP_INT_MULT_DIR)/include
APP_INT_MULT_LIB   := app-$(APP_INT_MULT_NAME).a

INC_DIRS += -I$(APP_INT_MULT_INC)
 
CFLAGS += 

# Update these lines with your source code
APP_INT_MULT_SRCS := $(wildcard $(APP_INT_MULT_SRC)/*.c)
APP_INT_MULT_OBJS :=  $(APP_INT_MULT_SRCS:.c=.o)

$(APP_INT_MULT_LIB) : $(APP_INT_MULT_OBJS)
	$(Q)$(AR) rcs $(APP_INT_MULT_LIB) $(APP_INT_MULT_OBJS) 
