/*
 * Implementation file for EXAMPLE-COUNTERS application.
 * Copyright (C) 2018-2019 Anderson Domingues, <ti.andersondomingues@gmail.com>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 simple*/
 
// app built to test basic newlin-nano features. 
// It is meant to be used in bare metal mode. It does not work with HellfireOS

#include "simple.h"
#include "orca-hardware-counters.h"

#ifdef BARE_METAL
#include <stdio.h>
#include <math.h>

int32_t ftoa(float f, int8_t *outbuf, int32_t precision);
int8_t *itoa(int32_t i, int8_t *s, int32_t base);
#endif

//Task for printing values store by CPU counters. 
void libc_test(void){

    float a = 1.2;
    float b = 2.1;
    int ai = 1;
    int bi = 2;
    
    char str[] = "C eh taaaaao legal !!!";

    double num = 123412341234.123456789; 
    char output[30];

    ftoa(a+b,output, 4);   

	printf("Hello!!!!\n");
  
    printf("\n\nTESTING int:  %d + %d = %d\n", ai, bi, ai+bi); // imprime inteiro
    printf("TESTING str: %s !!!!\n", str);   // imprime c_str
    printf("TESTING float str: %s !!!!\n", output); // imprime float em c_str
    printf("TESTING float %%f: %f !!!!\n", a+b); // imprime float direto ?!?!
    printf("TESTING float %%d: %d !!!!\n", (int)a+b); // nao imprime float direto
    ftoa(num,output, 9);
	printf("TESTING double: %s !!!!\n", output); // double

    float power = pow(4,2); // use libm
    ftoa(power,output, 4);
    printf("TESTING libm: %d ^ %d = %s !!!!\n", 4,2, output); // imprime o pow

#ifdef MEMORY_ENABLE_COUNTERS
	printf("MEM0: writes=%u, reads=%u\n", (int)GetCounter_M0_Stores(), (int)GetCounter_M0_Loads());

	#ifdef NI_ENABLE_COUNTERS
	printf("MEM1: writes=%u, reads=%u\n", (int)GetCounter_M1_Stores(), (int)GetCounter_M1_Loads());
	printf("MEM2: writes=%u, reads=%u\n", (int)GetCounter_M2_Stores(), (int)GetCounter_M2_Loads());
	#endif

	printf("---\n");
#endif

	printf("CPU: arith=%d, logical=%d\n", (int)GetCounter_CPU_ArithInstr(), (int)GetCounter_CPU_LogicInstr());
	printf("CPU: shift=%d, branches=%d\n",  (int)GetCounter_CPU_ShiftInstr(), (int)GetCounter_CPU_BranchInstr());
	printf("CPU: jumps=%d, loadstore=%d\n", (int)GetCounter_CPU_JumpsInstr(), (int)GetCounter_CPU_LoadStoreInstr());
	printf("CPU: cycles=%d, stalls=%d\n", (int)GetCounter_CPU_CyclesTotal(), (int)GetCounter_CPU_CyclesStall());
	printf("CPU: hosttime=%d\n", (int)GetCounter_CPU_HostTime());
	printf("---\n");

}

#ifdef BARE_METAL
	void main(){
		libc_test();
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

