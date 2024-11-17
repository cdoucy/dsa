#include "array.hpp"
#include "str.hpp"
#include "list.hpp"
#include <fmt/printf.h>
#include <iostream>

int main()
{
    dsa::list<dsa::str> l;
    l.push_back("hello");

    dsa::str s(std::move(l.pop_front()));

    dsa::list<int> l2;
    l2.push_back(1);
    auto x = l2.pop_front();
    std::cout << x << std::endl;
    return 0;
}
