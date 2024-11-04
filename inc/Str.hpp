//
// Created by cdoucy on 11/4/24.
//

#ifndef STR_HPP
#define STR_HPP

#include "DynamicArray.hpp"

namespace dsa {

class String {
public:
    String() = default;
    ~String() = default;
    explicit String(const char *cString);

    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] const char *toCString() const;

    char operator[](std::size_t index) const;
    String &operator+(const String &other);
    String &operator+=(const String &other);
    String &operator=(const String &other);
    bool operator==(const String &other);
    bool operator!=(const String &other);
    bool operator<(const String &other);
    bool operator>(const String &other);
    bool operator<=(const String &other);
    bool operator>=(const String &other);
    String substring(std::size_t start, std::size_t end) const;
    std::size_t find(const String &substr) const;\
    String toLower() const;
    String toUpper() const;
    void insert(size_t idx, char c);
    void remove(size_t idx);
    DynamicArray<String> split(const String &pattern);




private:
    DynamicArray<char> _str;
    std::size_t _size{};
};

} // dsa

#endif //STR_HPP
