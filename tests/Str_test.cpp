#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "Str.hpp"

TEST(String, Basic)
{
    const dsa::String s("hello");

    EXPECT_STREQ("hello", s.toCString());
    EXPECT_EQ('e', s[1]);
    EXPECT_EQ('\0', s.toCString()[5]);
    EXPECT_EQ(5, s.size());
    EXPECT_THROW(s[5], std::exception);
}

TEST(String, Concat)
{
    EXPECT_STREQ("hello world", (dsa::String("hello") + dsa::String(" world")).toCString());
    EXPECT_STREQ("hello", (dsa::String("hello") + dsa::String("")).toCString());
    EXPECT_STREQ("world", (dsa::String("") + dsa::String("world")).toCString());

    const dsa::String s1("hello");
    const dsa::String s2("world");
    const dsa::String s3 = s1 + s2;

    EXPECT_STREQ("hello", s1.toCString());
    EXPECT_STREQ("world", s2.toCString());
    EXPECT_STREQ("helloworld", s3.toCString());
    EXPECT_EQ('\0', s3.toCString()[10]);

    dsa::String s("hello");
    s += s2;
    EXPECT_STREQ("helloworld", s.toCString());
}

TEST(String, ctor)
{
    {
        dsa::String s;

        EXPECT_STREQ("", s.toCString());
        EXPECT_EQ('\0', s.toCString()[0]);
        EXPECT_EQ(0, s.size());
    }

    {
        dsa::String s1("foo");
        dsa::String s2(s1);

        EXPECT_STREQ("foo", s1.toCString());
        EXPECT_STREQ("foo", s2.toCString());

        s2 += dsa::String("bar");

        EXPECT_STREQ("foobar", s2.toCString());
        EXPECT_STREQ("foo", s1.toCString());

        dsa::String s3(std::move(s2));

        EXPECT_STREQ("foobar", s3.toCString());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.isEmpty());
        EXPECT_STREQ("", s2.toCString());

        s3 += dsa::String("foo");

        EXPECT_STREQ("foobarfoo", s3.toCString());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.isEmpty());
        EXPECT_STREQ("", s2.toCString());

        s2 += dsa::String("hello");
        EXPECT_STREQ("hello", s2.toCString());
    }

    {
        dsa::String s1("foo");
        dsa::String s2 = s1;

        EXPECT_STREQ("foo", s1.toCString());
        EXPECT_STREQ("foo", s2.toCString());

        s2 += dsa::String("bar");

        EXPECT_STREQ("foobar", s2.toCString());
        EXPECT_STREQ("foo", s1.toCString());

        dsa::String s3 = std::move(s2);

        EXPECT_STREQ("foobar", s3.toCString());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.isEmpty());
        EXPECT_STREQ("", s2.toCString());

        s3 += dsa::String("foo");

        EXPECT_STREQ("foobarfoo", s3.toCString());
        EXPECT_EQ(0, s2.size());
        EXPECT_EQ(true, s2.isEmpty());
        EXPECT_STREQ("", s2.toCString());

        s2 += dsa::String("hello");
        EXPECT_STREQ("hello", s2.toCString());
    }
}

TEST(String, Equality)
{
    EXPECT_EQ(true, dsa::String("foo") == dsa::String("foo"));
    EXPECT_EQ(false, dsa::String("foo") != dsa::String("foo"));

    EXPECT_EQ(false, dsa::String("bar") == dsa::String("foo"));
    EXPECT_EQ(true, dsa::String("bar") != dsa::String("foo"));

    EXPECT_EQ(true, dsa::String("") == dsa::String(""));
    EXPECT_EQ(false, dsa::String("") != dsa::String(""));

    EXPECT_EQ(false, dsa::String("") == dsa::String(" "));
    EXPECT_EQ(true, dsa::String("") != dsa::String(" "));

    EXPECT_EQ(false, dsa::String("1234567890") == dsa::String("1234567891"));
    EXPECT_EQ(true, dsa::String("1234567890") != dsa::String("1234567891"));
}

TEST(String, Comparisons)
{
    EXPECT_EQ(true, dsa::String("a") < dsa::String("b"));
    EXPECT_EQ(false, dsa::String("a") > dsa::String("b"));

    EXPECT_EQ(true, dsa::String("abcdef") < dsa::String("acbdeg"));
    EXPECT_EQ(false, dsa::String("abcdef") > dsa::String("acbdeg"));

    EXPECT_EQ(false, dsa::String("123567") < dsa::String("123456789"));
    EXPECT_EQ(true, dsa::String("123567") > dsa::String("123456789"));

    EXPECT_EQ(true, dsa::String("abc") <= dsa::String("abc"));
    EXPECT_EQ(false, dsa::String("abc") < dsa::String("abc"));

    EXPECT_EQ(false, dsa::String("ab") >= dsa::String("abc"));
    EXPECT_EQ(true, dsa::String("ab") <= dsa::String("abc"));

    EXPECT_EQ(false, dsa::String("") > dsa::String("abc"));
}

