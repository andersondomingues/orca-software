#include "ann.h"                                                                                                                                                                               
#include "orca-hardware-counters.h"    
#include <math.h>

#define N_PATTERNS 16
#define INPUT_NEURONS 7
#define HIDDEN_NEURONS 8
#define OUTPUT_NEURONS 4
#define ETA	.30
#define ALPHA 0.90
#define EPSILON 0.005
#define INITIAL_WEIGHT_MAX 0.50
#define EPOCHS_MAX 100000
#define STATUS_CYCLES 100

struct neural_net_weights_s {
	float hidden_weights[INPUT_NEURONS+1][HIDDEN_NEURONS];
	float output_weights[HIDDEN_NEURONS+1][OUTPUT_NEURONS];
};

struct neural_net_s {
	char input[N_PATTERNS][INPUT_NEURONS];
	char target[N_PATTERNS][OUTPUT_NEURONS];
	float hidden[HIDDEN_NEURONS];
	float output[OUTPUT_NEURONS];
	struct neural_net_weights_s weights;
};

float sigmoid(float val) {
	return 1.0 / (1.0 + exp(-val));
}



void load(struct neural_net_s *net, char input[N_PATTERNS][INPUT_NEURONS], char target[N_PATTERNS][OUTPUT_NEURONS]) {
	int i, j;

	for (i = 0; i < N_PATTERNS; i++) {
		for (j = 0; j < INPUT_NEURONS; j++)
			net->input[i][j] = input[i][j];
		for (j = 0; j < OUTPUT_NEURONS; j++)
			net->target[i][j] = target[i][j];
	}
}

void infer(struct neural_net_s *net, int p) {
	int i, j;
	float acc;

	// compute hidden layer activations
	for (i = 0; i < HIDDEN_NEURONS; i++) {
		acc = net->weights.hidden_weights[INPUT_NEURONS][i];
		for (j = 0; j < INPUT_NEURONS; j++)
			acc += net->input[p][j] * net->weights.hidden_weights[j][i];
		net->hidden[i] = sigmoid(acc);
	}

	// compute output layer activations and calculate errors
	for (i = 0; i < OUTPUT_NEURONS; i++) {
		acc = net->weights.output_weights[HIDDEN_NEURONS][i];
		for (j = 0; j < HIDDEN_NEURONS; j++)
			acc += net->hidden[j] * net->weights.output_weights[j][i];
		net->output[i] = sigmoid(acc);
	}
}

void show_training(struct neural_net_s *net) {
	int i, p;
	char buf[30];

	for (p = 0; p < N_PATTERNS; p++) {
		printf("\ninput (%02d): ", p);
		for (i = 0; i < INPUT_NEURONS; i++)
			printf("%d", net->input[p][i]);
		printf(" target: ");
		for (i = 0; i < OUTPUT_NEURONS; i++)
			printf("%d", net->target[p][i]);
		infer(net, p);
		printf(" output: ");
		for (i = 0; i < OUTPUT_NEURONS; i++) {
			ftoa(net->output[i], buf, 4);
			printf("%s ", buf);
		}
	}
	printf("\n");
}

