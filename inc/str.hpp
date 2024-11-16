//
// Created by cdoucy on 11/4/24.
//

#ifndef STR_HPP
#define STR_HPP

#include "array.hpp"

namespace dsa {

class str {
public:
    str();
    ~str() = default;
    str(const char *cString);
    str(const str &other);
    str(str &&other) noexcept ;
    str(array<char> &&arr);
    str(std::size_t capacity);

    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] const char *to_c_str() const;

    char operator[](std::size_t index) const;
    [[nodiscard]] char at(std::size_t index) const;
    str operator+(const str &other) const;
    str &operator+=(const str &other);
    str &operator=(str &&other) noexcept;
    str &operator=(const str &other);
    bool operator==(const str &other) const;
    bool operator!=(const str &other) const;
    bool operator<(const str &other) const;
    bool operator>(const str &other) const;
    bool operator<=(const str &other) const;
    bool operator>=(const str &other) const;
    [[nodiscard]] str substring(std::size_t start, std::size_t end) const;
    [[nodiscard]] std::optional<std::size_t> find(const str &substr) const;
    [[nodiscard]] std::optional<std::size_t> find(std::size_t start, const str &substr) const;
    [[nodiscard]] str to_lower() const;
    [[nodiscard]] str to_upper() const;
    void append(char c);
    void insert(size_t idx, char c);
    void remove(size_t idx);
    array<str> split(const str &pattern) const;
    bool is_empty() const;
    void reset();
    int cmp(const str &other) const;


private:
    array<char> _str;
    std::size_t _size;

    void concat(const str &s);
};

} // dsa

#endif //STR_HPP
