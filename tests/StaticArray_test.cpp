#include <gtest/gtest.h>
#include "StaticArray.hpp"

TEST(StaticArray, Indexing)
{
    dsa::StaticArray<int, 16> array;

    for (std::size_t i = 0; i < array.size(); i++)
        EXPECT_EQ(0, array[i]);

    array[6] = 42;
    EXPECT_EQ(42, array[6]);

    const dsa::StaticArray<float, 6> constArray(100.0);
    for (std::size_t i = 0; i < constArray.size(); i++)
        EXPECT_EQ(100.0, constArray[i]);

    EXPECT_THROW(array[100], std::exception);
    EXPECT_THROW(constArray[100], std::exception);
    EXPECT_THROW(array[array.size()], std::exception);
    EXPECT_THROW(constArray[constArray.size()], std::exception);
}

TEST(StaticArray, Search)
{
    dsa::StaticArray<int, 16> array(0);

    array[7] = 1;

    EXPECT_EQ(-1, array.search(42));
    EXPECT_EQ(7, array.search(1));
}