#include <hal.h>
#include <libc.h>
#include <kprintf.h>
#include <kernel.h>
#include <panic.h>
#include <scheduler.h>

struct tcb_entry krnl_tcb[MAX_TASKS];
struct pcb_entry krnl_pcb;

struct tcb_entry *krnl_task;				/*!< pointer to a task control block entry */
uint16_t krnl_tasks;					/*!< number of tasks in the system */
uint16_t krnl_current_task;				/*!< the current running task id */
uint16_t krnl_schedule;					/*!< scheduler enable / disable flag */
struct queue *krnl_run_queue;				/*!< pointer to a queue of best effort tasks */
struct queue *krnl_delay_queue;				/*!< pointer to a queue of delayed tasks */
struct queue *krnl_rt_queue;				/*!< pointer to a queue of real time tasks */
struct queue *krnl_event_queue;				/*!< pointer to a queue of tasks waiting for an event */
uint8_t krnl_heap[HEAP_SIZE];				/*!< contiguous heap memory area to be used as a memory pool. the memory allocator (malloc() and free()) controls this data structure */
uint32_t krnl_free;					/*!< amount of free heap memory, in bytes */
