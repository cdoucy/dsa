#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "str.hpp"

TEST(String, Basic)
{
    const dsa::str s("hello");

    EXPECT_STREQ("hello", s.to_c_str());
    EXPECT_EQ('e', s[1]);
    EXPECT_EQ('\0', s.to_c_str()[5]);
    EXPECT_EQ(5, s.size());
    EXPECT_THROW(s[5], std::exception);
}

TEST(String, Concat)
{
    EXPECT_STREQ("hello world", (dsa::str("hello") + dsa::str(" world")).to_c_str());
    EXPECT_STREQ("hello", (dsa::str("hello") + dsa::str("")).to_c_str());
    EXPECT_STREQ("world", (dsa::str("") + dsa::str("world")).to_c_str());

    const dsa::str s1("hello");
    const dsa::str s2("world");
    const dsa::str s3 = s1 + s2;

    EXPECT_STREQ("hello", s1.to_c_str());
    EXPECT_STREQ("world", s2.to_c_str());
    EXPECT_STREQ("helloworld", s3.to_c_str());
    EXPECT_EQ('\0', s3.to_c_str()[10]);

    dsa::str s("hello");
    s += s2;
    EXPECT_STREQ("helloworld", s.to_c_str());
}

TEST(String, ctor)
{
    {
        dsa::str s;

        EXPECT_STREQ("", s.to_c_str());
        EXPECT_EQ('\0', s.to_c_str()[0]);
        EXPECT_EQ(0, s.size());
    }

    {
        dsa::str s1("foo");
        dsa::str s2(s1);

        EXPECT_STREQ("foo", s1.to_c_str());
        EXPECT_STREQ("foo", s2.to_c_str());

        s2 += dsa::str("bar");

        EXPECT_STREQ("foobar", s2.to_c_str());
        EXPECT_STREQ("foo", s1.to_c_str());

        dsa::str s3(std::move(s2));

        EXPECT_STREQ("foobar", s3.to_c_str());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.is_empty());
        EXPECT_STREQ("", s2.to_c_str());

        s3 += dsa::str("foo");

        EXPECT_STREQ("foobarfoo", s3.to_c_str());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.is_empty());
        EXPECT_STREQ("", s2.to_c_str());

        s2 += dsa::str("hello");
        EXPECT_STREQ("hello", s2.to_c_str());
    }

    {
        dsa::str s1("foo");
        dsa::str s2 = s1;

        EXPECT_STREQ("foo", s1.to_c_str());
        EXPECT_STREQ("foo", s2.to_c_str());

        s2 += dsa::str("bar");

        EXPECT_STREQ("foobar", s2.to_c_str());
        EXPECT_STREQ("foo", s1.to_c_str());

        dsa::str s3 = std::move(s2);

        EXPECT_STREQ("foobar", s3.to_c_str());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.is_empty());
        EXPECT_STREQ("", s2.to_c_str());

        s3 += dsa::str("foo");

        EXPECT_STREQ("foobarfoo", s3.to_c_str());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.is_empty());
        EXPECT_STREQ("", s2.to_c_str());

        s2 += dsa::str("hello");
        EXPECT_STREQ("hello", s2.to_c_str());
    }
}

TEST(String, Equality)
{
    EXPECT_EQ(true, dsa::str("foo") == dsa::str("foo"));
    EXPECT_EQ(false, dsa::str("foo") != dsa::str("foo"));

    EXPECT_EQ(false, dsa::str("bar") == dsa::str("foo"));
    EXPECT_EQ(true, dsa::str("bar") != dsa::str("foo"));

    EXPECT_EQ(true, dsa::str("") == dsa::str(""));
    EXPECT_EQ(false, dsa::str("") != dsa::str(""));

    EXPECT_EQ(false, dsa::str("") == dsa::str(" "));
    EXPECT_EQ(true, dsa::str("") != dsa::str(" "));

    EXPECT_EQ(false, dsa::str("1234567890") == dsa::str("1234567891"));
    EXPECT_EQ(true, dsa::str("1234567890") != dsa::str("1234567891"));
}

TEST(String, Comparisons)
{
    EXPECT_EQ(true, dsa::str("a") < dsa::str("b"));
    EXPECT_EQ(false, dsa::str("a") > dsa::str("b"));

    EXPECT_EQ(true, dsa::str("abcdef") < dsa::str("acbdeg"));
    EXPECT_EQ(false, dsa::str("abcdef") > dsa::str("acbdeg"));

    EXPECT_EQ(false, dsa::str("123567") < dsa::str("123456789"));
    EXPECT_EQ(true, dsa::str("123567") > dsa::str("123456789"));

    EXPECT_EQ(true, dsa::str("abc") <= dsa::str("abc"));
    EXPECT_EQ(false, dsa::str("abc") < dsa::str("abc"));

    EXPECT_EQ(false, dsa::str("ab") >= dsa::str("abc"));
    EXPECT_EQ(true, dsa::str("ab") <= dsa::str("abc"));

    EXPECT_EQ(false, dsa::str("") > dsa::str("abc"));
}