TEST(String, Substring)
{
    EXPECT_STREQ("ll", dsa::String("hello").substring(2, 4).toCString());
    EXPECT_STREQ("hello", dsa::String("hello").substring(0, 5).toCString());
    EXPECT_STREQ("", dsa::String("").substring(0, 1).toCString());
    EXPECT_STREQ("a", dsa::String("a").substring(0, 1).toCString());
    EXPECT_STREQ("abc", dsa::String("0123abc456").substring(4, 7).toCString());

    EXPECT_EQ(1, dsa::String("a").substring(0, 1).size());

    EXPECT_THROW(dsa::String("abc").substring(5, 9), std::exception);
    EXPECT_THROW(dsa::String("").substring(1, 2), std::exception);
    EXPECT_THROW(dsa::String("").substring(1, 2), std::exception);
    EXPECT_THROW(dsa::String("abc").substring(0, 6), std::exception);
    EXPECT_THROW(dsa::String("abcefghklm").substring(5, 2), std::exception);
}

TEST(String, find)
{
    EXPECT_EQ(3, dsa::String("foobar").find(("bar")).value());
    EXPECT_EQ(false, dsa::String("foobar").find(("hello")).has_value());
    EXPECT_EQ(4, dsa::String("foobar").find(("a")).value());
    EXPECT_EQ(3, dsa::String("barfoobarfoo").find(("foo")).value());
    EXPECT_EQ(5, dsa::String("baarbbarrbaar").find(("bar")).value());

    EXPECT_EQ(false, dsa::String("").find("bar").has_value());
    EXPECT_EQ(0, dsa::String("").find(("")).value());
    EXPECT_EQ(0, dsa::String("hello").find(("")).value());

    EXPECT_EQ(false, dsa::String("hello").find(("hellohellohello")).has_value());
    EXPECT_EQ(0, dsa::String("hello").find(("hello")).value());

    EXPECT_EQ(false, dsa::String("foo").find(3, "foo").has_value());
}

TEST(String, LowerUpper)
{
    EXPECT_STREQ("hello12", dsa::String("HeLLo12").toLower().toCString());
    EXPECT_STREQ("HELLO12", dsa::String("HeLLo12").toUpper().toCString());
}

TEST(String, InsertAndRemove)
{
    dsa::String s("hello");
    s.insert(1, 'a');
    EXPECT_STREQ("haello", s.toCString());

    s.remove(1);
    EXPECT_STREQ("hello", s.toCString());

    s.insert(4, 'z');
    EXPECT_STREQ("hellzo", s.toCString());

    s.remove(4);
    EXPECT_STREQ("hello", s.toCString());

    s.insert(5, 'z');
    EXPECT_STREQ("helloz", s.toCString());

    s.remove(5);
    EXPECT_STREQ("hello", s.toCString());

    s.insert(0, 'x');
    EXPECT_STREQ("xhello", s.toCString());

    s.remove(0);
    EXPECT_STREQ("hello", s.toCString());

    EXPECT_THROW(s.insert(6, 'z'), std::runtime_error);
    EXPECT_THROW(s.insert(999, 'z'), std::runtime_error);
    EXPECT_THROW(dsa::String("").remove(0), std::runtime_error);
}

void assertEqualToVector(const dsa::DynamicArray<dsa::String> &actual, const std::vector<std::string> &expected)
{
    EXPECT_EQ(actual.size(), expected.size());

    for (std::size_t i = 0; i < actual.size(); i++)
        EXPECT_STREQ(actual[i].toCString(), expected[i].c_str());
}

TEST(String, Split)
{
    {
        auto actual = dsa::String(";;;hello;world;;foo;bar;;;").split(";");
        std::vector<std::string> expected{
            "hello",
            "world",
            "foo",
            "bar"
        };
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("hello;world").split(";");
        std::vector<std::string> expected = {"hello", "world"};
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("").split(";");
        std::vector<std::string>  expected = {};
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("hello").split(";");
        std::vector<std::string>  expected = {"hello"};
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("hello123world12f3oo123bar").split("123");
        std::vector<std::string>  expected = {
            "hello",
            "world12f3oo",
            "bar"
        };
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("hello").split("");
        std::vector<std::string>  expected = {
            "hello",
        };
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("hello").split(";");
        std::vector<std::string>  expected = {
            "hello",
        };
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("hello").split("helloworld");
        std::vector<std::string>  expected = {
            "hello",
        };
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("hello").split("hello");
        std::vector<std::string>  expected = {};
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("helloworld").split("hello");
        std::vector<std::string>  expected = {
            "world",
        };
        assertEqualToVector(actual, expected);
    }

    {
        auto actual = dsa::String("").split("");
        std::vector<std::string>  expected = {};
        assertEqualToVector(actual, expected);
    }
}