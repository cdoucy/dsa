#include <gtest/gtest.h>
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

    EXPECT_THROW(dsa::String("abc").substring(5, 9), std::exception);
    EXPECT_THROW(dsa::String("").substring(1, 2), std::exception);
    EXPECT_THROW(dsa::String("").substring(1, 2), std::exception);
    EXPECT_THROW(dsa::String("abc").substring(0, 6), std::exception);
}