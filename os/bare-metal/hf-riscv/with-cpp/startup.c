// source : https://raw.githubusercontent.com/cortexm/baremetal/master/src/startup/startup.cpp
// very simple startup code with definition of handlers for all cortex-m cores

#ifndef __STARTUP_H__
#define __STARTUP_H__

#ifdef __cplusplus
extern "C" {
#endif

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
#ifdef CONFIG_CPLUSPLUS
extern ptr_func_t __preinit_array_start[];
extern ptr_func_t __preinit_array_end[];

extern ptr_func_t __init_array_start[];
extern ptr_func_t __init_array_end[];

extern ptr_func_t __fini_array_start[];
extern ptr_func_t __fini_array_end[];
#endif

/** Copy default data to DATA section
 */
void copy_data() {
    unsigned *src = &_text;
    unsigned *dst = &_data;
    while (dst < &_edata) {
        *dst++ = *src++;
    }
}

/** Erase BSS section
 */
void zero_bss() {
    unsigned *dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
}

/** Fill heap memory
 */
void fill_heap(unsigned fill) {
    unsigned *dst = &__heap_start;
    while (dst < &__heap_end) {
        *dst++ = fill;
    }
}

#ifdef CONFIG_CPLUSPLUS

/** Call constructors for static objects
 */
void call_init_array() {
    auto array = __preinit_array_start;
    while (array < __preinit_array_end) {
        (*array)();
        array++;
    }

    array = __init_array_start;
    while (array < __init_array_end) {
        (*array)();
        array++;
    }
    printf("fim call_init_array\n");
}

/** Call destructors for static objects
 */
void call_fini_array() {
    auto array = __fini_array_start;
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
#ifdef CONFIG_CPLUSPLUS
    call_init_array();
#endif
    // run application
    main();
    // call destructors for static instances
#ifdef CONFIG_CPLUSPLUS
    call_fini_array();
#endif    
    // stop
    //while (true);
}

#ifdef __cplusplus
}
#endif

#endif //__STARTUP_H__