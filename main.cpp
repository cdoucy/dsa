#include "array.hpp"
#include "str.hpp"

int main()
{
    const dsa::str s1("foo");
    dsa::str s2(s1);


    s2 += dsa::str("bar");

    dsa::str s3(std::move(s2));

    s3 += dsa::str("foo");

    s2 += dsa::str("hello");


    return 0;
}