void train(struct neural_net_s *net) {
	int i, j, p, q, r;
	int rnd_index[N_PATTERNS];
	int epoch;
	float rnd, error, acc;
	float hidden_delta[HIDDEN_NEURONS];
	float output_delta[OUTPUT_NEURONS];
	float diff_hidden_weights[INPUT_NEURONS+1][HIDDEN_NEURONS];
	float diff_output_weights[HIDDEN_NEURONS+1][OUTPUT_NEURONS];
	char buf[30];

	for (p = 0; p < N_PATTERNS; p++)
		rnd_index[p] = p;

	// init hidden_weights and diff_hidden_weights
	for (i = 0; i < HIDDEN_NEURONS; i++) {
		for (j = 0; j <= INPUT_NEURONS; j++) {
			diff_hidden_weights[j][i] = 0.0;
			rnd = (random() % 100) / 100.0;
			net->weights.hidden_weights[j][i] = 2.0 * (rnd - 0.5) * INITIAL_WEIGHT_MAX;
		}
	}

	// init output_weights and diff_output_weights
	for (i = 0; i < OUTPUT_NEURONS; i++) {
		for (j = 0; j <= HIDDEN_NEURONS; j++) {
			diff_output_weights[j][i] = 0.0;
			rnd = (random() % 100) / 100.0;
			net->weights.output_weights[j][i] = 2.0 * (rnd - 0.5) * INITIAL_WEIGHT_MAX;
		}
	}

	printf("\ninitial outputs (not trained):");

	show_training(net);

	for (epoch = 1; epoch < EPOCHS_MAX; epoch++) {
		// randomize training pattern order
		for (p = 0; p < N_PATTERNS; p++) {
			q = random() % N_PATTERNS;
			r = rnd_index[p];
			rnd_index[p] = rnd_index[q];
			rnd_index[q] = r;
		}
		error = 0.0;

		// cycle through training patterns
		for (q = 0; q < N_PATTERNS; q++) {
			p = rnd_index[q];

			// feedforward: compute hidden layer activations
			for (i = 0; i < HIDDEN_NEURONS; i++) {
				acc = net->weights.hidden_weights[INPUT_NEURONS][i];
				for (j = 0; j < INPUT_NEURONS; j++)
					acc += net->input[p][j] * net->weights.hidden_weights[j][i];
				net->hidden[i] = sigmoid(acc);
			}

			// feedforward: compute output layer activation errors
			for (i = 0; i < OUTPUT_NEURONS; i++) {
				acc = net->weights.output_weights[HIDDEN_NEURONS][i];
				for (j = 0; j < HIDDEN_NEURONS; j++)
					acc += net->hidden[j] * net->weights.output_weights[j][i];
				net->output[i] = sigmoid(acc);
				output_delta[i] = (net->target[p][i] - net->output[i]) * net->output[i] * (1.0 - net->output[i]);
				error += 0.5 * (net->target[p][i] - net->output[i]) * (net->target[p][i] - net->output[i]);
			}

			// backpropagation of errors to hidden layer
			for (i = 0; i < HIDDEN_NEURONS; i++) {
				acc = 0.0;
				for (j = 0; j < OUTPUT_NEURONS; j++)
					acc += net->weights.output_weights[i][j] * output_delta[j];
				hidden_delta[i] = acc * net->hidden[i] * (1.0 - net->hidden[i]);
			}

			// update weights (input to hidden)
			for (i = 0; i < HIDDEN_NEURONS; i++) {
				diff_hidden_weights[INPUT_NEURONS][i] = ETA * hidden_delta[i] + ALPHA * diff_hidden_weights[INPUT_NEURONS][i];
				net->weights.hidden_weights[INPUT_NEURONS][i] += diff_hidden_weights[INPUT_NEURONS][i];
				for (j = 0; j < INPUT_NEURONS; j++) {
					diff_hidden_weights[j][i] = ETA * net->input[p][j] * hidden_delta[i] + ALPHA * diff_hidden_weights[j][i];
					net->weights.hidden_weights[j][i] += diff_hidden_weights[j][i] ;
				}
			}

			// update weights (hidden to output)
			for (i = 0; i < OUTPUT_NEURONS; i++) {
				diff_output_weights[HIDDEN_NEURONS][i] = ETA * output_delta[i] + ALPHA * diff_output_weights[HIDDEN_NEURONS][i];
				net->weights.output_weights[HIDDEN_NEURONS][i] += diff_output_weights[HIDDEN_NEURONS][i];
				for (j = 0; j < HIDDEN_NEURONS; j++) {
					diff_output_weights[j][i] = ETA * net->hidden[j] * output_delta[i] + ALPHA * diff_output_weights[j][i];
					net->weights.output_weights[j][i] += diff_output_weights[j][i];
				}
			}
		}

		if ((epoch % STATUS_CYCLES) == 0) {
			ftoa(error, buf, 6);
			printf("\nepoch: %d, error: %s", epoch, buf);
			show_training(net);
		}

		// if error rate is smaller than EPSILON, we are ok!
		if (error < EPSILON) break;
	}

	ftoa(error, buf, 6);

	printf("\n\ntraining set solved, %d epochs (error: %s)\n", epoch, buf);
}

