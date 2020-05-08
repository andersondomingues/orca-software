/*
 * Implementation file for mnist application.
 * Copyright (C) 2018-2020 Alexandre Amory, <amamory@gmail.com>
 * This file is part of project URSA (http://https://github.com/andersondomingues/ursa).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

// colocar credito da onde esse codigo foi tirado ?!?!? acho q o Jura sabe !
#include "simple-dma.h"
#include "orca-hardware-counters.h"

#ifdef BARE_METAL
int32_t ftoa(float f, int8_t *outbuf, int32_t precision);
int8_t *itoa(int32_t i, int8_t *s, int32_t base);
#endif


float SetDMA(uint32_t size, float* input_base_addr, float* weight_base_addr){
	volatile uint32_t * burst_size = DMA_BURST_SIZE;
	volatile uint32_t * iaddr = DMA_INPUT_MEM_ADDR;
	volatile uint32_t * waddr = DMA_WEIGHT_MEM_ADDR;
	volatile float * dma_out = DMA_MAC_OUT;
	volatile uint8_t * start = SIGNAL_DMA_PROG;
	float out;
	//uint32_t start_cycles,stall_cycles;
	//char str[40];

	*burst_size = size;
	*iaddr = input_base_addr; // op1
	*waddr = weight_base_addr;// op2 

	// uncomment the following line to measure the # of instructions
	//start_cycles = *CPU_COUNTER_CYCLES_TOTAL;
	//stall_cycles = *CPU_COUNTER_CYCLES_STALL;
	*start = 1;
	// then the processor goes into stall to perform the DMA ...
	// when it wakes up again, the result can be read

	out =  *dma_out; // this cast is required to convert to float
	// uncomment the following line to measure the # of instructions
	//printf("took: cycles=%u - stall cycles=%u\n", *CPU_COUNTER_CYCLES_TOTAL-start_cycles, *CPU_COUNTER_CYCLES_STALL-stall_cycles);
	// uncomment the following two lines to print the result
	//ftoa(out,str,6);
	//printf("result SetDMA: %s - 0x%X\n", str, dma_out);
	return out;
}

// place the MAC operands into their specific sections mapped into the NN memories
// section attribute cannot be specified for local variables
float op1[] __attribute__((section (".input_mem")))  = {1.1,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0} ;
float op2[] __attribute__((section (".weight_mem"))) = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0} ;

void simple_dma (void) {

	printf("### STARTING! ###.\n"); 
	float result;
	char str[20];

	result = SetDMA(10,op1, op2);
	ftoa(result,str,6);
	printf("RESULT: %s.\n",str); 
	// the expected result is 285.100006
	if (result < 285.09 || result > 285.11){
		printf("\n\n\nFAILED !!!!\n\n\n");
	}else{
		printf("\n\n\nTEST PASSED !!!!\n\n\n");
	}
	printf("### FINISHED! ###.\n"); 

#ifdef MEMORY_ENABLE_COUNTERS
	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("---\n");
#endif

	printf("CPU: arith=%u, logical=%u\n",   *CPU_COUNTER_ARITH, *CPU_COUNTER_LOGICAL);
	printf("CPU: shift=%u, branches=%u\n",  *CPU_COUNTER_SHIFT, *CPU_COUNTER_BRANCHES);
	printf("CPU: jumps=%u, loadstore=%u\n", *CPU_COUNTER_JUMPS, *CPU_COUNTER_LOADSTORE);
	printf("CPU: cycles=%u, stalls=%u\n", *CPU_COUNTER_CYCLES_TOTAL, *CPU_COUNTER_CYCLES_STALL);	
	printf("CPU: hosttime=%u\n", *CPU_COUNTER_HOSTTIME);
	printf("---\n");

#ifdef HELLFIREOS
	hf_kill(hf_selfid());
#endif
}

#ifdef BARE_METAL

void main(){
	simple_dma();
}


union float_long{
	float f;
	int32_t l;
	uint32_t u;
};

int8_t *itoa(int32_t i, int8_t *s, int32_t base){
	int8_t *ptr = s, *ptr1 = s, tmp_char;
	int32_t tmp_value;

	if (base < 2 || base > 36) {
		*s = '\0';
		return s;
	}
	do {
		tmp_value = i;
		i /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - i * base)];
	} while (i);
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return s;
}


int32_t ftoa(float f, int8_t *outbuf, int32_t precision){
	int32_t mantissa, int_part, frac_part, exp2, i;
	int8_t *p;
	union float_long fl;

	p = outbuf;

	if (f < 0.0){
		*p = '-';
		f = -f;
		p++;
	}

	fl.f = f;

	exp2 = (fl.l >> 23) - 127;
	mantissa = (fl.l & 0xffffff) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 31){
		return -1;	/* too large */
	}else{
		if (exp2 < -23){
//			return -1;	/* too small */
		}else{
			if (exp2 >= 23){
				int_part = mantissa << (exp2 - 23);
			}else{
				if (exp2 >= 0){
					int_part = mantissa >> (23 - exp2);
					frac_part = (mantissa << (exp2 + 1)) & 0xffffff;
				}else{
					frac_part = (mantissa & 0xffffff) >> (-(exp2 + 1));
				}
			}
		}
	}

	if (int_part == 0){
		*p = '0';
		p++;
	}else{
		itoa(int_part, p, 10);
		while(*p) p++;
	}
	*p = '.';
	p++;

	for (i = 0; i < precision; i++){
		frac_part = (frac_part << 3) + (frac_part << 1);
		*p = (frac_part >> 24) + '0';
		p++;
		frac_part = frac_part & 0xffffff;
	}

	*p = 0;

	return 0;
}	
#endif
