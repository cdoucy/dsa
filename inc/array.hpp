#pragma once

#include <cstddef>
#include <optional>
#include <stdexcept>

#include "buf_size.hpp"

#define MAX(x, y) ( (x) > (y) ? (x) : (y) )

namespace dsa
{
    template <class T>
    class array
    {
    public:
        array()
            : _array(new T[DEFAULT_BUF_SIZE]),
              _size(0),
              _capacity(DEFAULT_BUF_SIZE)
        {
        };

        explicit array(std::size_t size) : array(size, T{})
        {
        };

        array(const std::size_t size, const T& value)
            : _array(new T[MAX(size, DEFAULT_BUF_SIZE)]),
              _size(size),
              _capacity(MAX(size, DEFAULT_BUF_SIZE))
        {
            for (std::size_t i = 0; i < size; i++)
                this->_array[i] = value;
        };

        array(const array<T> &other)
            :   array(other._size)
        {
            for (std::size_t i = 0; i < other._size; i++)
                this->_array[i] = other._array[i];
        }

        array(array<T> &&other)
            :   _array(other._array),
                _size(other._size),
                _capacity(other._capacity)
        {
            other.reset();
        }

        ~array()
        {
            delete[] this->_array;
        };

        void reset()
        {
            this->_array = new T[DEFAULT_BUF_SIZE];
            this->_size = 0;
            this->_capacity = DEFAULT_BUF_SIZE;
        }

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

        std::optional<std::size_t> search(const T& target)
        {
            // Time complexity = O(N), where N = Size
            for (std::size_t i = 0; i < this->_size; i++)
                if (this->_array[i] == target)
                    return std::make_optional(i);

            return std::nullopt;
        };

        void push_back(T&& value)
        {
            this->push_back_impl(std::move(value));
        };

        void push_back(const T& value)
        {
            this->push_back_impl(value);
        };

        T& pop_back()
        {
            if (this->_size == 0)
                throw std::runtime_error("cannot pop empty array");

            --this->_size;
            return this->_array[this->_size];
        };

        void push_front(T&& value)
        {
            this->push_front_impl(std::move(value));
        }

        void push_front(const T& value)
        {
            this->push_front_impl(value);
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

        const T *data() const
        {
            return this->_array;
        }

        array<T> &operator=(array &&other) noexcept
        {
            delete []this->_array;

            this->_size = other._size;
            this->_array = other._array;
            this->_capacity = other._capacity;

            other.reset();
            return *this;
        }

        array<T> &operator=(const array &other) noexcept
        {
            if (this == &other)
                return *this;

            this->_size = other._size;
            this->_capacity = other._capacity;

            delete []this->_array;
            this->_array = new T[this->_capacity];

            for (std::size_t i = 0; i < this->_size; i++)
                this->_array[i] = other._array[i];

            return *this;
        }

        array<T> slice(std::size_t start, std::size_t end) const
        {
            if (start == end)
                throw std::runtime_error("start and end must be different");

            if (start > this->_size)
                throw std::runtime_error("start out of bound");

            if (end > this->_size + 1)
                throw std::runtime_error("end out of bound");

            std::size_t size = end - start;

            array<T> subarr(size);

            for (std::size_t i = 0; i < size; i++)
                subarr[i] = this->_array[i + start];

            return subarr;
        }

    private:
        T* _array;
        std::size_t _size;
        std::size_t _capacity;

        template <typename U>
        void push_front_impl(U&& value)
        {
            // T = O(N)
            if (this->_size < this->_capacity)
            {
                for (std::size_t i = this->_size; i > 0; --i)
                {
                    this->_array[i] = std::move(this->_array[i - 1]);
                }

                this->_array[0] = std::forward<U>(value);
                ++this->_size;
                return;
            }

            // T = O(N)
            // S = O(N)
            auto newCap = this->_capacity * 2;
            T* newArray = new T[newCap];

            newArray[0] = std::forward<U>(value);;
            ++this->_size;

            for (std::size_t i = 1; i < this->_size; i++)
                newArray[i] = std::move(this->_array[i - 1]);

            delete []this->_array;
            this->_capacity = newCap;
            this->_array = newArray;
        }

        template <typename U>
        void push_back_impl(U&& value)
        {
            // T = O(1)
            // No reallocation needed.
            if (this->_size < this->_capacity)
            {
                this->_array[this->_size] = std::forward<U>(value);
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
                newArray[i] = std::move(this->_array[i]);
            }

            newArray[this->_size] = std::forward<U>(value);

            delete []this->_array;
            this->_array = newArray;

            ++this->_size;
            this->_capacity = newCap;
        }
    };
}