void ann(void) {
	
	char input[N_PATTERNS][INPUT_NEURONS] = {
		{1, 1, 1, 1, 1, 1, 0},	// 0
		{0, 1, 1, 0, 0, 0, 0},	// 1
		{1, 1, 0, 1, 1, 0, 1},	// 2
		{1, 1, 1, 1, 0, 0, 1},	// 3
		{0, 1, 1, 0, 0, 1, 1},	// 4
		{1, 0, 1, 1, 0, 1, 1},	// 5
		{1, 0, 1, 1, 1, 1, 1},	// 6
		{1, 1, 1, 0, 0, 0, 0},	// 7
		{1, 1, 1, 1, 1, 1, 1},	// 8
		{1, 1, 1, 0, 0, 1, 1},	// 9
		{1, 1, 1, 0, 1, 1, 1},	// A
		{0, 0, 1, 1, 1, 1, 1},	// B
		{1, 0, 0, 1, 1, 1, 0},	// C
		{0, 1, 1, 1, 1, 0, 1},	// D
		{1, 0, 0, 1, 1, 1, 1},	// E
		{1, 0, 0, 0, 1, 1, 1}   // F
	};

	char target[N_PATTERNS][OUTPUT_NEURONS] = {
		{0, 0, 0, 0},
		{0, 0, 0, 1},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 1, 0, 0},
		{0, 1, 0, 1},
		{0, 1, 1, 0},
		{0, 1, 1, 1},
		{1, 0, 0, 0},
		{1, 0, 0, 1},
		{1, 0, 1, 0},
		{1, 0, 1, 1},
		{1, 1, 0, 0},
		{1, 1, 0, 1},
		{1, 1, 1, 0},
		{1, 1, 1, 1}
	};

	struct neural_net_s neural_network;

	load(&neural_network, input, target);
	train(&neural_network);
	show_training(&neural_network);

#ifdef MEMORY_ENABLE_COUNTERS
	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("---\n");
#endif                                                          
	printf("CPU: shift=%u, branches=%u\n",  *CPU_COUNTER_SHIFT, *CPU_COUNTER_BRANCHES);
	printf("CPU: arith=%u, logical=%u\n",   *CPU_COUNTER_ARITH, *CPU_COUNTER_LOGICAL);                                                                                                                                                                                                                                                                                                                                                                                                                         
    printf("CPU: jumps=%u, loadstore=%u\n", *CPU_COUNTER_JUMPS, *CPU_COUNTER_LOADSTORE);                                                                                                                 
	printf("CPU: cycles=%u, stalls=%u\n", *CPU_COUNTER_CYCLES_TOTAL, *CPU_COUNTER_CYCLES_STALL);                                                                                                         
	printf("CPU: hosttime=%u\n", *CPU_COUNTER_HOSTTIME);                                                                                                                                                 
	printf("---\n");                                                                                                                                                                                                
                                                                                                                                                                                                     
#ifdef HELLFIREOS
	hf_kill(hf_selfid());
#endif
}

#ifdef BARE_METAL

void main(){
	ann();
}

union float_long{
	float f;
	int32_t l;
	uint32_t u;
};

char *itoa(int32_t i, char *s, int32_t base){
	char *ptr = s, *ptr1 = s, tmp_char;
	int32_t tmp_value;

	if (base < 2 || base > 36) {
		*s = '\0';
		return s;
	}
	do {
		tmp_value = i;
		i /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - i * base)];
	} while (i);
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return s;
}

int32_t ftoa(float f, char *outbuf, int32_t precision){
	int32_t mantissa, int_part, frac_part, exp2, i;
	char *p;
	union float_long fl;

	p = outbuf;

	if (f < 0.0){
		*p = '-';
		f = -f;
		p++;
	}

	fl.f = f;

	exp2 = (fl.l >> 23) - 127;
	mantissa = (fl.l & 0xffffff) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 31){
		return -1;	/* too large */
	}else{
		if (exp2 < -23){
//			return -1;	/* too small */
		}else{
			if (exp2 >= 23){
				int_part = mantissa << (exp2 - 23);
			}else{
				if (exp2 >= 0){
					int_part = mantissa >> (23 - exp2);
					frac_part = (mantissa << (exp2 + 1)) & 0xffffff;
				}else{
					frac_part = (mantissa & 0xffffff) >> (-(exp2 + 1));
				}
			}
		}
	}

	if (int_part == 0){
		*p = '0';
		p++;
	}else{
		itoa(int_part, p, 10);
		while(*p) p++;
	}
	*p = '.';
	p++;

	for (i = 0; i < precision; i++){
		frac_part = (frac_part << 3) + (frac_part << 1);
		*p = (frac_part >> 24) + '0';
		p++;
		frac_part = frac_part & 0xffffff;
	}

	*p = 0;

	return 0;
}


#endif
