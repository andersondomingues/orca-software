#include <dma-driver-test.h>
#include <stdio.h>
#include "../../../libs/orca-baremetal-dma-driver/include/orca-baremetal-dma-driver.h"

int main(){

	uint8_t buffer[BUFFER_SIZE];

	dma_init();

	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t size = 10;

	for(int i = 0; i < 10; i++){
		buffer[i] = i + 10;
	}

	//push data back to the network
	dma_send_start(x, y, buffer, size);

	while(!dma_recv_probe());
	
	dma_recv_start(&x, &y, &size, buffer);
	
	for(int i = 0; i < 10; i++){
		buffer[i] = buffer[i] + 1;
	}

	//push data back to the network
	dma_send_start(x, y, buffer, size);

	//SYSCALL
	return 0;
}