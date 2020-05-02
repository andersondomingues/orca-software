# Apps to be compiled within kernel image. For multiple applications, 
# use spacebar to separate names. Applications defined here will not 
# be included in compilation unless you edit the file 
#          extensions/orca-core/src/orca-core.cpp,
# where you should simple of tasks in each of the cores. 
ORCA_APPLICATIONS := cpp-example
#ORCA_APPLICATIONS := producer-consumer-pubsub producer-consumer app-spawner app-bloater deadline-monitor

# Software extensions (experimental)
#ORCA_EXTENSIONS := orca-core orca-pubsub orca-monitoring 
ORCA_EXTENSIONS := orca-monitoring 

# part of the software, like the device drivers, might know plataform-specific details to parametrize the software.
# one example is the NoC driver and the orca-lib-client. Both of them require details about the Hw NoC, the size of the MpSoC, etc. 
# ORCA_PLATFORM := (orca-mpsoc | single-core | single-core-nn)
ORCA_PLATFORM := single-core

# select among the supported OS
#ORCA_OS := (bare-metal/hf-riscv | hellfireos)
ORCA_OS := bare-metal/hf-riscv

# ==================================================================[ ORCA ]
# Number of cycles before calling the frequency analisys tool. Shorter
# values may compromise the performance of the simulation, while higher
# values may provide inaccurate measurements of the achieved frequency.
ORCA_EPOCH_LENGTH  := 10000000
#ORCA_EPOCH_LENGTH  := 50000000

# Number of pulses to simulate. Set to INF to simulate indefinitely. 
ORCA_EPOCHS_TO_SIM := INF
#ORCA_EPOCHS_TO_SIM := 3

# ========================================================================
# GENERATION OF COMPILATION PARAMETERS STARTS HERE.
# DO NOT MODIFY BELOW THIS LINE!
# ========================================================================

#ORCA parameters
ifneq ($(ORCA_EPOCHS_TO_SIM), INF)
	COMPLINE := $(COMPLINE) -DORCA_EPOCHS_TO_SIM=$(ORCA_EPOCHS_TO_SIM)
endif

COMPLINE := $(COMPLINE) -DORCA_EPOCH_LENGTH=$(ORCA_EPOCH_LENGTH)

ifeq ($(ORCA_ENABLE_MULTITHREADING), YES)
	COMPLINE := $(COMPLINE) -DORCA_ENABLE_MULTITHREADING
endif 

export COMPLINE
export ORCA_APPLICATIONS
export ORCA_OS
export ORCA_PLATFORM
