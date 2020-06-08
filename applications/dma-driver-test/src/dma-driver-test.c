#include <dma-driver-test.h>
#include "../../../libs/orca-baremetal-dma-driver/include/orca-baremetal-dma-driver.h"


int main(){

	//periodically bursts messages through the network 
	while(1){

		char buffer[BUFFER_SIZE];

		for(int i = 0; i < BUFFER_SIZE; i++)
			buffer[i] = 'a' + i;

		dma_send_start(0, 0, buffer, BUFFER_SIZE);
	}
}