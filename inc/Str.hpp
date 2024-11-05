//
// Created by cdoucy on 11/4/24.
//

#ifndef STR_HPP
#define STR_HPP

#include "DynamicArray.hpp"

namespace dsa {

class String {
public:
    String();
    ~String() = default;
    explicit String(const char *cString);
    String(const String &other);
    String(String &&other) noexcept ;
    explicit String(DynamicArray<char> &&arr);

    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] const char *toCString() const;

    char operator[](std::size_t index) const;
    String operator+(const String &other) const;
    String &operator+=(const String &other);
    String &operator=(String &&other) noexcept;
    String &operator=(const String &other);
    bool operator==(const String &other) const;
    bool operator!=(const String &other) const;
    bool operator<(const String &other) const;
    bool operator>(const String &other) const;
    bool operator<=(const String &other) const;
    bool operator>=(const String &other) const;
    String substring(std::size_t start, std::size_t end) const;
    std::size_t find(const String &substr) const;
    String toLower() const;
    String toUpper() const;
    void insert(size_t idx, char c);
    void remove(size_t idx);
    DynamicArray<String> split(const String &pattern);
    bool isEmpty() const;
    void reset();
    int cmp(const String &other) const;



private:
    DynamicArray<char> _str;
    std::size_t _size;

    void concat(const String &s);
};

} // dsa

#endif //STR_HPP
