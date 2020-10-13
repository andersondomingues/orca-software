#include <dma-driver-test.h>
#include <stdio.h>
#include "../../../libs/orca-baremetal-dma-driver/include/orca-baremetal-dma-driver.h"

int main(){

	uint32_t buffer[BUFFER_SIZE];
	struct ni_packet_t* pkt = (struct ni_packet_t*)&buffer;

	uint32_t buffer2[BUFFER_SIZE];
	struct ni_packet_t* pkt2 = (struct ni_packet_t*)&buffer2;

	dma_init();

	pkt->x = 0;
	pkt->y = 0;
	pkt->payload_size = 10;

	pkt2->x = 0;
	pkt2->y = 0;
	pkt2->payload_size = 10;

	for(int i = 0; i < 10; i++){
		(pkt->payload_data)[i] = i + 10;
	}

	//push data back to the network
	dma_send_start(pkt);

	while(!dma_recv_probe());

	while(!get_irq());

	dma_recv_start(pkt2);

	for(int i = 0; i < 10; i++){
		(pkt2->payload_data)[i] = (pkt2->payload_data)[i] + 1;
	}

	(pkt2->payload_data)[0] = dma_get_addr_x();
	(pkt2->payload_data)[1] = dma_get_addr_y();

	//push data back to the network
	dma_send_start(pkt2);
	
	//SYSCALL
	return 0;
}