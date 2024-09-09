#pragma once

#include <cstddef>
#include <stdexcept>

namespace dsa
{
    template <class T, std::size_t Size>
    class StaticArray
    {
    public:
        StaticArray() : StaticArray(T{}) {};

        ~StaticArray() = default;
        explicit StaticArray(const T &value)
        {
            // Time complexity = O(N), where N = Size
            for (std::size_t i = 0; i < Size; i++)
                this->_array[i] = value;
        }

        [[nodiscard]] std::size_t size() const
        {
            return Size;
        }

        T& operator[](std::size_t index)
        {
            if (index >= Size)
                throw std::runtime_error("out of bound");

            return this->_array[index];
        }

        const T& operator[](std::size_t index) const
        {
            if (index >= Size)
                throw std::runtime_error("out of bound");

            return this->_array[index];
        }

        int search(const T &target)
        {
            // Time complexity = O(N), where N = Size
            for (int i = 0; i < Size; i++)
                if (this->_array[i] == target)
                    return i;

            return -1;
        }

    private:
        T _array[Size];
    };
};
