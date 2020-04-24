#ifndef __TEST_CLASS_HPP__
#define __TEST_CLASS_HPP__

// source: https://github.com/PinusThunbergii/stm32/blob/master/src/test_class.cpp

class TestClass
{
public:
    TestClass();
    TestClass(int u);
    ~TestClass();
    void setI(int i);
    int getI(void);
private:
    int i;
    static int s_i;
protected:

};

#endif