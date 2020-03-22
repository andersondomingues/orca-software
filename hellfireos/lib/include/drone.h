#ifndef _DRONE_LIB_HF_H
#define _DRONE_LIB_HF_H

#include <hellfire.h>

typedef struct {

	int32_t a;
	int32_t b;
	int32_t c;
	int32_t d;
	
	int32_t e;
	int32_t f;
	int32_t g;
	int32_t h;
	
	int32_t i;
	int32_t j;
	int32_t k;
	int32_t l;
	
} drone_ekf_data_t;

typedef struct {

	int32_t a;
	int32_t b;
	int32_t c;
	int32_t d;

} drone_pid_data_t;


#endif