#include <iostream>
#include "DynamicArray.hpp"
#include "Str.hpp"
#include <vector>

int main()
{
    dsa::String s("hello");

    std::cout << s.toCString() << std::endl;

    s += dsa::String(" ");
    s += dsa::String("world");

    std::cout << s.toCString() << std::endl;

    return 0;
}