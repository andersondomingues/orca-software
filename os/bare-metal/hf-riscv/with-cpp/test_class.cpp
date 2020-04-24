#include "test_class.hpp"

TestClass::TestClass()
{
    i = 0xccddccdd;
}

TestClass::TestClass(int u)
{
    i = u;
}

TestClass::~TestClass()
{
    i = 0xdeadbeef;
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