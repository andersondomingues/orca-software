#include <stdint.h>
#include <stdio.h>

volatile int my_init_global = 1;
volatile int my_uninit_global;

// hf-riscv debug addr
extern uint32_t* DEBUG;

int main() {
  volatile int my_init_local = 2;
  volatile int my_uninit_local;
  volatile const int my_const = 3;
  volatile const char my_str[] = "I love embedded sw !!!!\n";

  printf("Hello, World!\n");
  // rudimentary printf
  char *loc_ptr=my_str;
	while (*loc_ptr != '\0') 
	{
		DEBUG = *loc_ptr++;
	}	  
}


