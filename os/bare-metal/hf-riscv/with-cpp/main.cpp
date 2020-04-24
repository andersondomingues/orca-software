#include <stdio.h>
#include "test_class.h"


TestClass global_class_with_static_item_0(5);
TestClass global_class_with_static_item_1(2);
TestClass global_class_with_static_item_2(6);


//https://github.com/daedaleanai/libsupcxx/blob/master/tests/test-03-heap.cc
//https://github.com/daedaleanai/libsupcxx/blob/master/tests/test-01-initializers-operators.cc


/*
libs c++
https://github.com/eloraiby/bmcpp
https://github.com/michalsc/tiny-stl
https://github.com/ETLCPP/etl
https://github.com/jsren/osstdlib


https://github.com/X-Ryl669/eMQTT5
https://github.com/dnivanthaka/HTTPLib

https://github.com/gbmhunter/MFixedPoint
https://github.com/pronenewbits/Embedded_EKF_Library
https://github.com/pronenewbits/Embedded_UKF_Library

video
https://github.com/mpaland/vic

os wrapper
https://github.com/katono/cppelib

parallel
https://github.com/siemens/embb

*/

#ifdef __cplusplus
extern "C" {
#endif

class TestGlobalCtorDtor {
public:  
  ~TestGlobalCtorDtor() {
    printf("Dtor called!\n");
  }

  TestGlobalCtorDtor() {
    printf("Ctor called!\n");
  }
};

TestGlobalCtorDtor tmp;

extern unsigned	_text_size;
extern unsigned	_bss_size;
extern unsigned	_heap_size;
extern unsigned	_stack_size;
extern unsigned	_data_size;
extern unsigned	_init_array_size;
extern unsigned	_preinit_array_size;
extern unsigned	_fini_array_size;


int main (){
  unsigned	text_size = (unsigned)&_text_size;
  unsigned	bss_size = (unsigned)&_bss_size;
  unsigned	heap_size = (unsigned)&_heap_size;
  unsigned	stack_size = (unsigned)&_stack_size;
  unsigned	data_size = (unsigned)&_data_size;
  unsigned	init_array_size = (unsigned)&_init_array_size;
  unsigned	preinit_array_size = (unsigned)&_preinit_array_size;
  unsigned	fini_array_size = (unsigned)&_fini_array_size;

  // 'Ctor called' must be printed before the main

  printf("main is starting ...\n");
  printf("Hello, Kernel World!\nAnd Hello Again!\n");
  printf("Memory sizes (bytes):\n-text: %d\n-data: %d\n-bss: %d\n-heap: %d\n-stack: %d\n-preinit: %d\n-init: %d\n-fini: %d\n",
    text_size,data_size,bss_size,heap_size,stack_size,preinit_array_size,init_array_size,fini_array_size);
  
  printf("class 1: %d\n",global_class_with_static_item_0.getI());
  printf("class 2: %d\n",global_class_with_static_item_1.getI());
  printf("class 3: %d\n",global_class_with_static_item_2.getI());
  

  printf("main is finishing ...\n");
  
  // 'Dtor called' must be printed after the last print of main
  return 0;
}

#ifdef __cplusplus
}
#endif
