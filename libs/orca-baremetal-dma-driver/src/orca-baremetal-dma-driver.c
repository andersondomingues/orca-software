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
#include <stdio.h>

//cpu-specific signals
volatile uint8_t* sig_stall = (volatile uint8_t*)SIGNAL_CPU_STALL;
volatile uint8_t* sig_intr  = (volatile uint8_t*)SIGNAL_CPU_INTR;

//signals to start the ni
volatile uint32_t* sig_status = (volatile uint32_t*)SIGNAL_STATUS;

//signals to ni programming
volatile uint32_t* sig_addr = (volatile uint32_t*)SIGNAL_PROG_ADDR;
volatile uint32_t* sig_size = (volatile uint32_t*)SIGNAL_PROG_SIZE;

int dma_recv_probe(){
    return (*sig_status & 0x0000ffff) + 2;
}

// 0111 1111 1111 1111 
int dma_init(){
	return *sig_status = (*sig_status) & 0x7fffffff;
}

void dma_set_size(uint32_t size){
	*sig_size = size;
}

void dma_set_addr(uint32_t addr){
	*sig_addr = addr;
}

void dma_set_recv_start(uint32_t flag){
	*sig_status = (flag)
		? (*sig_status | 0x20000000)
		: (*sig_status & ~0x20000000);
}

void dma_set_send_start(int value){
	*sig_status = (value) 
		? *sig_status | 0x40000000
		: *sig_status & ~0x40000000;		
}

// 0100 0000 0000 0000
int dma_get_send_start(){
	return (*sig_status & 0x40000000) > 0;
}

// 0010 0000 0000 0000
int dma_get_recv_start(){
	return (*sig_status & 0x20000000) > 0;
}

void dma_send_start(uint32_t x, uint32_t y, uint8_t* data_ptr, uint32_t size){

	// pack address into first flit 
    uint32_t first_flit = (x << 8) | (0x000000FF & y);

	// pack number of flits into second flit (data size)
	// (size >> 2)  ==> size / 4
	// (0x00000003 | size) ===> size % 4
	uint32_t second_flit = (size >> 2) + !((0x00000003 & size) == 0);
	
	// create a new buffer
    uint32_t buffer[second_flit + 2];
	buffer[0] = first_flit;
	buffer[1] = second_flit;

	//copy the payload
    for(int i = 0; i < second_flit; i++)
		buffer[i + 2] = ((uint32_t*)data_ptr)[i];

	//configure dma 
	dma_set_size(second_flit + 2);
	dma_set_addr((uint32_t)&buffer);
	
	//stall and send, skip one cycle (TODO: necessary?)
	dma_set_send_start(1);
    __asm__ volatile ("nop"); //skip one cycle
	
	//flag off 
	dma_set_send_start(0);
}

uint32_t dma_recv_start(uint32_t* x, uint32_t* y, uint32_t* size, uint8_t* data_ptr){

    // cannot receive without 
    // a packet at the input
    if(!dma_recv_probe()) return -1;

	// configure dma 
	dma_set_size(dma_recv_probe());
	dma_set_addr((uint32_t)data_ptr);
	
    *size = dma_recv_probe();

	dma_set_recv_start(1);
	__asm__ volatile ("nop");
	
	dma_set_recv_start(0);

	*y = data_ptr[0];
	*x = data_ptr[1];

	uint32_t reported_size = data_ptr[4];
	return (*size != reported_size);
}
