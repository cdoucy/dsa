#include <gtest/gtest.h>
#include <vector>
#include <fmt/printf.h>

// Override default buf size to ensure reallocation is covered by tests
#define DEFAULT_BUF_SIZE 4
#include "DynamicArray.hpp"

void assertEqualToVector(const dsa::DynamicArray<int> &arr, const std::vector<int> &expected, bool debug=false)
{
    EXPECT_EQ(arr.size(), expected.size());

    for (std::size_t i = 0; i < expected.size(); i++)
    {
        EXPECT_EQ(arr[i], expected[i]);

        if (debug)
            fmt::print("arr[{}] = {}, expected[{}] = {}\n", i, arr[i], i, expected[i]);
    }

}

TEST(DynamicArray, Indexing)
{
    dsa::DynamicArray<int> array(16);

    for (std::size_t i = 0; i < array.size(); i++)
        EXPECT_EQ(0, array[i]);

    array[6] = 42;
    EXPECT_EQ(42, array[6]);

    const dsa::DynamicArray<float> constArray(6, 100.0);
    for (std::size_t i = 0; i < constArray.size(); i++)
        EXPECT_EQ(100.0, constArray[i]);

    EXPECT_THROW(array[100], std::exception);
    EXPECT_THROW(constArray[100], std::exception);
    EXPECT_THROW(array[array.size()], std::exception);
    EXPECT_THROW(constArray[constArray.size()], std::exception);
}

TEST(DynamicArray, Search)
{
    dsa::DynamicArray<int> array(16, 0);

    array[7] = 1;

    EXPECT_EQ(false, array.search(42).has_value());
    EXPECT_EQ(true, array.search(1).has_value());
    EXPECT_EQ(7, array.search(1).value());
}

TEST(DynamicArray, PushBack)
{
    dsa::DynamicArray<int> arr;

    for (std::size_t i = 0; i < 128; i++) {
        arr.pushBack(42);
        EXPECT_EQ(42, arr[i]);
    }

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(128, arr.size());
}

TEST(DynamicArray, PopBack)
{
    dsa::DynamicArray<int> arr;

    EXPECT_THROW(arr.popBack(), std::runtime_error);

    for (int i = 0; i < 128; i++) {
        arr.pushBack(i);
    }

    for (int i = 127; i >= 0; i--)
        EXPECT_EQ(i, arr.popBack());

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(0, arr.size());
}

TEST(DynamicArray, PushFront)
{
    dsa::DynamicArray<int> arr;

    for (int i = 0; i < 128; i++) {
        arr.pushFront(i);
        EXPECT_EQ(i, arr[0]);
    }

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(128, arr.size());

    dsa::DynamicArray<int> arr2;

    arr2.pushBack(4);
    arr2.pushBack(5);
    arr2.pushBack(6);
    arr2.pushFront(3);
    arr2.pushFront(2);
    arr2.pushFront(1);

    EXPECT_EQ(6, arr2.size());

    for (int i = 0; i < 6; i++)
        // 1, 2, 3, 4, 5, 6
        EXPECT_EQ(i + 1, arr2[i]);

    dsa::DynamicArray<int> arr3(2, 42);

    arr3.pushFront(1);
    arr3.pushFront(2);
    ASSERT_EQ(2, arr3[0]);
    ASSERT_EQ(1, arr3[1]);
    ASSERT_EQ(42, arr3[2]);
    ASSERT_EQ(42, arr3[3]);
}

TEST(DynamicArray, PopFront)
{
    dsa::DynamicArray<int> arr;

    EXPECT_THROW(arr.popFront(), std::runtime_error);

    for (int i = 0; i < 128; i++) {
        arr.pushFront(i);
    }

    for (int i = 127; i >= 0; i--)
        EXPECT_EQ(i, arr.popFront());

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(0, arr.size());
}

TEST(DynamicArray, Reallocate)
{
    dsa::DynamicArray<int> arr(0);

    for (int i = 0; i < 128; i++)
    {
        arr.pushBack(1);
        EXPECT_EQ(1, arr[arr.size() - 1]);
    }

    EXPECT_EQ(128, arr.size());
    EXPECT_EQ(128, arr.cap());


    dsa::DynamicArray<int> arr2(0);

    for (int i = 0; i < 128; i++)
    {
        arr2.pushFront(1);
        EXPECT_EQ(1, arr2[arr2.size() - 1]);
    }

    EXPECT_EQ(128, arr2.size());
    EXPECT_EQ(128, arr2.cap());
}

TEST(DynamicArray, Insert)
{
    dsa::DynamicArray<int> arr(4);
    std::vector<int> expected{1, 2, 3, 4};

    // [1, 2, 3, 4]
    for (std::size_t i = 0; i < arr.size(); i++)
        arr[i] = static_cast<int>(i) + 1;


    // [1, 2, 42, 3, 4]
    arr.insert(2, 42);
    expected.insert(expected.begin() + 2, 42);
    assertEqualToVector(arr, expected);

    // [100, 1, 2, 42, 3, 4]
    arr.insert(0, 100);
    expected.insert(expected.begin(), 100);
    assertEqualToVector(arr, expected);

    // [100, 1, 2, 42, 3, 4, 200]
    arr.insert(6, 200);
    expected.insert(expected.begin() + 6, 200);
    assertEqualToVector(arr, expected);

    // [100, 1, 2, 42, 3, 4, 300, 200]
    arr.insert(6, 200);
    expected.insert(expected.begin() + 6, 200);
    assertEqualToVector(arr, expected);

    EXPECT_THROW(arr.insert(9, 444), std::runtime_error);
    EXPECT_THROW(arr.insert(100, 444), std::runtime_error);
}

TEST(DynamicArray, remove)
{
    dsa::DynamicArray<int> arr(4);
    std::vector<int> expected{1, 2, 3, 4};

    EXPECT_THROW(arr.remove(4), std::runtime_error);
    EXPECT_THROW(arr.remove(42), std::runtime_error);

    // [1, 2, 3, 4]
    for (std::size_t i = 0; i < arr.size(); i++)
        arr[i] = static_cast<int>(i) + 1;

    auto removed = arr.remove(2);
    expected.erase(expected.begin() + 2);
    EXPECT_EQ(removed, 3);
    assertEqualToVector(arr, expected);
    // [1, 2, 4]

    removed = arr.remove(0);
    expected.erase(expected.begin());
    EXPECT_EQ(removed, 1);
    assertEqualToVector(arr, expected);
    // [2, 4]

    removed = arr.remove(1);
    expected.erase(expected.begin() + 1);
    EXPECT_EQ(removed, 4);
    assertEqualToVector(arr, expected);
    // [2]

    removed = arr.remove(0);
    expected.erase(expected.begin());
    EXPECT_EQ(removed, 2);
    assertEqualToVector(arr, expected);
    // []

    EXPECT_THROW(arr.remove(0), std::runtime_error);
}