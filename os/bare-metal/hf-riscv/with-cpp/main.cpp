#include <stdio.h>
#include "test_class.h"

/*
TestClass global_class_with_static_item_0(5);
TestClass global_class_with_static_item_1(2);
TestClass global_class_with_static_item_2(6);
*/

//https://github.com/daedaleanai/libsupcxx/blob/master/tests/test-03-heap.cc
//https://github.com/daedaleanai/libsupcxx/blob/master/tests/test-01-initializers-operators.cc



class TestGlobalCtorDtor {
public:
  TestGlobalCtorDtor() {
    printf("Ctor called!\n");
  }
  ~TestGlobalCtorDtor() {
    printf("Dtor called!\n");
  }
};

//TestGlobalCtorDtor tmp;

int main (){

    printf("Hello, Kernel World!\nAnd Hello Again!\n");
    /*
    printf("%d %d %d\n",
        global_class_with_static_item_0.getI(),
        global_class_with_static_item_1.getI(),
        global_class_with_static_item_2.getI());
*/
}