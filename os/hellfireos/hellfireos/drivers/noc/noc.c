/**
 * @file noc.c
 * @author Sergio Johann Filho
 * @date April 2016
 *
 * @section LICENSE
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file 'doc/license/gpl-2.0.txt' for more details.
 *
 * @section DESCRIPTION
 *
 * NoC (Network-on-Chip) interconnect driver. This driver works with 16-bit word packets and
 * a basic communication protocol between the cores and the network interface, provided by
 * ni_ready(), ni_flush(), ni_read_packet() and ni_write_packet() helper functions (defined
 * on ni_generic.h). The generic interfaces were defined for a typical Hermes NoC configuration
 * (ni_hermes.c) but may be adapted for other NoCs or configurations if network packets are
 * abstracted in the format presented below.
 *
 * Packet format is as follows:
 *
 \verbatim
  2 bytes   2 bytes   2 bytes   2 bytes   2 bytes   2 bytes   2 bytes   2 bytes       ....
 --------------------------------------------------------------------------------------------------
 |tgt_cpu  |payload  |src_cpu  |src_port |tgt_port |msg_size |seq      |channel  |  ... data ...  |
 --------------------------------------------------------------------------------------------------
 \endverbatim
 *
 * The platform should include the following macros:
 *
 * NOC_INTERCONNECT			intra-chip interconnection type
 * CPU_ID				a unique sequential number for each core (unused, see hf_cpuid(void))
 * NOC_WIDTH				number of columns of the 2D mesh
 * NOC_HEIGHT				number of rows of the 2D mesh
 * NOC_PACKET_SIZE			packet size (in 16 bit flits)
 * NOC_PACKET_SLOTS			number of slots in the shared packet queue per core
 */

#include <hellfire.h>
#include <noc.h>
#include <ni_orca.h>

//externs
uint16_t pktdrv_ports[MAX_TASKS];
struct queue *pktdrv_tqueue[MAX_TASKS];
struct queue *pktdrv_queue;
int32_t (*pktdrv_callback)(uint16_t *buf);

/**
 * @brief NoC driver: initializes the network interface.
 *
 * A queue for the packet driver is initialized with NOC_PACKET_SLOTS capacity (in packets).
 * The queue is populated with empty packets (pointers to dinamically allocated memory areas)
 * which will be used (shared) among all tasks for the reception of data. The hardware is reset
 * and the NoC interrupt handler is registered. This routine is called during the system boot-up
 * and is dependent on the architecture implementation.
 */
void ni_init(void)
{
	int32_t i;
	void *ptr;

	kprintf("\nKERNEL: this is core #%d", hf_cpuid());
	kprintf("\nKERNEL: NoC queue init, %d packets", NOC_PACKET_SLOTS);

	pktdrv_queue = hf_queue_create(NOC_PACKET_SLOTS);
	if (pktdrv_queue == NULL) panic(PANIC_OOM);

	for (i = 0; i < MAX_TASKS; i++)
		pktdrv_ports[i] = 0;

	for (i = 0; i < NOC_PACKET_SLOTS; i++){
		ptr = hf_malloc(sizeof(int16_t) * NOC_PACKET_SIZE);
		if (ptr == NULL) panic(PANIC_OOM);
		hf_queue_addtail(pktdrv_queue, ptr);
		//printf("malloc => 0x%x\n", ptr);
	}

	uint32_t im = _di();
	i = ni_flush(NOC_PACKET_SIZE);
	_ei(im);

	if (i){
		_irq_register(IRQ_NOC_READ, (funcptr)ni_isr);
		_irq_mask_set(IRQ_NOC_READ);
		kprintf("\nKERNEL: NoC driver registered");
	}else{
		kprintf("\nKERNEL: NoC NI init failed");
	}
}

