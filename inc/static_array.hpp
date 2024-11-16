#pragma once

#include <cstddef>
#include <stdexcept>

namespace dsa
{
    template <class T, std::size_t Size>
    class static_array
    {
    public:
        static_array() : static_array(T{}) {};

        ~static_array() = default;
        explicit static_array(const T &value)
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

        std::optional<std::size_t> search(const T& target)
        {
            // Time complexity = O(N), where N = Size
            for (std::size_t i = 0; i < Size; i++)
                if (this->_array[i] == target)
                    return std::make_optional(i);

            return std::nullopt;
        };

    private:
        T _array[Size];
    };
};
