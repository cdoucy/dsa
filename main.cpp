#include <iostream>
#include "DynamicArray.hpp"
#include "Str.hpp"
#include <vector>
#include <string>
#include <DynamicArray.hpp>
#include <string.h>

int main()
{
    dsa::String s1("foo");
    dsa::String s2(s1);


    s2 += dsa::String("bar");

    dsa::String s3(std::move(s2));

    s3 += dsa::String("foo");

    s2 += dsa::String("hello");


    return 0;
}