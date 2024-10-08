#pragma once

#include <cstddef>

#include "buf_size.hpp"

#define MAX(x, y) ( (x) > (y) ? (x) : (y) )

namespace dsa
{
    template <class T>
    class DynamicArray
    {
    public:
        DynamicArray()
            : _array(new T[DEFAULT_BUF_SIZE]),
              _size(0),
              _capacity(DEFAULT_BUF_SIZE)
        {
        };

        explicit DynamicArray(std::size_t size) : DynamicArray(size, T{})
        {
        };

        DynamicArray(const std::size_t size, const T& value)
            : _array(new T[MAX(size, DEFAULT_BUF_SIZE)]),
              _size(size),
              _capacity(MAX(size, DEFAULT_BUF_SIZE))
        {
            for (std::size_t i = 0; i < size; i++)
                this->_array[i] = value;
        };

        ~DynamicArray()
        {
            delete[] this->_array;
        };

        T& operator[](std::size_t index)
        {
            if (index >= this->_size)
                throw std::runtime_error("out of bound");
            return this->_array[index];
        }

        const T& operator[](std::size_t index) const
        {
            if (index >= this->_size)
                throw std::runtime_error("out of bound");

            return this->_array[index];
        }

        [[nodiscard]] std::size_t size() const
        {
            return this->_size;
        };

        [[nodiscard]] std::size_t cap() const
        {
            return this->_capacity;
        };

        int search(const T& target)
        {
            // Time complexity = O(N), where N = Size
            for (int i = 0; i < this->_size; i++)
                if (this->_array[i] == target)
                    return i;

            return -1;
        };

        void push_back(const T& value)
        {
            // T = O(1)
            // No reallocation needed.
            if (this->_size < this->_capacity)
            {
                this->_array[this->_size] = value;
                ++this->_size;
                return;
            }

            // We need to reallocate the array.
            // N = size
            // T = O(N)
            // S = O(N)

            auto newCap = this->_capacity * 2;
            T* newArray = new T[newCap];

            for (std::size_t i = 0; i < this->_size; i++)
            {
                newArray[i] = this->_array[i];
            }

            newArray[this->_size] = value;

            delete []this->_array;
            this->_array = newArray;

            ++this->_size;
            this->_capacity = newCap;
        };

        T& pop_back()
        {
            if (this->_size == 0)
                throw std::runtime_error("cannot pop empty array");

            --this->_size;
            return this->_array[this->_size];
        };

        void push_front(const T& value)
        {
            // T = O(N)
            if (this->_size < this->_capacity)
            {
                for (std::size_t i = this->_size; i > 0; --i)
                {
                    this->_array[i] = this->_array[i - 1];
                }

                this->_array[0] = value;
                ++this->_size;
                return;
            }

            // T = O(N)
            // S = O(N)
            auto newCap = this->_capacity * 2;
            T* newArray = new T[newCap];

            newArray[0] = value;
            ++this->_size;

            for (std::size_t i = 1; i < this->_size; i++)
                newArray[i] = this->_array[i - 1];

            delete []this->_array;
            this->_capacity = newCap;
            this->_array = newArray;
        }

        T pop_front()
        {
            if (this->_size == 0)
                throw std::runtime_error("cannot pop empty array");

            auto popped = std::move(this->_array[0]);

            // T = O(N)
            for (std::size_t i = 1; i < this->_size; i++)
                this->_array[i - 1] = this->_array[i];

            --this->_size;

            return popped;
        };

        void insert(std::size_t index, const T& value)
        {
            if (index > this->_size)
                throw std::runtime_error("insert index cannot be greater than array size");

            if (this->_size < this->_capacity)
            {
                // T = O(Size - Index)
                for (std::size_t i = this->_size; i > index; --i)
                    this->_array[i] = this->_array[i - 1];
                this->_array[index] = value;
                ++this->_size;
                return;
            }

            // N = Size + 1
            // T = O(N)
            // S = O(N)

            auto newCap = this->_capacity *= 2;
            auto newArr = new T[newCap];

            for (std::size_t i = 0; i < index; i++)
                newArr[i] = this->_array[i];

            newArr[index] = value;

            for (std::size_t i = index; i < this->_size; i++)
                newArr[i + 1] = this->_array[i];

            delete []this->_array;

            this->_capacity = newCap;
            this->_array = newArr;
            ++this->_size;
        };

        T remove(std::size_t index)
        {
            if (index >= this->_size)
                throw std::runtime_error("index out of bound");

            auto removed = std::move(this->_array[index]);

            // T = O(Size - Index)
            for (std::size_t i = index; i + 1 < this->_size; i++)
                this->_array[i] = this->_array[i + 1];

            --this->_size;

            return removed;
        };

    private:
        T* _array;
        std::size_t _size;
        std::size_t _capacity;
    };
};
