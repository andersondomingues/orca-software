# place here only OS depedent definitions 

# ============================================================[ HELLFIREOS ]
#arch confs.
ARCH = riscv/hf-riscv

export OS_STATIC_LIB := hellfire-os.a

#dir config.
HFOS_DIR = $(ORCA_SW_DIR)/os/hellfireos/hellfireos
SRC_DIR = $(HFOS_DIR)

CPU_ARCH = \"$(ARCH)\"
MAX_TASKS = 30
MUTEX_TYPE = 0
MEM_ALLOC = 3
HEAP_SIZE = 500000
FLOATING_POINT = 0
# Set level of logging for the HellfireOS kernel. 
# 0 => disabled 
# 1 => interruption and dispatch information (default)
# 2 => same as level one plus calls to kernel functions 
KERNEL_LOG = 1

#includes for kernel parts
include $(HFOS_DIR)/arch/$(ARCH)/arch.mak
include $(HFOS_DIR)/lib/lib.mak
include $(HFOS_DIR)/drivers/noc.mak
include $(HFOS_DIR)/sys/kernel.mak

# common definition to all software modules
INC_DIRS += -I $(HFOS_DIR)/lib/include \
			-I $(HFOS_DIR)/sys/include \
			-I $(HFOS_DIR)/drivers/noc/include \

CFLAGS += -DCPU_ARCH=$(CPU_ARCH) \
	-DMAX_TASKS=$(MAX_TASKS) -DMEM_ALLOC=$(MEM_ALLOC) \
	-DHEAP_SIZE=$(HEAP_SIZE) -DMUTEX_TYPE=$(MUTEX_TYPE) \
	-DFLOATING_POINT=$(FLOATING_POINT) \
	-DKERNEL_LOG=$(KERNEL_LOG) 

export KERNEL_LOG_LEVEL

$(OS_STATIC_LIB):
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Kernel ...               \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)make hal
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Kernel ...  libc             \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)make libc
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Kernel ...     noc           \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)make noc
	@echo "$'\e[7m==================================\e[0m"
	@echo "$'\e[7m  Making Kernel ...   kernel            \e[0m"
	@echo "$'\e[7m==================================\e[0m"
	$(Q)make kernel 
	$(Q)$(AR) rcs $(OS_STATIC_LIB) *.o