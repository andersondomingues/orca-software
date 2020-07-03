# Do not modify the lines below
HFUNIT_NAME  := hfunit
HFUNIT_DIR   := $(ORCA_SW_DIR)/libs/$(HFUNIT_NAME)
HFUNIT_SRC   := $(HFUNIT_DIR)/src
HFUNIT_INC   := $(HFUNIT_DIR)/include
HFUNIT_LIB   := lib-$(HFUNIT_NAME).a

USE_LIBC = 1

INC_DIRS += -I$(HFUNIT_INC)

CFLAGS += 

# Update these lines with your source code
HFUNIT_SRCS := $(wildcard $(HFUNIT_SRC)/*.c)
HFUNIT_OBJS :=  $(HFUNIT_SRCS:.c=.o)

$(HFUNIT_LIB) : $(HFUNIT_OBJS)
	$(Q)$(AR) rcs $(HFUNIT_LIB) $(HFUNIT_OBJS) 
