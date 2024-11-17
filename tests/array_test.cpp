#include <gtest/gtest.h>
#include <vector>
#include <fmt/printf.h>

// Override default buf size to ensure reallocation is covered by tests
#define DEFAULT_BUF_SIZE 4
#include "array.hpp"

void assert_equal_to_vector(const dsa::array<int> &arr, const std::vector<int> &expected, bool debug=false)
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
    dsa::array<int> array(16);

    for (std::size_t i = 0; i < array.size(); i++)
        EXPECT_EQ(0, array[i]);

    array[6] = 42;
    EXPECT_EQ(42, array[6]);

    const dsa::array<float> constArray(6, 100.0);
    for (std::size_t i = 0; i < constArray.size(); i++)
        EXPECT_EQ(100.0, constArray[i]);

    EXPECT_THROW(array[100], std::exception);
    EXPECT_THROW(constArray[100], std::exception);
    EXPECT_THROW(array[array.size()], std::exception);
    EXPECT_THROW(constArray[constArray.size()], std::exception);
}

TEST(DynamicArray, Search)
{
    dsa::array<int> array(16, 0);

    array[7] = 1;

    EXPECT_EQ(false, array.search(42).has_value());
    EXPECT_EQ(true, array.search(1).has_value());
    EXPECT_EQ(7, array.search(1).value());
}

TEST(DynamicArray, PushBack)
{
    dsa::array<int> arr;

    for (std::size_t i = 0; i < 128; i++) {
        arr.push_back(42);
        EXPECT_EQ(42, arr[i]);
    }

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(128, arr.size());
}

TEST(DynamicArray, PopBack)
{
    dsa::array<int> arr;

    EXPECT_THROW(arr.pop_back(), std::runtime_error);

    for (int i = 0; i < 128; i++) {
        arr.push_back(i);
    }

    for (int i = 127; i >= 0; i--)
        EXPECT_EQ(i, arr.pop_back());

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(0, arr.size());
}

TEST(DynamicArray, PushFront)
{
    dsa::array<int> arr;

    for (int i = 0; i < 128; i++) {
        arr.push_front(i);
        EXPECT_EQ(i, arr[0]);
    }

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(128, arr.size());

    dsa::array<int> arr2;

    arr2.push_back(4);
    arr2.push_back(5);
    arr2.push_back(6);
    arr2.push_front(3);
    arr2.push_front(2);
    arr2.push_front(1);

    EXPECT_EQ(6, arr2.size());

    for (int i = 0; i < 6; i++)
        // 1, 2, 3, 4, 5, 6
        EXPECT_EQ(i + 1, arr2[i]);

    dsa::array<int> arr3(2, 42);

    arr3.push_front(1);
    arr3.push_front(2);
    ASSERT_EQ(2, arr3[0]);
    ASSERT_EQ(1, arr3[1]);
    ASSERT_EQ(42, arr3[2]);
    ASSERT_EQ(42, arr3[3]);
}

TEST(DynamicArray, PopFront)
{
    dsa::array<int> arr;

    EXPECT_THROW(arr.pop_front(), std::runtime_error);

    for (int i = 0; i < 128; i++) {
        arr.push_front(i);
    }

    for (int i = 127; i >= 0; i--)
        EXPECT_EQ(i, arr.pop_front());

    EXPECT_EQ(128, arr.cap());
    EXPECT_EQ(0, arr.size());
}

TEST(DynamicArray, Reallocate)
{
    dsa::array<int> arr(0);

    for (int i = 0; i < 128; i++)
    {
        arr.push_back(1);
        EXPECT_EQ(1, arr[arr.size() - 1]);
    }

    EXPECT_EQ(128, arr.size());
    EXPECT_EQ(128, arr.cap());


    dsa::array<int> arr2(0);

    for (int i = 0; i < 128; i++)
    {
        arr2.push_front(1);
        EXPECT_EQ(1, arr2[arr2.size() - 1]);
    }

    EXPECT_EQ(128, arr2.size());
    EXPECT_EQ(128, arr2.cap());
}

TEST(DynamicArray, Insert)
{
    dsa::array<int> arr(4);
    std::vector<int> expected{1, 2, 3, 4};

    // [1, 2, 3, 4]
    for (std::size_t i = 0; i < arr.size(); i++)
        arr[i] = static_cast<int>(i) + 1;


    // [1, 2, 42, 3, 4]
    arr.insert(2, 42);
    expected.insert(expected.begin() + 2, 42);
    assert_equal_to_vector(arr, expected);

    // [100, 1, 2, 42, 3, 4]
    arr.insert(0, 100);
    expected.insert(expected.begin(), 100);
    assert_equal_to_vector(arr, expected);

    // [100, 1, 2, 42, 3, 4, 200]
    arr.insert(6, 200);
    expected.insert(expected.begin() + 6, 200);
    assert_equal_to_vector(arr, expected);

    // [100, 1, 2, 42, 3, 4, 300, 200]
    arr.insert(6, 200);
    expected.insert(expected.begin() + 6, 200);
    assert_equal_to_vector(arr, expected);

    EXPECT_THROW(arr.insert(9, 444), std::runtime_error);
    EXPECT_THROW(arr.insert(100, 444), std::runtime_error);
}

TEST(DynamicArray, remove)
{
    dsa::array<int> arr(4);
    std::vector<int> expected{1, 2, 3, 4};

    EXPECT_THROW(arr.remove(4), std::runtime_error);
    EXPECT_THROW(arr.remove(42), std::runtime_error);

    // [1, 2, 3, 4]
    for (std::size_t i = 0; i < arr.size(); i++)
        arr[i] = static_cast<int>(i) + 1;

    auto removed = arr.remove(2);
    expected.erase(expected.begin() + 2);
    EXPECT_EQ(removed, 3);
    assert_equal_to_vector(arr, expected);
    // [1, 2, 4]

    removed = arr.remove(0);
    expected.erase(expected.begin());
    EXPECT_EQ(removed, 1);
    assert_equal_to_vector(arr, expected);
    // [2, 4]

    removed = arr.remove(1);
    expected.erase(expected.begin() + 1);
    EXPECT_EQ(removed, 4);
    assert_equal_to_vector(arr, expected);
    // [2]

    removed = arr.remove(0);
    expected.erase(expected.begin());
    EXPECT_EQ(removed, 2);
    assert_equal_to_vector(arr, expected);
    // []

    EXPECT_THROW(arr.remove(0), std::runtime_error);
}

TEST(array, reset)
{
    dsa::array<int> arr(4);

    arr.reset();

    EXPECT_EQ(0, arr.cap());
    EXPECT_EQ(0, arr.size());

    arr.push_back(1);

    EXPECT_EQ(1, arr.size());
    EXPECT_EQ(DEFAULT_BUF_SIZE, arr.cap());
    EXPECT_EQ(1, arr[0]);

    arr.reset();

    EXPECT_EQ(0, arr.cap());
    EXPECT_EQ(0, arr.size());

    arr.push_front(1);

    EXPECT_EQ(1, arr.size());
    EXPECT_EQ(DEFAULT_BUF_SIZE, arr.cap());
    EXPECT_EQ(1, arr[0]);

    arr.reset();

    EXPECT_EQ(0, arr.cap());
    EXPECT_EQ(0, arr.size());

    arr.insert(0, 1);

    EXPECT_EQ(1, arr.size());
    EXPECT_EQ(DEFAULT_BUF_SIZE, arr.cap());
    EXPECT_EQ(1, arr[0]);
}