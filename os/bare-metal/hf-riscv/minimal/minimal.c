#include <stdint.h>

volatile int my_init_global = 1;
volatile int my_uninit_global;

// hf-riscv debug addr
#define DEBUG_ADDR	0xf00000d0
volatile uint32_t* DEBUG = (volatile uint32_t*) DEBUG_ADDR;

int main() {
  volatile int my_init_local = 2;
  volatile int my_uninit_local;
  volatile const int my_const = 3;
  volatile const char my_str[] = "I love embedded sw !!!!\n";

  // rudimentary printf
  char *loc_ptr=my_str;
	while (*loc_ptr != '\0') 
	{
		DEBUG = *loc_ptr++;
	}	  
}


