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

		printf("app: received \"%s\" from x=%x y=%d (%d bytes)\n", &buffer[4], x, y, size);

		//change data
		for(int i = 0; i < size; i++)
			if(buffer[i] != '\0')
				buffer[i]++;

		//push data back to the network
		dma_send_start(x, y, buffer, size);
		printf("app: replied \"%s\"\n", &buffer[4]);
	}
}