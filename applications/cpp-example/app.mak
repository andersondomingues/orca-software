# Do not modify the lines below
APP_CPP_EXAMPLE_NAME  := cpp-example
APP_CPP_EXAMPLE_DIR   := ./applications/$(APP_CPP_EXAMPLE_NAME)
APP_CPP_EXAMPLE_SRC   := $(APP_CPP_EXAMPLE_DIR)/src
APP_CPP_EXAMPLE_INC   := $(APP_CPP_EXAMPLE_DIR)/include
APP_CPP_EXAMPLE_LIB   := app-$(APP_CPP_EXAMPLE_NAME).a 

INC_DIRS += -I$(APP_CPP_EXAMPLE_INC) -I/opt/gcc-riscv64/riscv-gcc-8.3.0-sifive/riscv64-unknown-elf/include/c++/8.3.0/

CFLAGS += 
CXXFLAGS += 
ASMFLAGS +=

ifeq ($(ORCA_OS), bare-metal/hf-riscv)
# define/undefine this symbol to turn newlib on/off
USE_LIBC = 1
# define/undefine this symbol to turn cpp on/off
USE_CPP = 1
endif

# Update these lines with your source code
APP_CPP_EXAMPLE_SRCS := $(wildcard $(APP_CPP_EXAMPLE_SRC)/*.c)
APP_CPP_EXAMPLE_SRCS += $(wildcard $(APP_CPP_EXAMPLE_SRC)/*.cpp)
APP_CPP_EXAMPLE_SRCS += $(wildcard $(APP_CPP_EXAMPLE_SRC)/*.s)
APP_CPP_EXAMPLE_OBJS1 :=  $(APP_CPP_EXAMPLE_SRCS:.c=.o)
APP_CPP_EXAMPLE_OBJS2 +=  $(APP_CPP_EXAMPLE_OBJS1:.cpp=.o)
APP_CPP_EXAMPLE_OBJS +=  $(APP_CPP_EXAMPLE_OBJS2:.s=.o)

#$(info $$APP_CPP_EXAMPLE_OBJS is [${APP_CPP_EXAMPLE_OBJS}])

$(APP_CPP_EXAMPLE_LIB) : $(APP_CPP_EXAMPLE_OBJS)
	$(Q)$(AR) rcs $@ $^

