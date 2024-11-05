//
// Created by cdoucy on 11/4/24.
//

#include "Str.hpp"

namespace dsa
{
    static std::size_t strlen(const char* s)
    {
        std::size_t len = 0;

        for (; s[len] != '\0'; len++)
        {
        }

        return len;
    }

    static DynamicArray<char> arrayFromCString(const char* s)
    {
        const std::size_t len = strlen(s);
        DynamicArray<char> arr(len + 1);

        for (std::size_t i = 0; i < len; i++)
            arr[i] = s[i];

        arr[len] = '\0';

        return arr;
    }

    String::String()
        :   _str(1, '\0'),
            _size(0)
    {
    }

    String::String(const char* cString)
        : _str(arrayFromCString(cString)),
          _size(strlen(cString))
    {
    }

    String::String(const String& other)
        : _str(other._str),
          _size(other._size)
    {
    }

    String::String(String&& other) noexcept
    :   _str(std::move(other._str)),
        _size(other._size)
    {
        other.reset();
    }

    String::String(DynamicArray<char>&& arr)
        : _str(std::move(arr)),
            _size(arr.size())
    {
        this->_str.pushBack('\0');
    }

    std::size_t String::size() const
    {
        return this->_size;
    }

    const char* String::toCString() const
    {
        return this->_str.data();
    }

    char String::operator[](const std::size_t index) const
    {
        if (index >= this->_size)
            throw std::runtime_error("string index out of range");
        return this->_str[index];
    }


    String String::operator+(const String& other) const
    {
        String res(*this);
        res.concat(other);
        return res;
    }

    String& String::operator+=(const String& other)
    {
        this->concat(other);
        return *this;
    }

    String& String::operator=(String&& other) noexcept
    {
        this->_str = std::move(other._str);
        this->_size = other._size;

        return *this;
    }

    bool String::operator==(const String& other) const
    {
        return this->cmp(other) == 0;
    }

    bool String::operator!=(const String& other) const
    {
        return this->cmp(other) != 0;
    }

    bool String::operator<(const String& other) const
    {
        return this->cmp(other) < 0;
    }

    bool String::operator>(const String& other) const
    {
        return this->cmp(other) > 0;
    }

    bool String::operator<=(const String& other) const
    {
        return this->cmp(other) <= 0;
    }

    bool String::operator>=(const String& other) const
    {
        return this->cmp(other) >= 0;
    }

    String String::substring(std::size_t start, std::size_t end) const
    {
        if (start == end)
            throw std::runtime_error("start and end must be different");

        if (start > this->_size)
            throw std::runtime_error("start out of bound");

        if (end > this->_size + 1)
            throw std::runtime_error("end out of bound");

        return String(this->_str.slice(start, end));
    }

    int String::cmp(const String &other) const
    {
        std::size_t i = 0;
        std::size_t j = 0;

        while (i < this->_size && j < other._size)
        {
            const char ci = (*this)[i];
            const char cj = other[j];

            if (ci != cj)
                return ci - cj;
            i++;
            j++;
        }

        if (i == this->_size && j == other._size)
            return 0;

        return static_cast<int>(this->_size) - static_cast<int>(other._size);
    }



    bool String::isEmpty() const
    {
        return this->_size == 0;
    }

    void String::reset()
    {
        this->_str = DynamicArray<char>(1, '\0');
        this->_size = 0;
    }

    void String::concat(const String& s)
    {
        if (this->_str.size() != 0)
            // Remove trailing '\0'
            this->_str.popBack();

        for (std::size_t i = 0; i < s.size(); i++)
        {
            this->_str.pushBack(s[i]);
        }

        this->_str.pushBack('\0');

        this->_size += s.size();
    }
} // dsa
