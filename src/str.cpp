//
// Created by cdoucy on 11/4/24.
//

#include "str.hpp"

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

    static array<char> array_from_c_str(const char* s)
    {
        const std::size_t len = strlen(s);
        array<char> arr(len + 1);

        for (std::size_t i = 0; i < len; i++)
            arr[i] = s[i];

        arr[len] = '\0';

        return arr;
    }

    str::str()
        : _str(1, '\0'),
          _size(0)
    {
    }

    str::str(const char* cString)
        : _str(array_from_c_str(cString)),
          _size(strlen(cString))
    {
    }

    str::str(const str& other)
        : _str(other._str),
          _size(other._size)
    {
    }

    str::str(str&& other) noexcept
        : _str(std::move(other._str)),
          _size(other._size)
    {
        other.reset();
    }

    str::str(array<char>&& arr)
        : _str(std::move(arr)),
          _size(this->_str.size())
    {
        this->_str.push_back('\0');
    }

    str::str(std::size_t capacity)
        : _str(capacity, '\0'),
          _size(0)

    {
    }

    std::size_t str::size() const
    {
        return this->_size;
    }

    const char* str::to_c_str() const
    {
        return this->_str.data();
    }

    char str::operator[](const std::size_t index) const
    {
        return this->at(index);
    }

    char str::at(const std::size_t index) const
    {
        if (index >= this->_size)
            throw std::runtime_error("string index out of range");
        return this->_str[index];
    }


    str str::operator+(const str& other) const
    {
        str res(*this);
        res.concat(other);
        return res;
    }

    str& str::operator+=(const str& other)
    {
        this->concat(other);
        return *this;
    }

    str& str::operator=(str&& other) noexcept
    {
        this->_str = std::move(other._str);
        this->_size = other._size;

        return *this;
    }

    str& str::operator=(const str& other)
    {
        if (this == &other)
            return *this;
        this->_str = other._str;
        this->_size = other > _size;

        return *this;
    }

    bool str::operator==(const str& other) const
    {
        return this->cmp(other) == 0;
    }

    bool str::operator!=(const str& other) const
    {
        return this->cmp(other) != 0;
    }

    bool str::operator<(const str& other) const
    {
        return this->cmp(other) < 0;
    }

    bool str::operator>(const str& other) const
    {
        return this->cmp(other) > 0;
    }

    bool str::operator<=(const str& other) const
    {
        return this->cmp(other) <= 0;
    }

    bool str::operator>=(const str& other) const
    {
        return this->cmp(other) >= 0;
    }

    str str::substring(std::size_t start, std::size_t end) const
    {
        if (start == end)
            throw std::runtime_error("start and end must be different");

        if (end < start)
            throw std::runtime_error("end must be greater than start");

        if (start > this->_size)
            throw std::runtime_error("start out of bound");

        if (end > this->_size + 1)
            throw std::runtime_error("end out of bound");

        return str(std::move(this->_str.slice(start, end)));
    }

    std::optional<std::size_t> str::find(const str& substr) const
    {
        return this->find(0, substr);
    }

    std::optional<std::size_t> str::find(std::size_t start, const str& substr) const
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

    str str::to_lower() const
    {
        dsa::str lowered(this->_size);

        for (std::size_t i = 0; i < this->size(); i++)
        {
            char c = this->at(i);

            if (c >= 'A' && c <= 'Z')
                c -= 'A' - 'a';

            lowered.append(c);
        }

        return lowered;
    }

    str str::to_upper() const
    {
        dsa::str uppered(this->_size);

        for (std::size_t i = 0; i < this->size(); i++)
        {
            char c = this->at(i);

            if (c >= 'a' && c <= 'z')
                c += 'A' - 'a';

            uppered.append(c);
        }

        return uppered;
    }

    void str::append(const char c)
    {
        this->_str.insert(this->_size, c);
        this->_size += 1;
    }

    void str::insert(std::size_t idx, char c)
    {
        if (idx > this->_size)
            throw std::runtime_error("index out of bound");

        this->_str.insert(idx, c);
        this->_size++;
    }

    void str::remove(size_t idx)
    {
        if (idx >= this->_size)
            throw std::runtime_error("index out of bound");

        this->_str.remove(idx);
        this->_size--;
    }

    array<str> str::split(const str& pattern) const
    {
        if (this->is_empty())
            return {};
        if (pattern.is_empty())
            return {1, *this};

        array<str> splitted;
        std::size_t start = 0;
        std::size_t end = 0;

        while (start < this->_size)
        {
            auto optEnd = this->find(start, pattern);
            if (optEnd.has_value())
            {
                end = optEnd.value();
            }
            else
            {
                end = this->_size;
            }

            if (end > start)
                splitted.push_back(this->substring(start, end));

            start = end + pattern.size();
        }

        return splitted;
    }

    int str::cmp(const str& other) const
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


    bool str::is_empty() const
    {
        return this->_size == 0;
    }

    void str::reset()
    {
        this->_str = array<char>(1, '\0');
        this->_size = 0;
    }

    void str::concat(const str& s)
    {
        if (this->_str.size() != 0)
            // Remove trailing '\0'
            this->_str.pop_back();

        for (std::size_t i = 0; i < s.size(); i++)
        {
            this->_str.push_back(s[i]);
        }

        this->_str.push_back('\0');

        this->_size += s.size();
    }
} // dsa
