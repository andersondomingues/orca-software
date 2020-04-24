#include "test_class.h"
#include <stdio.h>

TestClass::TestClass()
{
    i = 0xccddccdd;
    printf("Ctor1 called!\n");
}

TestClass::TestClass(int u)
{
    i = u;
    printf("Ctor2 called!\n");
}

TestClass::~TestClass()
{
    i = 0xdeadbeef;
    printf("Dtor called!\n");
}

void TestClass::setI(int i)
{
    this->i = i;
}

int TestClass::getI(void)
{
    return this->i;
}

int TestClass::s_i = 3;