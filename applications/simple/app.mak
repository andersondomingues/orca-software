# Do not modify the lines below
APP_SIMPLE_NAME  := simple
APP_SIMPLE_DIR   := ./applications/$(APP_SIMPLE_NAME)
APP_SIMPLE_SRC   := $(APP_SIMPLE_DIR)/src
APP_SIMPLE_INC   := $(APP_SIMPLE_DIR)/include
APP_SIMPLE_LIB   := app-$(APP_SIMPLE_NAME).a 

INC_DIRS += -I$(APP_SIMPLE_INC)

CFLAGS += 

# Update these lines with your source code
APP_SIMPLE_SRCS := $(wildcard $(APP_SIMPLE_SRC)/*.c)
APP_SIMPLE_OBJS :=  $(APP_SIMPLE_SRCS:.c=.o)

$(APP_SIMPLE_LIB) : $(APP_SIMPLE_OBJS)
	$(Q)$(AR) rcs $@ $^