/**
 * @brief NoC driver: network interface interrupt service routine.
 *
 * This routine is called by the second level of interrupt handling. An interrupt from the network
 * interface means a full packet has arrived. The packet header is decoded and the target port is
 * identified. A reference to an empty packet is removed from the pool of buffers (packets), the
 * contents of the empty packet are filled with flits from the hardware queue and the reference is
 * put on the target task (associated to a port) queue of packets. There is one queue per task of
 * configurable size (individual queues are elastic if size is zero, limited to the size of free
 * buffer elements from the common pool). If port 0xffff (65535) is used as the target, the packet
 * is passed to a callback. This mechanism can be used to build custom OS functions (such as user
 * defined protocols, RPC or remote system calls). Port 0 is used as a discard function, for testing
 * purposes.
 */
 
void ni_isr(void *arg)
{
	int32_t k;
	uint16_t *buf_ptr;

	//Since we know the size of the received packet, we ask the driver to copy only 
	//the necessary bytes. The size of the packet is written by the NI to the 
	//sig_recv_status signal (see drv). We need to know the size of the packet before
	//receiving it, so it can be flushed case no more room is available
	uint16_t act_pkt_size = ni_get_next_size();

	//get an slot from the global queue
	buf_ptr = hf_queue_remhead(pktdrv_queue);

	//if slot is valid (is NULL when no slot can be acquired)
	if (buf_ptr) {

		//Since we know the size of the received packet, we ask the driver to copy
		//only the necessary bytes. The size of the packet is written by the NI to
		//the sig_recv_status signal (see drv).
		ni_read_packet(buf_ptr, act_pkt_size);

		//Since have an arbitrary number of flits per packet, we !!!!!!!!!!!!!!
		//cannot determine packet integrity from its size.
		//COMMENTED OUT >> if (buf_ptr[PKT_PAYLOAD] != NOC_PACKET_SIZE - 2){
		//COMMENTED OUT >>     hf_queue_addtail(pktdrv_queue, buf_ptr);
		//COMMENTED OUT >>     return;
		//COMMENTED OUT >> }

		//get rid of packets which address is not the same of this cpu
		if (buf_ptr[PKT_TARGET_CPU] != ((NOC_COLUMN(hf_cpuid()) << 4) | NOC_LINE(hf_cpuid()))){
			kprintf("\nKERNEL: hardware error: this is not CPU X:%d Y:%d", (buf_ptr[PKT_TARGET_CPU] & 0xf0) >> 4, buf_ptr[PKT_TARGET_CPU] & 0xf);
			hf_queue_addtail(pktdrv_queue, buf_ptr);
			return;
		}

		//get the queue associated to the port indicated in the packet
		for (k = 0; k < MAX_TASKS; k++)
			if (pktdrv_ports[k] == buf_ptr[PKT_TARGET_PORT]) break;

		//check whether some task is running for that port
		if (k < MAX_TASKS && krnl_tcb[k].ptask){
		
			//check whether the task has room for more packets in that queue
			if (hf_queue_addtail(pktdrv_tqueue[k], buf_ptr)){
				kprintf("\nKERNEL: task (on port %d) queue full! dropping packet...", buf_ptr[PKT_TARGET_PORT]);
				hf_queue_addtail(pktdrv_queue, buf_ptr);
			}
			
		//no task is running for that port, return the buffer to the global queue
		//and drop the packet.
		}else{
			kprintf("\nKERNEL: no task on port %d (offender: cpu %d port %d) - dropping packet...", buf_ptr[PKT_TARGET_PORT], buf_ptr[PKT_SOURCE_CPU], buf_ptr[PKT_SOURCE_PORT]);
			hf_queue_addtail(pktdrv_queue, buf_ptr);
		}

	//no more space in the global queue, flush
	}else{
		kprintf("\nKERNEL: NoC queue full! dropping packet...");
		ni_flush(act_pkt_size);
	}

	return;
}

/**
 * @brief Returns the current cpu id number.
 *
 * @return the current cpu id, defined by the CPU_ID macro.
 */
uint16_t hf_cpuid(void)
{
	//return CPU_ID
	uint16_t id = *MAGIC_TILE_ID;
	return id;
}

/**
 * @brief Returns the number of processors in the system.
 *
 * @return the number of cores, defined by the dimensions of the NoC mesh.
 */
uint16_t hf_ncores(void)
{
	return NOC_WIDTH * NOC_HEIGHT;
}

