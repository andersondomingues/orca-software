#include <vector>
#include <iostream>
#include <cstddef>


int main(int argc, const char** argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    int i=0;

    std::vector<int> v;
    static const int MaxVecSize = 5;
    for (i = 0; i < MaxVecSize; ++i) {
        std::cout << "alloc " << i << std::endl;
        v.push_back(i);
    }

    for (i = 0; i < MaxVecSize; ++i) {
        std::cout << v[i] << std::endl;
    }

    return 0;
}