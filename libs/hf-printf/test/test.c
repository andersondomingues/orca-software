/*
simple app to perform minimal testing for hf_printf
*/
#include "hf-printf.h"
#include "orca-hardware-counters.h"

void main(void){

    float a = 1.2;
    float b = 2.1;
    int ai = 1;
    int bi = 2;
    const char str[] = "C eh taaaaao legal !!!";
    double num = 1234.123456789; 
    char output[30];

    ftoa(a+b,output, 4);
	printf("Hello!!!!\n");
    printf("\n\nTESTING int:  %d + %d = %d\n", ai, bi, ai+bi); // imprime inteiro
    printf("TESTING str: %s !!!!\n", str);   // imprime c_str
    printf("TESTING float str: %s !!!!\n", output); // imprime float em c_str
    printf("TESTING float: %f !!!!\n", a+b); // imprime float direto ?!?!
    printf("TESTING float (int): %d !!!!\n", (int)a+b); // nao imprime float direto convertido
    ftoa(num,output, 9);
	printf("TESTING double: %s !!!!\n", output); // double

#ifdef MEMORY_ENABLE_COUNTERS
	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("MEM1: writes=%u, reads=%u\n", *M1_COUNTER_STORE, *M1_COUNTER_LOAD);
	printf("MEM2: writes=%u, reads=%u\n", *M2_COUNTER_STORE, *M2_COUNTER_LOAD);
	printf("---\n");
#endif

	printf("CPU: arith=%u, logical=%u\n",   *CPU_COUNTER_ARITH, *CPU_COUNTER_LOGICAL);
	printf("CPU: shift=%u, branches=%u\n",  *CPU_COUNTER_SHIFT, *CPU_COUNTER_BRANCHES);
	printf("CPU: jumps=%u, loadstore=%u\n", *CPU_COUNTER_JUMPS, *CPU_COUNTER_LOADSTORE);
	printf("CPU: cycles=%u, stalls=%u\n", *CPU_COUNTER_CYCLES_TOTAL, *CPU_COUNTER_CYCLES_STALL);	
	printf("CPU: hosttime=%u\n", *CPU_COUNTER_HOSTTIME);
	printf("---\n");
}

