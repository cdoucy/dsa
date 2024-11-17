#include "array.hpp"
#include "str.hpp"
#include "list.hpp"

int main()
{
    dsa::str s("yoo");
    dsa::array<int> arr;
    dsa::list<int> l;

    s += "wasup";
    arr.push_back(42);
    l.push_front(42);

    return 0;
}