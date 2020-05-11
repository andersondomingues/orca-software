# Do not modify the lines below
APP_COUNTER_TEST_NAME  := counter-test
APP_COUNTER_TEST_DIR   := $(ORCA_SW_DIR)/applications/$(APP_COUNTER_TEST_NAME)
APP_COUNTER_TEST_SRC   := $(APP_COUNTER_TEST_DIR)/src
APP_COUNTER_TEST_INC   := $(APP_COUNTER_TEST_DIR)/include
APP_COUNTER_TEST_LIB   := app-$(APP_COUNTER_TEST_NAME).a 

INC_DIRS += -I$(APP_COUNTER_TEST_INC)

CFLAGS += 

ifeq ($(ORCA_OS), bare-metal/hf-riscv)
# define/undefine this symbol to turn newlib on/off
USE_LIBC = 1
# define/undefine this symbol to turn cpp on/off
#USE_CPP = 1
endif

# Update these lines with your source code
APP_COUNTER_TEST_SRCS := $(wildcard $(APP_COUNTER_TEST_SRC)/*.c)
APP_COUNTER_TEST_OBJS :=  $(APP_COUNTER_TEST_SRCS:.c=.o)

$(APP_COUNTER_TEST_LIB) : $(APP_COUNTER_TEST_OBJS)
	$(Q)$(AR) rcs $@ $^

