// the memory map is defined in the platform design, in the orca-sim repo.
#include "MemoryMap.h"

#define IRQ_NOC_READ 0x100
#define NI_PACKET_SIZE 64

//raise/lower macroses
#define RAISE(x) *x=0x1
#define LOWER(x) *x=0x0

int32_t ni_ready(void);
int32_t ni_flush(uint16_t pkt_size);
int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size);
int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size);
int32_t ni_get_next_size();
