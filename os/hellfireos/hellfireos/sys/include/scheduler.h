#ifndef __SCHEDULER
#define __SCHEDULER

void dispatch_isr(void *arg);
int32_t sched_lottery(void);
int32_t sched_priorityrr(void);
int32_t sched_rma(void);
int32_t sched_edf(void);

#endif