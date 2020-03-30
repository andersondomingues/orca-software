# =======================================================[ GLOBAL SETTINGS ]
# Flags we set here will be propagated to all hardware modules (not sw)
# Currently adopted flags:
# -Wall, -Wextra, Werror: activate a lot of warning at compile time
# -g: attach debug information to the generated executable
# -std: required by old GCC to set c++17 as default the c++ 
# -march, -mtune: optimize code for current machine architecture
# -lasan, -fsanitize: add memory sanitizer to code
GLOBAL_SETTINGS := -Wall -Wextra -Werror -g -std=c++17 -O3 -march=native -mtune=native 
#-lasan -fsanitize=address

# Apps to be compiled within kernel image. For multiple applications, 
# use spacebar to separate names. Applications defined here will not 
# be included in compilation unless you edit the file 
#          extensions/orca-core/src/orca-core.cpp,
# where you should set the spawn of tasks in each of the cores. 
ORCA_APPLICATIONS := simple
#ORCA_APPLICATIONS := producer-consumer-pubsub producer-consumer app-spawner app-bloater deadline-monitor

# Software extensions (experimental)
#ORCA_EXTENSIONS := orca-core orca-pubsub orca-monitoring
ORCA_EXTENSIONS := orca-monitoring

# part of the software, like the device drivers, might know plataform-specific details to parametrize the software.
# one example is the NoC driver and the orca-lib-client. Both of them require details about the Hw NoC, the size of the MpSoC, etc. 
# PLATFORM := (orca-dma | single-core)
PLATFORM := single-core

# select among the supported OS
#ORCA_OS := (bare-metal/hf-riscv | hellfireos)
ORCA_OS := bare-metal/hf-riscv

# ==================================================================[ ORCA ]
# Width (x-axis coordinate) of the network-on-chip. Cannot be zero,
# otherwise simulation won't compile.
#ORCA_NOC_HEIGHT := 3
# Width (y-axis coordinate) of the network-on-chip. Cannot be zero,
# otherwise simulation won't compile.
#ORCA_NOC_WIDTH  := 2

# Number of cycles before calling the frequency analisys tool. Shorter
# values may compromise the performance of the simulation, while higher
# values may provide inaccurate measurements of the achieved frequency.
ORCA_EPOCH_LENGTH  := 10000000
#ORCA_EPOCH_LENGTH  := 50000000

# Number of pulses to simulate. Set to INF to simulate indefinitely.
ORCA_EPOCHS_TO_SIM := INF
#ORCA_EPOCHS_TO_SIM := 3

# ===============================================================[ MEMORY ]
# Enable counter for read and write operations (depletes performance).
MEMORY_ENABLE_COUNTERS := YES

# ==============================================================[ HFRISCV ]
# Enable counter for instructions' classes (depletes performance).
HFRISCV_ENABLE_COUNTERS := YES

# ==============================================================[ ROUTER ]
# Enable counters for number of active cycles
#ROUTER_ENABLE_COUNTERS := NO

# ========================================================================
# GENERATION OF COMPILATION PARAMETERS STARTS HERE.
# DO NOT MODIFY BELOW THIS LINE!
# ========================================================================

#ORCA parameters
ifneq ($(ORCA_EPOCHS_TO_SIM), INF)
	COMPLINE := $(COMPLINE) -DORCA_EPOCHS_TO_SIM=$(ORCA_EPOCHS_TO_SIM)
endif

COMPLINE := $(COMPLINE) \
	-DORCA_EPOCH_LENGTH=$(ORCA_EPOCH_LENGTH)
#	-DORCA_NOC_HEIGHT=$(ORCA_NOC_HEIGHT) \
#	-DORCA_NOC_WIDTH=$(ORCA_NOC_WIDTH) \

ifeq ($(ORCA_ENABLE_MULTITHREADING), YES)
	COMPLINE := $(COMPLINE) -DORCA_ENABLE_MULTITHREADING
endif 

ifeq ($(MEMORY_ENABLE_COUNTERS), YES)
	COMPLINE := $(COMPLINE) -DMEMORY_ENABLE_COUNTERS
endif

#hfriscv parameters
ifeq ($(HFRISCV_ENABLE_COUNTERS), YES)
	COMPLINE := $(COMPLINE) -DHFRISCV_ENABLE_COUNTERS
endif

#router parameters
ifeq ($(ROUTER_ENABLE_COUNTERS), YES)
	COMPLINE := $(COMPLINE) -DROUTER_ENABLE_COUNTERS
endif

export COMPLINE
export GLOBAL_SETTINGS
#export ORCA_NOC_HEIGHT
#export ORCA_NOC_WIDTH
export ORCA_APPLICATIONS
#export ORCA_EXTENSIONS
export ORCA_OS