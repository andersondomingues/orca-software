#include <stdint.h>
#include <stddef.h>

/*
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
*/
//#include <samd21g18a.h>
//#include <port.h>
//#include <usart_serial.h>

//static struct usart_module stdio_uart_module;

// hf-riscv debug addr
#define DEBUG_ADDR	0xf00000d0
volatile uint32_t* DEBUG = (volatile uint32_t*) DEBUG_ADDR;

// LIBC SYSCALLS
/////////////////////

extern int _end;

void *_sbrk(int incr) {
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL) {
    heap = (unsigned char *)&_end;
  }
  prev_heap = heap;

  heap += incr;

  return prev_heap;
}

int _close(int file) {
  return -1;
}

#include <sys/stat.h>
int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;

  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

void _exit(int status) {
	/* Nothing to do */
	for (;;);
}

void _kill(int pid, int sig) {
  return;
}

int _getpid(void) {
  return -1;
}

int _write (int file, char * ptr, int len) {
  /*
  int written = 0;

  if ((file != 1) && (file != 2) && (file != 3)) {
    return -1;
  }

  for (; len != 0; --len) {
    if (usart_serial_putchar(&stdio_uart_module, (uint8_t)*ptr++)) {
      return -1;
    }
    ++written;
  }
  return written;
  */
  char *loc_ptr=ptr;

	while (loc_ptr-ptr < len) 
	{
		*DEBUG = *loc_ptr;
		//*((uint32_t *) 0xe1034000) = *loc_ptr;
		loc_ptr++;
	}	
	
	return len;  
}

int _read (int file, char * ptr, int len) {
  /*
  int read = 0;

  if (file != 0) {
    return -1;
  }

  for (; len > 0; --len) {
    usart_serial_getchar(&stdio_uart_module, (uint8_t *)ptr++);
    read++;
  }
  return read;
  */
  char *loc_ptr=ptr;

	while (loc_ptr-ptr < len) 
	{
		*loc_ptr = *DEBUG;
		loc_ptr++;		
	}	
	
	return len;  
}