TEST(String, Substring)
{
    EXPECT_STREQ("ll", dsa::str("hello").substring(2, 4).to_c_str());
    EXPECT_STREQ("hello", dsa::str("hello").substring(0, 5).to_c_str());
    EXPECT_STREQ("", dsa::str("").substring(0, 1).to_c_str());
    EXPECT_STREQ("a", dsa::str("a").substring(0, 1).to_c_str());
    EXPECT_STREQ("abc", dsa::str("0123abc456").substring(4, 7).to_c_str());

    EXPECT_EQ(1, dsa::str("a").substring(0, 1).size());

    EXPECT_THROW(dsa::str("abc").substring(5, 9), std::exception);
    EXPECT_THROW(dsa::str("").substring(1, 2), std::exception);
    EXPECT_THROW(dsa::str("").substring(1, 2), std::exception);
    EXPECT_THROW(dsa::str("abc").substring(0, 6), std::exception);
    EXPECT_THROW(dsa::str("abcefghklm").substring(5, 2), std::exception);
}

TEST(String, find)
{
    EXPECT_EQ(3, dsa::str("foobar").find(("bar")).value());
    EXPECT_EQ(false, dsa::str("foobar").find(("hello")).has_value());
    EXPECT_EQ(4, dsa::str("foobar").find(("a")).value());
    EXPECT_EQ(3, dsa::str("barfoobarfoo").find(("foo")).value());
    EXPECT_EQ(5, dsa::str("baarbbarrbaar").find(("bar")).value());

    EXPECT_EQ(false, dsa::str("").find("bar").has_value());
    EXPECT_EQ(0, dsa::str("").find(("")).value());
    EXPECT_EQ(0, dsa::str("hello").find(("")).value());

    EXPECT_EQ(false, dsa::str("hello").find(("hellohellohello")).has_value());
    EXPECT_EQ(0, dsa::str("hello").find(("hello")).value());

    EXPECT_EQ(false, dsa::str("foo").find(3, "foo").has_value());
}

TEST(String, LowerUpper)
{
    EXPECT_STREQ("hello12", dsa::str("HeLLo12").to_lower().to_c_str());
    EXPECT_STREQ("HELLO12", dsa::str("HeLLo12").to_upper().to_c_str());
}

TEST(String, InsertAndRemove)
{
    dsa::str s("hello");
    s.insert(1, 'a');
    EXPECT_STREQ("haello", s.to_c_str());

    s.remove(1);
    EXPECT_STREQ("hello", s.to_c_str());

    s.insert(4, 'z');
    EXPECT_STREQ("hellzo", s.to_c_str());

    s.remove(4);
    EXPECT_STREQ("hello", s.to_c_str());

    s.insert(5, 'z');
    EXPECT_STREQ("helloz", s.to_c_str());

    s.remove(5);
    EXPECT_STREQ("hello", s.to_c_str());

    s.insert(0, 'x');
    EXPECT_STREQ("xhello", s.to_c_str());

    s.remove(0);
    EXPECT_STREQ("hello", s.to_c_str());

    EXPECT_THROW(s.insert(6, 'z'), std::runtime_error);
    EXPECT_THROW(s.insert(999, 'z'), std::runtime_error);
    EXPECT_THROW(dsa::str("").remove(0), std::runtime_error);
}

void assert_equal_to_vector(const dsa::array<dsa::str> &actual, const std::vector<std::string> &expected)
{
    EXPECT_EQ(actual.size(), expected.size());

    for (std::size_t i = 0; i < actual.size(); i++)
        EXPECT_STREQ(actual[i].to_c_str(), expected[i].c_str());
}

TEST(String, Split)
{
    {
        auto actual = dsa::str(";;;hello;world;;foo;bar;;;").split(";");
        std::vector<std::string> expected{
            "hello",
            "world",
            "foo",
            "bar"
        };
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("hello;world").split(";");
        std::vector<std::string> expected = {"hello", "world"};
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("").split(";");
        std::vector<std::string>  expected = {};
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("hello").split(";");
        std::vector<std::string>  expected = {"hello"};
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("hello123world12f3oo123bar").split("123");
        std::vector<std::string>  expected = {
            "hello",
            "world12f3oo",
            "bar"
        };
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("hello").split("");
        std::vector<std::string>  expected = {
            "hello",
        };
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("hello").split(";");
        std::vector<std::string>  expected = {
            "hello",
        };
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("hello").split("helloworld");
        std::vector<std::string>  expected = {
            "hello",
        };
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("hello").split("hello");
        std::vector<std::string>  expected = {};
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("helloworld").split("hello");
        std::vector<std::string>  expected = {
            "world",
        };
        assert_equal_to_vector(actual, expected);
    }

    {
        auto actual = dsa::str("").split("");
        std::vector<std::string>  expected = {};
        assert_equal_to_vector(actual, expected);
    }
}