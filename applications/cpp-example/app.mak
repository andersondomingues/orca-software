# Do not modify the lines below
APP_CPP_EXAMPLE_NAME  := cpp-example
APP_CPP_EXAMPLE_DIR   := ./applications/$(APP_CPP_EXAMPLE_NAME)
APP_CPP_EXAMPLE_SRC   := $(APP_CPP_EXAMPLE_DIR)/src
APP_CPP_EXAMPLE_INC   := $(APP_CPP_EXAMPLE_DIR)/include
APP_CPP_EXAMPLE_LIB   := app-$(APP_CPP_EXAMPLE_NAME).a 

INC_DIRS += -I$(APP_CPP_EXAMPLE_INC) -I/opt/gcc-riscv64/riscv-gcc-8.3.0-sifive/riscv64-unknown-elf/include/c++/8.3.0/

CFLAGS += 
#CXXFLAGS += CFLAGS -lstdc++ -std=c++11
CXXFLAGS += 
ASMFLAGS +=

# Update these lines with your source code
APP_CPP_EXAMPLE_SRCS := $(wildcard $(APP_CPP_EXAMPLE_SRC)/*.c)
APP_CPP_EXAMPLE_OBJS :=  $(APP_CPP_EXAMPLE_SRCS:.c=.o)
APP_CPP_EXAMPLE_SRCS += $(wildcard $(APP_CPP_EXAMPLE_SRC)/*.cpp)
APP_CPP_EXAMPLE_OBJS +=  $(APP_CPP_EXAMPLE_SRCS:.cpp=.o)
APP_CPP_EXAMPLE_SRCS += $(wildcard $(APP_CPP_EXAMPLE_SRC)/*.s)
APP_CPP_EXAMPLE_OBJS +=  $(APP_CPP_EXAMPLE_SRCS:.s=.o)


$(APP_CPP_EXAMPLE_LIB) : $(APP_CPP_EXAMPLE_OBJS)
	$(Q)$(AR) rcs $(APP_CPP_EXAMPLE_LIB) $(APP_CPP_EXAMPLE_OBJS) 

