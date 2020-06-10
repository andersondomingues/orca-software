#ifndef __ORCA_HARDWARE_COUNTERS_H
#define __ORCA_HARDWARE_COUNTERS_H

//either use stdint.h or hellfire.h. both define uint32_t & cia
#ifdef BARE_METAL
    #include "stdint.h"
#endif
#ifdef HELLFIREOS
    #include "hellfire.h"
#endif

// the memory map is defined in the platform design, in the orca-sim repo.
#include "MemoryMap.h"

//0x403F1xxx => memory mapped counters
#ifdef MEMORY_ENABLE_COUNTERS
extern volatile uint32_t* M0_COUNTER_STORE;
extern volatile uint32_t* M0_COUNTER_LOAD;

	#ifdef NI_ENABLE_COUNTERS
	extern volatile uint32_t* M1_COUNTER_STORE;
	extern volatile uint32_t* M1_COUNTER_LOAD;
	extern volatile uint32_t* M2_COUNTER_STORE;
	extern volatile uint32_t* M2_COUNTER_LOAD;
	#endif

#endif

#ifdef MEMORY_ENABLE_COUNTERS
uint32_t GetCounter_M0_Stores();
uint32_t GetCounter_M0_Loads();

	#ifdef NI_ENABLE_COUNTERS
	uint32_t GetCounter_M1_Stores();
	uint32_t GetCounter_M1_Loads();
	uint32_t GetCounter_M2_Stores();
	uint32_t GetCounter_M2_Loads();
	#endif

#endif

#ifdef HFRISCV_ENABLE_COUNTERS
extern volatile uint32_t* CPU_COUNTER_ARITH;
extern volatile uint32_t* CPU_COUNTER_LOGICAL;
extern volatile uint32_t* CPU_COUNTER_SHIFT;
extern volatile uint32_t* CPU_COUNTER_BRANCHES;
extern volatile uint32_t* CPU_COUNTER_JUMPS;
extern volatile uint32_t* CPU_COUNTER_LOADSTORE;
extern volatile uint32_t* CPU_COUNTER_HOSTTIME;
extern volatile uint32_t* CPU_COUNTER_CYCLES_TOTAL;
extern volatile uint32_t* CPU_COUNTER_CYCLES_STALL;

uint32_t GetCounter_CPU_ArithInstr();
uint32_t GetCounter_CPU_LogicInstr();
uint32_t GetCounter_CPU_ShiftInstr();
uint32_t GetCounter_CPU_BranchInstr();
uint32_t GetCounter_CPU_JumpsInstr();
uint32_t GetCounter_CPU_LoadStoreInstr();
uint32_t GetCounter_CPU_HostTime();
uint32_t GetCounter_CPU_CyclesTotal();
uint32_t GetCounter_CPU_CyclesStall();

#endif

//0x403F15xx => router wires
#ifdef ROUTER_ENABLE_COUNTERS
extern volatile uint32_t* ROUTER_COUNTER_ACTIVE;
uint32_t GetCounter_ROUTER_Active();
#endif

#endif /* __ORCA_HARDWARE_COUNTERS_H */
