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
volatile uint8_t* sig_intr  = (volatile uint8_t*)SIGNAL_CPU_INTR;

//signals to start the ni
volatile uint32_t* sig_status = (volatile uint32_t*)SIGNAL_STATUS;

//signals to ni programming
volatile uint32_t* sig_addr = (volatile uint32_t*)SIGNAL_PROG_ADDR;
volatile uint32_t* sig_size = (volatile uint32_t*)SIGNAL_PROG_SIZE;

//address to which the ni would sent the pkt
volatile uint32_t* sig_dest = (volatile uint32_t*)SIGNAL_DESTINATION;

//tile id
volatile uint32_t* sig_tile_id = (volatile uint32_t*)SIGNAL_TILE_ID;

int get_irq(){
	return *sig_intr & 0x00000001; 
}

int dma_get_addr_x(){
	return (*sig_tile_id >> 8) & 0x000000ff;
}

int dma_get_addr_y(){
	return (*sig_tile_id & 0x000000ff);
}


int dma_recv_probe(){
    return (*sig_status & 0x0000ffff);
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


void dma_set_destination(int x, int y){
	
	*sig_dest = (x << 8) | (y & 0x000000FF);
	*sig_dest &= 0x0000FFFF;
	// 0000 0000 0000 0000 0000 0000 0000 0000 
}

void dma_send_start(struct ni_packet_t* pkt){
	
	//configure dma 
	dma_set_destination(pkt->x, pkt->y);
	dma_set_size(pkt->payload_size);
	dma_set_addr((uint32_t)(pkt->payload_data));
	
	//stall and send, skip one cycle (TODO: necessary?)
	dma_set_send_start(1);
    __asm__ volatile ("nop"); //skip one cycle
	
	//flag off 
	dma_set_send_start(0);
}


uint32_t dma_recv_start(struct ni_packet_t* pkt){

    // cannot receive without 
    // a packet at the input
    if(!dma_recv_probe()) return -1;

	// configure dma 
	dma_set_size(pkt->payload_size);
	dma_set_addr((uint32_t)(pkt->payload_data));
	
	uint32_t expected_size = dma_recv_probe();

	dma_set_recv_start(1);
	__asm__ volatile ("nop");
	
	dma_set_recv_start(0);

	// return number dropped flits
	return expected_size - pkt->payload_size;
}
