#ifdef CPP
extern "C" {
#endif

int32_t hf_id(int8_t *name);
int8_t *hf_name(uint16_t id);
uint16_t hf_selfid(void);
int8_t *hf_selfname(void);
int32_t hf_state(uint16_t id);
int32_t hf_jobs(uint16_t id);
int32_t hf_dlm(uint16_t id);
int32_t hf_priorityset(uint16_t id, uint8_t priority);
int32_t hf_priorityget(uint16_t id);
int32_t hf_spawn(void (*task)(), uint16_t period, uint16_t capacity, uint16_t deadline, int8_t *name, uint32_t stack_size);
void hf_yield(void);
int32_t hf_block(uint16_t id);
int32_t hf_resume(uint16_t id);
int32_t hf_kill(uint16_t id);
int32_t hf_delay(uint16_t id, uint32_t delay);

#ifdef CPP
}
#endif