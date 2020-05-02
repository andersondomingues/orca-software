# Do not modify the lines below
ORCA_MONITORING_NAME  := orca-monitoring
ORCA_MONITORING_DIR   := $(ORCA_SW_DIR)/extensions/$(ORCA_MONITORING_NAME)
ORCA_MONITORING_SRC   := $(ORCA_MONITORING_DIR)/src
ORCA_MONITORING_INC   := $(ORCA_MONITORING_DIR)/include
ORCA_MONITORING_LIB   := ext-$(ORCA_MONITORING_NAME).a

INC_DIRS += -I$(ORCA_MONITORING_INC)

CFLAGS += 

# Update these lines with your source code
ORCA_MONITORING_SRCS := $(wildcard $(ORCA_MONITORING_SRC)/*.c)
ORCA_MONITORING_OBJS :=  $(ORCA_MONITORING_SRCS:.c=.o)

$(ORCA_MONITORING_LIB) : $(ORCA_MONITORING_OBJS)
	$(Q)$(AR) rcs $(ORCA_MONITORING_LIB) $(ORCA_MONITORING_OBJS) 
