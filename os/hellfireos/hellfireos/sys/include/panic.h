#define PANIC_ABORTED			0x00
#define PANIC_GPF			0x01
#define PANIC_STACK_OVERFLOW		0x02
#define PANIC_STACK_CORRUPT		0x0e
#define PANIC_NO_TASKS_LEFT		0x03
#define PANIC_OOM			0x04
#define PANIC_NO_TASKS_RUN		0x05
#define PANIC_NO_TASKS_RT		0x06
#define PANIC_NO_TASKS_DELAY		0x07
#define PANIC_UNKNOWN_TASK_STATE	0x08
#define PANIC_CANT_PLACE_RUN		0x09
#define PANIC_CANT_PLACE_DELAY		0x0a
#define PANIC_CANT_PLACE_RT		0x0b
#define PANIC_CANT_SWAP			0x0c
#define PANIC_NUTS_SEM			0x0d
#define PANIC_UNKNOWN			0xff

#ifdef CPP
extern "C" {
#endif

void panic(int32_t cause);

#ifdef CPP
}
#endif