/**
 * @brief Creates a communication queue for a task, using a port number as an alias.
 *
 * @param id is the task id which will own the communication queue
 * @param port is the receiving port for the task
 * @param packets is the communication queue size, in packets
 *
 * @return ERR_OK when successful, ERR_INVALID_ID if no task matches the specified id, ERR_COMM_UNFEASIBLE
 * if there is already a communication queue for the task, ERR_COMM_ERROR if there is already another task
 * using the specified port and ERR_OUT_OF_MEMORY if the systems runs out of memory.
 *
 * The queue created for the task will be used for the reception of data. Both ni_isr() and hf_recv()
 * routines will manage the queue, putting and pulling packets from the queue on demand. The communication
 * subsystem is configured by the association of a task id to a receiving port (alias) and the definition
 * of how many packet slots a task has on its queue.
 *
 * If the third parameter (packets) is set to 0, the maximum number of packets from the pool is available
 * to the task for the reception of data. This is the default, and should be used in most situations.
 */
int32_t hf_comm_create(uint16_t id, uint16_t port, uint16_t packets)
{
	int32_t k;

	if (id < MAX_TASKS){
		if (krnl_tcb[id].ptask == 0)
			return ERR_INVALID_ID;
		if (pktdrv_tqueue[id] != NULL)
			return ERR_COMM_UNFEASIBLE;
		for (k = 0; k < MAX_TASKS; k++)
			if (pktdrv_ports[k] == port) break;
		if (!k || k < MAX_TASKS)
			return ERR_COMM_ERROR;
	}else{
		return ERR_INVALID_ID;
	}

	if (packets > NOC_PACKET_SLOTS || packets == 0)
		packets = NOC_PACKET_SLOTS;

	pktdrv_tqueue[id] = hf_queue_create(packets);
	if (pktdrv_tqueue[id] == 0){
		return ERR_OUT_OF_MEMORY;
	}else{
		pktdrv_ports[id] = port;

		return ERR_OK;
	}
}

/**
 * @brief Destroys a communication queue, returning packets buffered on a task message queue to
 * the shared pool of packets.
 *
 * @param id is the task id which owns the communication queue
 *
 * @return ERR_OK when successful, ERR_INVALID_ID if no task matches the specified id, ERR_COMM_ERROR
 * if the queue could not be destroyed.
 */
int32_t hf_comm_destroy(uint16_t id)
{
	int32_t status;

	if (id < MAX_TASKS){
		if (krnl_tcb[id].ptask == 0)
			return ERR_INVALID_ID;
	}else{
		return ERR_INVALID_ID;
	}

	status = _di();
	while (hf_queue_count(pktdrv_tqueue[id]))
		hf_queue_addtail(pktdrv_queue, hf_queue_remhead(pktdrv_tqueue[id]));
	_ei(status);

	if (hf_queue_destroy(pktdrv_tqueue[id])){
		return ERR_COMM_ERROR;
	}else{
		pktdrv_ports[id] = 0;

		return ERR_OK;
	}

}

/**
 * @brief Probes for a message from a task.

 * @return channel of the first message that is waiting in queue (a value >= 0), ERR_COMM_EMPTY when no messages are
 * waiting in queue, ERR_COMM_UNFEASIBLE when no message queue (comm) was created.
 *
 * Asynchronous communication is possible using this primitive, as it first tests if there is data
 * ready for reception with hf_recv() which is a blocking primitive. The main advantage of using hf_recvprobe()
 * along with hf_recv() is that a selective receive can be performed in the right communication channel. As
 * the message is waiting at the beginning of the queue, a receive on its channel can be used to process the
 * messages in order, avoiding packet loss.
 */
int32_t hf_recvprobe(void)
{
	uint16_t id;
	int32_t k;
	uint16_t *buf_ptr;

	id = hf_selfid();
	if (pktdrv_tqueue[id] == NULL) return ERR_COMM_UNFEASIBLE;

	k = hf_queue_count(pktdrv_tqueue[id]);
	if (k){
		buf_ptr = hf_queue_get(pktdrv_tqueue[id], 0);
		if (buf_ptr)
			if (buf_ptr[PKT_CHANNEL] != 0xffff)
				return buf_ptr[PKT_CHANNEL];
	}

	return ERR_COMM_EMPTY;
}

