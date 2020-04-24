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
extern uint32_t __data_start;
extern uint32_t __data_end;
extern uint32_t _sstack;
extern uint32_t _estack;
extern uint32_t _sbss;
extern uint32_t _ebss;

#ifdef _DEBUG
// used only to debug syscalls
int8_t *itoa_syscal(int32_t i, int8_t *s, int32_t base);
#endif

void *_sbrk(int incr) {
  static unsigned char *heap   = (unsigned char *)&__heap_start;
  static unsigned char *eheap  = (unsigned char *)&__heap_end;
  static unsigned char *sstack = (unsigned char *)&_sstack;
  static unsigned char *estack = (unsigned char *)&_estack;
  unsigned char *prev_heap;

  // get the current SP
  void* sp = NULL;

  // check whether the SP is invading the heap area
  if(eheap > &sp){
    _write (1, "ERROR (sbrk): possible heap and stack overlap\n", 46);
    _exit();
  }

  if ((heap + incr) >=  eheap) {
    _write (1, "ERROR (sbrk): cannot alloc\n", 27);
    _exit();
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
  char *loc_ptr=ptr;

	while (loc_ptr-ptr < len) 
	{
		*DEBUG = *loc_ptr;
		loc_ptr++;
	}	
	
	return len;  
}

int _read (int file, char * ptr, int len) {
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

//void *operator new(size_t size) throw() { return malloc(size); }     
//void operator delete(void *p) throw() { free(p); }    


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

///https://github.com/arobenko/embxx_on_rpi/blob/master/src/stdlib/stdlib_stub.cpp
/*
// invoked using delete operator.
void operator delete(void*)
{
    while (true) {}
}

// Compiler requires this function when there are pure virtual functions
extern "C" void __cxa_pure_virtual()
{
    while (true) {}
}

// Compiler requires this function when there are static objects that
// require custom destructors
extern "C" int __aeabi_atexit(
    void *object,
    void (*destructor)(void *),
    void *dso_handle)
{
    static_cast<void>(object);
    static_cast<void>(destructor);
    static_cast<void>(dso_handle);
    return 0;
}

// Compiler requires this symbol when there are static objects that
// require custom destructors
void* __dso_handle = nullptr;

*/


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

