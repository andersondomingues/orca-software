
#ifndef __SYSCALLS__
#define __SYSCALLS__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>

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

void *_sbrk(int incr);
int _close(int file) ;
int _fstat(int file, struct stat *st) ;
int _isatty(int file) ;
int _lseek(int file, int ptr, int dir);
void _exit(int status) ;
void _kill(int pid, int sig) ;
int _getpid(void) ;
int _write (int file, char * ptr, int len) ;
int _read (int file, char * ptr, int len) ;

// c++
void __dso_handle(void);



#ifdef __cplusplus
}
#endif

#endif //__STARTUP_H__