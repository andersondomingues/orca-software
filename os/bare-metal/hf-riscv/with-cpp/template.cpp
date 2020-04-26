// https://github.com/arobenko/embxx_on_rpi/blob/master/src/test_cpp/test_cpp_templates/main.cpp


#include <cstddef>
#include <iostream>

template <typename T>
void func(T startValue)
{
    std::cout << "func" << std::endl;
    for (volatile T i = startValue; i < startValue * 2; i += 1) {std::cout << i << std::endl;}
    for (volatile T i = startValue; i < startValue * 2; i += 2) {std::cout << i << std::endl;}
}

template <typename T, std::size_t TDummy>
struct SomeTemplateClass
{
    static void func(T startValue)
    {
        std::cout << "SomeTemplateClass:func" << std::endl;
        for (volatile T i = startValue; i < startValue * 2; i += 1) {std::cout << i << std::endl;}
        for (volatile T i = startValue; i < startValue * 2; i += 2) {std::cout << i << std::endl;}
    }
};

int main(int argc, const char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    int start1 = 10;
    unsigned start2 = 20;

    func(start1);
    func(start2);

    SomeTemplateClass<int, 5>::func(5);
    SomeTemplateClass<int, 10>::func(5);

    return 0;
}