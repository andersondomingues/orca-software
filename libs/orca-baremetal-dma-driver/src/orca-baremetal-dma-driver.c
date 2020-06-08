/******************************************************************************
 * This file is part of project ORCA. More information on the project
 * can be found at the following repositories at GitHub's website.
 *
 * http://https://github.com/andersondomingues/orca-sim
 * http://https://github.com/andersondomingues/orca-software
 * http://https://github.com/andersondomingues/orca-mpsoc
 * http://https://github.com/andersondomingues/orca-tools
 *
 * Copyright (C) 2018-2020 Anderson Domingues, <ti.andersondomingues@gmail.com>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
******************************************************************************/
#include <orca-baremetal-dma-driver.h>

//cpu-specific signals
volatile uint8_t* sig_stall = (volatile uint8_t*)SIGNAL_CPU_STALL;
volatile uint8_t* sig_intr  = (volatile uint8_t*)SIGNAL_CPU_INTR;

//signals to start the ni
volatile uint8_t* sig_send  = (volatile uint8_t*)SIGNAL_PROG_SEND;
volatile uint8_t* sig_recv  = (volatile uint8_t*)SIGNAL_PROG_RECV;

//signals to check on ni statuses
volatile uint8_t*  sig_send_status = (volatile uint8_t*)SIGNAL_SEND_STATUS;
volatile uint32_t* sig_recv_status = (volatile uint32_t*)SIGNAL_RECV_STATUS;

//signals to ni programming
volatile uint32_t* sig_addr = (volatile uint32_t*)SIGNAL_PROG_ADDR;
volatile uint32_t* sig_size = (volatile uint32_t*)SIGNAL_PROG_SIZE;


//this method will report zero
//only if no packet if at the 
//input. otherwise, it will report
//the size of the incoming packet 
//in bytes
int dma_recv_probe(){
    return *sig_recv_status;
}

int dma_send_start(int x, int y, char* data_ptr, int size){

    uint16_t first_flit = (x << 4) & y;
    uint16_t second_flit = size;

    char buffer[size + 4];

    *((uint16_t*)&(buffer[0])) = first_flit;
    *((uint16_t*)&(buffer[2])) = second_flit;

    for(int i = 0; i < size; i++)
		buffer[i + 4] = data_ptr[i];
	
	//wait until previous send to finish (if any)
	while(*sig_send_status == 0x1);

	//configure dma 
	*sig_size = size + 4;
	*sig_addr = (uint32_t)buffer;
	
	// printf("ni prog size %d, addr 0x%x\n", *sig_size, *sig_addr);
	//stall and send
	*sig_send = 0x1;
    
    __asm__ volatile ("nop"); //skip one cycle
	__asm__ volatile ("nop"); //skip one cycle
	__asm__ volatile ("nop"); //skip one cycle
	__asm__ volatile ("nop"); //skip one cycle
	
	//flag off 
	*sig_send = 0x0;
	*sig_send = 0x0;
	*sig_send = 0x0;
	*sig_send = 0x0;
	
	return 0; //<<- no reason for failing
}

int dma_recv_start(int* x, int* y, int* size, char* data_ptr){

    //cannot receive without 
    //a packet at the input
    if(!dma_recv_probe()) return -1;

	//configure dma 
	*sig_size = dma_recv_probe();
	*sig_addr = (uint32_t)data_ptr;
	
    *size = *sig_size;

	//stall and recv
	*sig_recv = 0x1;
	
	//hold the cpu until no size is given
	while(*sig_recv_status != 0x0);
	
	//flag off 
	*sig_recv = 0x0;

    *x = ((uint16_t*)(data_ptr))[0] >> 4;
    *y = ((uint16_t*)(data_ptr))[0] & 0x00FF;
	
	return 0; //<<- no reason to fail
}
