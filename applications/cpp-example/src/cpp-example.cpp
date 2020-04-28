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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */
 
#include "cpp-example.h"
#include "orca-hardware-counters.h"
#include <vector>
#include <algorithm>

//Task for printing values store by CPU counters. 
void cpp_example(void){

	printf("Hello World, C!\n");

	std::cout << "Hello World, C++!" << std::endl;

	ExampleBaseClass *obj = new ExampleBaseClass;
	obj->print_something_virt();
	obj->print_something_novirt();
	delete obj;

	obj = new ExampleSubClass;
	obj->print_something_virt();
	obj->print_something_novirt();
	delete obj;

	std::cout << "C++ vector!" << std::endl;
	std::vector<unsigned int> some_ints;
	some_ints.push_back(0x48c9b3e4);
	some_ints.push_back(0x79109b6a);
	some_ints.push_back(0x16155039);
	some_ints.push_back(0xa3635c9a);
	some_ints.push_back(0x8d2f4702);
/*	
	some_ints.push_back(0x38d232ae);
	some_ints.push_back(0x93924a17);
	some_ints.push_back(0x62b895cc);
	some_ints.push_back(0x6130d459);
	some_ints.push_back(0x837c8b44);
	some_ints.push_back(0x3d59b4fe);
	some_ints.push_back(0x444914d8);
	some_ints.push_back(0x3a3dc660);
	some_ints.push_back(0xe5a121ef);
	some_ints.push_back(0xff00866d);
	some_ints.push_back(0xb843b879);
*/
	std::sort(some_ints.begin(), some_ints.end());

	for (auto n : some_ints)
		std::cout << std::hex << n << std::endl;

	std::cout << "All done." << std::endl;

	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("MEM1: writes=%u, reads=%u\n", *M1_COUNTER_STORE, *M1_COUNTER_LOAD);
	printf("MEM2: writes=%u, reads=%u\n", *M2_COUNTER_STORE, *M2_COUNTER_LOAD);
	printf("---\n");

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
		cpp_example();
	}
#endif
