#include <stdint.h>
#include <stddef.h>

// hf-riscv debug addr
#define DEBUG_ADDR	0xf00000d0
#define EXIT_TRAP   0xe0000000
volatile uint32_t* DEBUG = (volatile uint32_t*) DEBUG_ADDR;
volatile uint32_t* EXIT = (volatile uint32_t*) EXIT_TRAP;

// LIBC SYSCALLS
/////////////////////

extern uint32_t __heap_start;
extern uint32_t __heap_end;
extern uint32_t _sstack;
extern uint32_t _estack;
//extern uint32_t _stack;

#ifdef _DEBUG
// used only to debug syscalls
int8_t *itoa_syscal(int32_t i, int8_t *s, int32_t base);
void uart_write(int8_t *s);
#endif

void *_sbrk(int incr) {
  static unsigned char *heap   = (unsigned char *)&__heap_start;
  static unsigned char *eheap  = (unsigned char *)&__heap_end;
  static unsigned char *sstack = (unsigned char *)&_sstack;
  static unsigned char *estack = (unsigned char *)&_estack;
  //static unsigned char *stack  = (unsigned char *)&_stack;
  unsigned char *prev_heap;

  // get the current SP
  void* sp = NULL;
  //printf("%p", (void*)&p);
  //register void *sp asm ("sp");

#ifdef _DEBUG
  char numstr[20];
  char newline[] = "\n";
  char space[] = " - ";
  char alloc_str[] = "sbrk: ";
  char error_msg[] = "ERROR (sbrk): cannot alloc";
  char error_mem_size_msg[] = "ERROR (sbrk): possible heap and stack overlap";

  uart_write(alloc_str);
  itoa((uint32_t)heap,numstr,16);
  uart_write(numstr);
  uart_write(space);
  itoa((uint32_t)(heap+incr),numstr,16);
  uart_write(numstr);
  uart_write(space);
  itoa((uint32_t)(&sp),numstr,16);
  uart_write(numstr);
  uart_write(space);
  itoa((uint32_t)eheap,numstr,16);
  uart_write(numstr);
  uart_write(space);
  itoa((uint32_t)incr,numstr,10);
  uart_write(numstr);
  uart_write(newline);

  // if the stack or the heap are too big, then these variables will overlap
  if(eheap > sstack){
    uart_write(error_mem_size_msg);
    uart_write(newline);
    _exit();
  }

  // check whether the SP is invading the heap area
  if(eheap > &sp){
    uart_write(error_mem_size_msg);
    uart_write(newline);
    _exit();
  }

#endif




  if ((heap + incr) >=  eheap) {
#ifdef _DEBUG
    uart_write(error_msg);
    uart_write(newline);
    _exit();
#endif
    return (unsigned char *)-1;
  }

  prev_heap = heap;
  heap += incr;

  return (void *)prev_heap;
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
  * EXIT = 1;
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


//////////////////////////////////////
// c++ stubs to reduce memory usage
//////////////////////////////////////

void *operator new(size_t size) throw() { return malloc(size); }     
void operator delete(void *p) throw() { free(p); }    
extern "C" int __aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle){
  return 0;
} 


void __aeabi_unwind_cpp_pr0(void)
{
}

void __aeabi_unwind_cpp_pr1(void)
{    
}

void __aeabi_atexit(void)
{
}

void __dso_handle(void)
{    
}

//////////////////////////////////////
// if the heap is not used
//////////////////////////////////////
//extern "C" void *malloc(size_t) {return (void *)0;}
//extern "C" void free(void *) {  }

#ifdef _DEBUG
// used only to debug syscalls
int8_t *itoa_syscall(int32_t i, int8_t *s, int32_t base){
	int8_t *ptr = s, *ptr1 = s, tmp_char;
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
#endif


#ifdef _DEBUG
// rudimentary printf to debug system calls
void uart_write(int8_t *s){
  int8_t *ptr = s;
  int32_t i=0;
  char error_msg[] = "ERROR: cstr has no \\0\n";

  while (*ptr != '\0')
  {
    *DEBUG = *ptr;
    ptr++;
    i++;
    // check for strings without \0 
    if (i>100) {
      uart_write(error_msg);
    }
  }	
#endif


}

