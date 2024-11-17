#include <gtest/gtest.h>
#include <vector>
#include <fmt/printf.h>
#include "list.hpp"

TEST(list, basic)
{
    dsa::list<int> l;

    EXPECT_EQ(false, !!l.head());

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    int i = 0;
    for (auto it = l.head(); it; it = it.next())
    {
        i++;
        EXPECT_EQ(i, it.value());
    }

    for (auto it = l.tail(); it; it = it.prev())
    {
        EXPECT_EQ(i, it.value());
        i--;
    }

    EXPECT_EQ(3, l.pop_back());
    EXPECT_EQ(1, l.pop_front());
    EXPECT_EQ(2, l.pop_back());

    EXPECT_THROW(l.pop_back(), std::runtime_error);
    EXPECT_THROW(l.pop_front(), std::runtime_error);
}