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
        if (this->size() != other.size())
            return false;

        for (std::size_t i = 0; i < this->size(); i++)
            if ((*this)[i] != other[i])
                return false;

        return true;
    }

    bool String::operator!=(const String& other) const
    {
        return !((*this) == other);
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
