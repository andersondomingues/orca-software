// https://github.com/arobenko/embxx_on_rpi/blob/master/src/test_cpp/test_cpp_abstract_class/main.cpp

#include <iostream>

class AbstractBase
{
public:
    virtual ~AbstractBase();
    virtual void func() = 0;
    virtual void nonOverridenFunc() final;
};


AbstractBase::~AbstractBase()
{
    std::cout << "dtor AbstractBase" << std::endl;
}

void AbstractBase::nonOverridenFunc()
{
    std::cout << "nonOverridenFunc AbstractBase" << std::endl;

}

class Derived : public AbstractBase
{
public:
    virtual ~Derived();
    virtual void func() override;
};

Derived::~Derived()
{
    std::cout << "dtor Derived" << std::endl;
}

void Derived::func()
{
    std::cout << "func Derived" << std::endl;
}


int main(int argc, const char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    Derived obj;
    AbstractBase* basePtr = &obj;
    basePtr->func();

    // Uncomment the following lines to generate destructor invocation code
    basePtr->~AbstractBase();
    delete basePtr;

    return 0;
}