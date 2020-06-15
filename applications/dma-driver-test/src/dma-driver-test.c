#include <dma-driver-test.h>
#include <stdio.h>
#include "../../../libs/orca-baremetal-dma-driver/include/orca-baremetal-dma-driver.h"

int main(){

	char buffer[BUFFER_SIZE];

	//periodically bursts messages through the network 
	while(1){

		//holds until data is available from the 
		//network
		while(!dma_recv_probe());

		int x, y, size;

		//receive data
		dma_recv_start(&x, &y, &size, buffer);

		printf("%s\n", &buffer[4]);

		//change data
		for(int i = 0; i < BUFFER_SIZE; i++)
			buffer[i]++;

		//push data back to the network
		// dma_send_start(x, y, buffer, size);
	}
}