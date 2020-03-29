#define IRQ_NOC_READ 0x100
#define NI_PACKET_SIZE 64

#define MAGIC_TILE_ID       (uint16_t*)0x40411000  

//dma-ni program signals 0 1 2 (3)
#define SIGNAL_CPU_STALL    (uint8_t*)(0x40410000)
#define SIGNAL_CPU_INTR     (uint8_t*)(0x40410001)
#define SIGNAL_SEND_STATUS  (uint8_t*)(0x40410002)

//dma-ni recv status 4-7
#define SIGNAL_RECV_ADDR    (uint32_t*)(0x40410004)

//programming low 8 9 (10 11)
#define SIGNAL_PROG_SEND    (uint8_t*)(0x40410008)
#define SIGNAL_PROG_RECV    (uint8_t*)(0x40410009)

//programming addresses 12-15 16-19
#define SIGNAL_PROG_ADDR    (uint32_t*)(0x4041000C)
#define SIGNAL_PROG_SIZE    (uint32_t*)(0x40410010)

//raise/lower macroses
#define RAISE(x) *x=0x1
#define LOWER(x) *x=0x0

int32_t ni_ready(void);
int32_t ni_flush(uint16_t pkt_size);
int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size);
int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size);
int32_t ni_get_next_size();