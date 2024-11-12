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
            _size(this->_str.size())
    {
        this->_str.pushBack('\0');
    }

    String::String(std::size_t capacity)
        :   _str(capacity, '\0'),
            _size(0)

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
        return this->at(index);
    }

    char String::at(const std::size_t index) const
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

    String& String::operator=(const String& other)
    {
        if (this == &other)
            return *this;

        this->_str = other._str;
        this->_size = other>_size;

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

        if (end < start)
            throw std::runtime_error("end must be greater than start");

        if (start > this->_size)
            throw std::runtime_error("start out of bound");

        if (end > this->_size + 1)
            throw std::runtime_error("end out of bound");

        return String(std::move(this->_str.slice(start, end)));
    }

    std::optional<std::size_t> String::find(const String& substr) const
    {
        return this->find(0, substr);
    }

    std::optional<std::size_t> String::find(std::size_t start, const String& substr) const
    {
        if (substr.size() == 0)
            return std::make_optional(0);

        std::size_t j = 0;
        std::size_t pos = 0;

        for (std::size_t i = start; i < this->_size; i++)
        {
            if (this->at(i) != substr[j])
                j = 0;

            if (this->at(i) != substr[j])
                continue;

            if (j == 0)
                pos = i;

            j++;

            if (j == substr.size())
                return std::make_optional(pos);

        }

        return std::nullopt;
    }

    String String::toLower() const
    {
        dsa::String lowered(this->_size);

        for (std::size_t i = 0; i < this->size(); i++)
        {
            char c = this->at(i);

            if (c >= 'A' && c <= 'Z')
                c -= 'A' - 'a';

            lowered.append(c);
        }

        return lowered;
    }

    String String::toUpper() const
    {
        dsa::String uppered(this->_size);

        for (std::size_t i = 0; i < this->size(); i++)
        {
            char c = this->at(i);

            if (c >= 'a' && c <= 'z')
                c += 'A' - 'a';

            uppered.append(c);
        }

        return uppered;
    }

    void String::append(const char c)
    {
        this->_str.insert(this->_size, c);
        this->_size += 1;
    }

    void String::insert(std::size_t idx, char c)
    {
        if (idx > this->_size)
            throw std::runtime_error("index out of bound");

        this->_str.insert(idx, c);
        this->_size++;
    }

    void String::remove(size_t idx)
    {
        if (idx >= this->_size)
            throw std::runtime_error("index out of bound");

        this->_str.remove(idx);
        this->_size--;
    }

    DynamicArray<String> String::split(const String& pattern) const
    {
        if (this->isEmpty())
            return {};
        if (pattern.isEmpty())
            return {1, *this};

        DynamicArray<String> splitted;
        std::size_t start = 0;
        std::size_t end = 0;

        while (start < this->_size)
        {
            auto optEnd = this->find(start, pattern);
            if (optEnd.has_value())
            {
                end = optEnd.value();
            } else
            {
                end = this->_size;
            }

            if (end > start)
                splitted.pushBack(this->substring(start, end));

            start = end + pattern.size();
        }

        return splitted;
    }

    int String::cmp(const String &other) const
    {
        std::size_t i = 0;
        std::size_t j = 0;

        while (i < this->_size && j < other._size)
        {
            const char ci = this->at(i);
            const char cj = other[j];

            if (ci != cj)
                return ci - cj;
            i++;
            j++;
        }

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
