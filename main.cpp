#include <iostream>
#include "DynamicArray.hpp"
#include <vector>

int main()
{
    dsa::DynamicArray<int> arr(4);
    std::vector<int> expected{1, 2, 3, 4};

    // [1, 2, 3, 4]
    for (int i = 0; i < arr.size(); i++)
        arr[i] = i + 1;

    std::cout << arr.cap() << std::endl;


    // [1, 2, 42, 3, 4]
    arr.insert(2, 42);
    expected.insert(expected.begin() + 2, 42);

    std::cout << arr.cap() << std::endl;

    // [100, 1, 2, 42, 3, 4]
    arr.insert(0, 100);
    expected.insert(expected.begin(), 100);

    std::cout << arr.cap() << std::endl;

    // [100, 1, 2, 42, 3, 4, 200]
    arr.insert(6, 200);
    expected.insert(expected.begin() + 6, 200);

    std::cout << arr.cap() << std::endl;

    // [100, 1, 2, 42, 3, 4, 300, 200]
    arr.insert(6, 200);
    expected.insert(expected.begin() + 6, 200);

    std::cout << arr.cap() << std::endl;


    return 0;
}