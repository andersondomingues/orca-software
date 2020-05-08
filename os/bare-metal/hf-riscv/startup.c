
/**
 * @file
 * 
 * @brief startup script with support to newlib nano and cpp
 * 
 */

/*
essa funcao c esse atributo nao usa stack. pode ser usada como startup code totalmente em c

static inline __attribute__((always_inline)) int staticFunc(int a, int b) 
{ 
  return a+b;
} 

int main()
{
    int a = staticFunc(2,4);
    return 0;
}

*/

#ifndef __STARTUP_H__
#define __STARTUP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define INLINE inline __attribute__((always_inline))

typedef void (*ptr_func_t)();

// main application
extern int main();

// location of these variables is defined in linker script
extern unsigned _data;
extern unsigned _edata;
extern unsigned _text;

extern unsigned _sbss;
extern unsigned _ebss;

extern unsigned __heap_start;
extern unsigned __heap_end;

extern unsigned _sstack;
extern unsigned _estack;

// used only when c++ is required
#ifdef USE_CPP
extern ptr_func_t __preinit_array_start[];
extern ptr_func_t __preinit_array_end[];

extern ptr_func_t __init_array_start[];
extern ptr_func_t __init_array_end[];

extern ptr_func_t __fini_array_start[];
extern ptr_func_t __fini_array_end[];
#endif

/** Copy default data to DATA section
 */
/*
static INLINE void copy_data() {
    unsigned *src = &_text;
    unsigned *dst = &_data;
    while (dst < &_edata) {
        *dst++ = *src++;
    }
}
*/

/** Erase BSS section
 */
static INLINE void zero_bss() {
    unsigned *dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
}

/** Fill heap memory
 */
/*
static INLINE void fill_heap(unsigned fill) {
    unsigned *dst = &__heap_start;
    while (dst < &__heap_end) {
        *dst++ = fill;
    }
}
*/

#ifdef USE_CPP

static INLINE void call_init_array() {
    ptr_func_t *array = __preinit_array_start;

    while (array < __preinit_array_end) {
        (*array)();
        array++;
    }

    array = __init_array_start;
    while (array < __init_array_end) {
        (*array)();
        //printf("init: %p\n",array);
        array++;
    }
}

/** Call destructors for static objects
 */
static INLINE void call_fini_array() {
    ptr_func_t *array = __fini_array_start;
    while (array < __fini_array_end) {
        (*array)();
        array++;
    }
}
#endif

// reset handler
void RESET_handler() {
    //copy_data();
    zero_bss();
    //fill_heap(0x45455246);
#ifdef USE_CPP
    call_init_array();
#endif
    // run application
    main();
    // call destructors for static instances
#ifdef USE_CPP
    call_fini_array();
#endif    
}

#ifdef __cplusplus
}
#endif

#endif //__STARTUP_H__