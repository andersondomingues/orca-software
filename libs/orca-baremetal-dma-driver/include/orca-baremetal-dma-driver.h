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

#include "/home/adomingues/gaph/orca-sim/platforms/hfriscv-with-extcomm/include/MemoryMap.h"
#include <inttypes.h>

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
int dma_recv_start(int* x, int* y, int* size, char* data_ptr);

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
int dma_send_start(int x, int y, char* data_ptr, int size);

#endif // _ORCA_BAREMETAL_DMA_DRIVER_H