/**
 * @brief Receives a message from a task (blocking receive).
 *
 * @param source_cpu is a pointer to a variable which will hold the source cpu
 * @param source_port is a pointer to a variable which will hold the source port
 * @param buf is a pointer to a buffer to hold the received message
 * @param size a pointer to a variable which will hold the size (in bytes) of the received message
 * @param channel is the selected message channel of this message (must be the same as in the sender)
 *
 * @return ERR_OK when successful, ERR_COMM_UNFEASIBLE when no message queue (comm) was
 * created and ERR_SEQ_ERROR when received packets arrive out of order, so the message
 * is corrupted.
 *
 * A message is build from packets received on the ni_isr() routine. Packets are decoded and
 * combined in a complete message, returning the message, its size and source identification
 * to the calling task. The buffer where the message will be stored must be large enough or
 * we will have a problem that may not be noticed before its too late.
 */
int32_t hf_recv(uint16_t *source_cpu, uint16_t *source_port, int8_t *buf, uint16_t *size, uint16_t channel)
{
	uint16_t id, seq = 0, packet = 0, packets, payload_bytes;
	uint32_t status;
	int32_t i, k, p = 0, error = ERR_OK;
	uint16_t *buf_ptr;

	//printf("hf_recv: 0x%x\n", (uint32_t)buf);

	id = hf_selfid();
	if (pktdrv_tqueue[id] == NULL) return ERR_COMM_UNFEASIBLE;

	while (1){
		k = hf_queue_count(pktdrv_tqueue[id]);
		if (k){
			buf_ptr = hf_queue_get(pktdrv_tqueue[id], 0);
			if (buf_ptr)
				if (buf_ptr[PKT_CHANNEL] == channel && buf_ptr[PKT_SEQ] == seq + 1) break;

			status = _di();
			buf_ptr = hf_queue_remhead(pktdrv_tqueue[id]);
			hf_queue_addtail(pktdrv_tqueue[id], buf_ptr);
			_ei(status);
		}
	}

	//printf("queue size: 0x%x\n", hf_queue_count(pktdrv_tqueue[id]);

	status = _di();
	buf_ptr = hf_queue_remhead(pktdrv_tqueue[id]);
	_ei(status);

	*source_cpu = buf_ptr[PKT_SOURCE_CPU];
	*source_port = buf_ptr[PKT_SOURCE_PORT];
	*size = buf_ptr[PKT_MSG_SIZE];
	seq = buf_ptr[PKT_SEQ];

	payload_bytes = (NOC_PACKET_SIZE - PKT_HEADER_SIZE) * sizeof(uint16_t);
	packets = (*size % payload_bytes == 0) ? (*size / payload_bytes) : (*size / payload_bytes + 1);

	while (++packet < packets){
		if (buf_ptr[PKT_SEQ] != seq++)
			error = ERR_SEQ_ERROR;

		for (i = PKT_HEADER_SIZE; i < NOC_PACKET_SIZE; i++){
			buf[p++] = (uint8_t)(buf_ptr[i] >> 8);
			buf[p++] = (uint8_t)(buf_ptr[i] & 0xff);
		}
		status = _di();
		hf_queue_addtail(pktdrv_queue, buf_ptr);
		_ei(status);

		i = 0;
		while (1){
			k = hf_queue_count(pktdrv_tqueue[id]);
			if (k){
				buf_ptr = hf_queue_get(pktdrv_tqueue[id], 0);
				if (buf_ptr)
					if (buf_ptr[PKT_CHANNEL] == channel && buf_ptr[PKT_SEQ] == seq) break;

				status = _di();
				buf_ptr = hf_queue_remhead(pktdrv_tqueue[id]);
				hf_queue_addtail(pktdrv_tqueue[id], buf_ptr);
				_ei(status);
				if (i++ > NOC_PACKET_SLOTS << 3) break;
			}
		}
		status = _di();
		buf_ptr = hf_queue_remhead(pktdrv_tqueue[id]);
		_ei(status);
	}

	if (buf_ptr[PKT_SEQ] != seq++)
		error = ERR_SEQ_ERROR;

	//TODO: treat endianess up to the size of the packet
	//for (i = PKT_HEADER_SIZE; i < NOC_PACKET_SIZE && p < *size; i++){
	for (i = PKT_HEADER_SIZE; i < buf_ptr[PKT_PAYLOAD] + 1 && p < *size; i++){
		buf[p++] = (uint8_t)(buf_ptr[i] >> 8);
		buf[p++] = (uint8_t)(buf_ptr[i] & 0xff);
	}
	
	status = _di();
	hf_queue_addtail(pktdrv_queue, buf_ptr);
	_ei(status);

	return error;
}

