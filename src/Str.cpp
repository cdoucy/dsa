//
// Created by cdoucy on 11/4/24.
//

#include "Str.hpp"

namespace dsa {
    static std::size_t strlen(const char *s)
    {
        std::size_t len = 0;

        for (; s[len] != '\0'; len++) {}

        return len;
    }

    static DynamicArray<char> arrayFromCString(const char *s)
    {
        const std::size_t len = strlen(s);
        DynamicArray<char> arr(len + 1);

        for (std::size_t i = 0; i < len; i++)
            arr[i] = s[i];

        arr[len] = '\0';

        return arr;
    }

    String::String(const char* cString)
        :   _str(arrayFromCString(cString)),
            _size(strlen(cString))
    {
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
        return this->_str[index];
    }

    String &String::operator+(const String& other)
    {
        this->_str[this->_size] = other[0];

        for (std::size_t i = 1; i < other.size(); i++)
        {
            this->_str.pushBack(other[i]);
        }

        this->_str.pushBack('\0');

        this->_size += other.size();
        return *this;
    }

    String &String::operator+=(const String& other)
    {
        return *this + other;
    }

    String &String::operator=(const String& other)
    {
        this->_str = other._str;
        this->_size = other._size;
        return *this;
    }
} // dsa