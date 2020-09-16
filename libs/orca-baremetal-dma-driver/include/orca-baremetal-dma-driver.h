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
#ifndef _ORCA_BAREMETAL_DMA_DRIVER_H
#define _ORCA_BAREMETAL_DMA_DRIVER_H

// #include "/home/adomingues/gaph/orca-sim/platforms/hfriscv-with-extcomm/include/MemoryMap.h"

// @TODO maybe move these to the Configuration.mk file
#define ORCA_MEMORY_BASE_1 0x00000000
#define ORCA_MEMORY_SIZE_1 0x00000080
#define ORCA_MEMORY_BASE_2 0x00000080
#define ORCA_MEMORY_SIZE_2 0x00000080

// control wires (cpu <-> dma)
#define SIGNAL_CPU_STALL    0x40410000  /* 8 bits */
#define SIGNAL_CPU_INTR     0x40410001
// #define SIGNAL_SEND_STATUS  0x40410002
// 0x40410003
// #define SIGNAL_RECV_STATUS  0x40410004
// 0x40410005
// 0x40410006
// 0x40410007
// #define SIGNAL_PROG_SEND    0x40410008
// #define SIGNAL_PROG_RECV    0x40410009
// 0x4041000A
// 0x4041000B
// #define SIGNAL_PROG_ADDR    0x4041000C  /* 32 bits */
// #define SIGNAL_PROG_SIZE    0x40410010

// tile indentifier, hardware fixed
// #define MAGIC_TILE_ID       0x40411000

#define SIGNAL_TILE_ID      0xe0ff8000
#define SIGNAL_STATUS       0xe0ff8010
#define SIGNAL_PROG_ADDR    0xe0ff8020  /* 32 bits */
#define SIGNAL_PROG_SIZE    0xe0ff8030

#include <inttypes.h>

int dma_init();
/**
 * @brief Checks whether there are any packets to be 
 * received from the network. 
 * 
 * @return int Returns 1 when a packet is available, Zero otherwise. 
 */
int dma_recv_probe();

/**
 * @brief Sends a packet through the network
 * 
 * @param x The X-axis address of the target device
 * @param y The Y-axis address of the target device
 * @param size The number of bytes of the payload
 * @param data_ptr A pointer to the data to be transmitted
 * @return int Returns Zero is data is succefully transmitted, 
 *  error code otherwirse.
 */
uint32_t dma_recv_start(uint32_t* x, uint32_t* y, uint32_t* size, uint8_t* data_ptr);

/**
 * @brief Receives a packet from the network.
 * 
 * @param x A pointer to a variable to store the X-address of the sender 
 * @param y A pointer to a variable to store the Y-address of the sender
 * @param data_ptr A pointer to the place where the payload will be writen
 * @param size The number of bytes of the received packet
 * @return int Returns Zero if the packet has being succefully received, -1
 * if there are no packets to be received, error code otherwise.
 */
void dma_send_start(uint32_t x, uint32_t y, uint8_t* data_ptr, uint32_t size);

#endif // _ORCA_BAREMETAL_DMA_DRIVER_H