/**
 * @brief Sends a message to a task (blocking send).
 *
 * @param target_cpu is the target processor
 * @param target_port is the target task port
 * @param buf is a pointer to a buffer that holds the message
 * @param size is the size (in bytes) of the message
 * @param channel is the selected message channel of this message (must be the same as in the receiver)
 *
 * @return ERR_OK
 *
 * A message is broken into packets containing a header and part of the message as the payload.
 * The packets are injected, one by one, in the network through the network interface.
 */
int32_t hf_send(uint16_t target_cpu, uint16_t target_port, int8_t *buf, uint16_t size, uint16_t channel)
{
	uint16_t packet = 0, packets, payload_bytes, id;
	int32_t i, p = 0;
	uint16_t out_buf[NOC_PACKET_SIZE];

	id = hf_selfid();
	if (pktdrv_tqueue[id] == NULL) return ERR_COMM_UNFEASIBLE;

	payload_bytes = (NOC_PACKET_SIZE - PKT_HEADER_SIZE) * sizeof(uint16_t);
	packets = (size % payload_bytes == 0) ? (size / payload_bytes) : (size / payload_bytes + 1);

	while (++packet < packets){
		out_buf[PKT_TARGET_CPU] = (NOC_COLUMN(target_cpu) << 4) | NOC_LINE(target_cpu);
		out_buf[PKT_PAYLOAD] = NOC_PACKET_SIZE - 2;
		out_buf[PKT_SOURCE_CPU] = hf_cpuid();
		out_buf[PKT_SOURCE_PORT] = pktdrv_ports[id];
		out_buf[PKT_TARGET_PORT] = target_port;
		out_buf[PKT_MSG_SIZE] = size;
		out_buf[PKT_SEQ] = packet;
		out_buf[PKT_CHANNEL] = channel;

		for (i = PKT_HEADER_SIZE; i < NOC_PACKET_SIZE; i++, p+=2)
			out_buf[i] = ((uint8_t)buf[p] << 8) | (uint8_t)buf[p+1];

		ni_write_packet(out_buf, NOC_PACKET_SIZE);
	}

	out_buf[PKT_TARGET_CPU] = (NOC_COLUMN(target_cpu) << 4) | NOC_LINE(target_cpu);

	//Last packet, which most of the time is the only packet, must have size equals
	//to the number of flits in the payload plus two, instead of NOC_PACKET_SIZE. 
	//COMMENTED OUT >> out_buf[PKT_PAYLOAD] = NOC_PACKET_SIZE - 2; 
	out_buf[PKT_SOURCE_CPU] = hf_cpuid();
	out_buf[PKT_SOURCE_PORT] = pktdrv_ports[id];
	out_buf[PKT_TARGET_PORT] = target_port;
	out_buf[PKT_MSG_SIZE] = size;
	out_buf[PKT_SEQ] = packet;
	out_buf[PKT_CHANNEL] = channel;

	for (i = PKT_HEADER_SIZE; i < NOC_PACKET_SIZE && (p < size); i++, p+=2)
		out_buf[i] = ((uint8_t)buf[p] << 8) | (uint8_t)buf[p+1];
	
	//Actual size of payload fixed here (total data minus the first two flits).
	out_buf[PKT_PAYLOAD] = i - 2;
	
	//We can save some processing time by leaving garbage at the end of the packet.
	//Since we never read from these addresses, there is no reason to fill them up.
	//COMMENTED OUT >> for(; i < NOC_PACKET_SIZE; i++)
	//COMMENTED OUT >>     out_buf[i] = 0xdead;
	
	//Here, we configure the NI to send the actual size of data instead of 
	//a whole packet.
	//COMMENTED OUT >> ni_write_packet(out_buf, NOC_PACKET_SIZE);
	//uint32_t im = _di();
	ni_write_packet(out_buf, i);
	//_ei(im);
	
	//CPU becomes stalled during network operations, so there is no need to hold 
	//the process.
	//COMMENTED OUT >> delay_ms(1);
	
	return ERR_OK;
}

