# Apps to be compiled within kernel image. For multiple applications, 
# use spacebar to separate names. Applications defined here will not 
# be included in compilation unless you edit the file 
#          os/hellfireos/orca-core/src/orca-core.cpp,
# where you should simple of tasks in each of the cores. 
ORCA_APPLICATIONS := hfunit-example

# Software libraries (experimental)
#ORCA_LIBS := orca-pubsub orca-monitoring hf-printf orca-baremetal-dma-driver
ORCA_LIBS := hfunit

# part of the software, like the device drivers, might know plataform-specific details to parametrize the software.
# one example is the NoC driver and the orca-lib-client. Both of them require details about the Hw NoC, the size of the MpSoC, etc. 
# ORCA_PLATFORM := (orca-mpsoc | single-core | single-core-nn)
ORCA_PLATFORM := hfriscv-with-extcomm
#ORCA_PLATFORM := single-core

# select among the supported OS
#ORCA_OS := (bare-metal/hf-riscv | hellfireos)
ORCA_OS := bare-metal/hf-riscv

export ORCA_APPLICATIONS
export ORCA_LIBS
export ORCA_PLATFORM
export ORCA_OS