/**
 * @brief Receives a message from a task (blocking receive) with acknowledgement.
 *
 * @param source_cpu is a pointer to a variable which will hold the source cpu
 * @param source_port is a pointer to a variable which will hold the source port
 * @param buf is a pointer to a buffer to hold the received message
 * @param size a pointer to a variable which will hold the size (in bytes) of the received message
 * @param channel is the selected message channel of this message (must be the same as in the sender)
 *
 * @return ERR_OK when successful, ERR_COMM_UNFEASIBLE when no message queue (comm) was
 * created and ERR_SEQ_ERROR when received packets arrive out of order, so the message
 * is corrupted.
 *
 * A message is build from packets received on the ni_isr() routine. Packets are decoded and
 * combined in a complete message, returning the message, its size and source identification
 * to the calling task. The buffer where the message will be stored must be large enough or
 * we will have a problem that may not be noticed before its too late. After the reception
 * of the whole message is completed, an acknowledgement is sent to the sender task. This works
 * as a flow control mechanism, avoiding buffer/queue overflows common to the raw protocol.
 * Message channel 32767 will be used for the flow control mechanism. This routine must be used
 * exclusively with hf_sendack().
 */
int32_t hf_recvack(uint16_t *source_cpu, uint16_t *source_port, int8_t *buf, uint16_t *size, uint16_t channel)
{
	int32_t error;

	error = hf_recv(source_cpu, source_port, buf, size, channel);
	if (error == ERR_OK){
		hf_send(*source_cpu, *source_port, "ok", 3, 0x7fff);
	}

	return error;
}

/**
 * @brief Sends a message to a task (blocking send) with acknowledgement.
 *
 * @param target_cpu is the target processor
 * @param target_port is the target task port
 * @param buf is a pointer to a buffer that holds the message
 * @param size is the size (in bytes) of the message
 * @param channel is the selected message channel of this message (must be the same as in the receiver)
 * @param timeout is the time (in ms) that the sender will wait for a reception acknowledgement
 *
 * @return ERR_OK
 *
 * A message is broken into packets containing a header and part of the message as the payload.
 * The packets are injected, one by one, in the network through the network interface. After that, the
 * sender will wait for an acknowledgement from the receiver. This works as a flow control mechanism,
 * avoiding buffer/queue overflows common to the raw protocol. Message channel 32767 will be used for
 * the flow control mechanism. This routine should be used exclusively with hf_recvack().
 */
int32_t hf_sendack(uint16_t target_cpu, uint16_t target_port, int8_t *buf, uint16_t size, uint16_t channel, uint32_t timeout)
{
	uint16_t id, source_cpu, source_port;
	int32_t error, k;
	uint32_t time;
	int8_t ack[4];
	uint16_t *buf_ptr;

	error = hf_send(target_cpu, target_port, buf, size, channel);
	if (error == ERR_OK){
		id = hf_selfid();
		time = _read_us() / 1000;
		while (1){
			k = hf_queue_count(pktdrv_tqueue[id]);
			if (k){
				buf_ptr = hf_queue_get(pktdrv_tqueue[id], 0);
				if (buf_ptr)
					if (buf_ptr[PKT_CHANNEL] == 0x7fff && buf_ptr[PKT_MSG_SIZE] == 3) break;
			}
			if (((_read_us() / 1000) - time) > timeout) return ERR_COMM_TIMEOUT;
		}
		hf_recv(&source_cpu, &source_port, ack, &size, 0x7fff);
	}

	return error;
}
