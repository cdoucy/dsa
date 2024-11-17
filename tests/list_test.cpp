#include <gtest/gtest.h>
#include <vector>
#include <fmt/printf.h>
#include "list.hpp"

TEST(list, basic)
{
    dsa::list<int> l;

    EXPECT_EQ(false, !!l.head());

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    int i = 0;
    for (auto it = l.head(); it; it = it.next())
    {
        i++;
        EXPECT_EQ(i, it.value());
    }

    for (auto it = l.tail(); it; it = it.prev())
    {
        EXPECT_EQ(i, it.value());
        i--;
    }

    EXPECT_EQ(3, l.pop_back());
    EXPECT_EQ(1, l.pop_front());
    EXPECT_EQ(2, l.pop_back());

    EXPECT_THROW(l.pop_back(), std::runtime_error);
    EXPECT_THROW(l.pop_front(), std::runtime_error);
    EXPECT_THROW(dsa::list<int>().head().value(), std::runtime_error);
    EXPECT_THROW(dsa::list<int>().head().next(), std::runtime_error);
    EXPECT_THROW(dsa::list<int>().head().prev(), std::runtime_error);

    EXPECT_EQ(false, !!dsa::list<int>().head());
    EXPECT_EQ(false, !!dsa::list<int>().tail());

    dsa::list<int> l2;
    l2.push_front(42);
    l2.push_front(84);
    EXPECT_EQ(84, l2.pop_front());
    EXPECT_EQ(true, !!l2.head());
    EXPECT_EQ(true, !!l2.tail());
    EXPECT_EQ(42, l2.pop_front());
    EXPECT_EQ(false, !!l2.head());
    EXPECT_EQ(false, !!l2.tail());
}

TEST(list, find)
{
    {
        dsa::list<int> l;

        EXPECT_EQ(false, !!l.find(1));
        l.push_back(2);
        l.push_back(1);
        l.push_back(3);
        auto found = l.find(1);
        EXPECT_EQ(true, !!found);
        EXPECT_EQ(1, found.value());
        EXPECT_EQ(2, found.prev().value());
        EXPECT_EQ(3, found.next().value());
        EXPECT_EQ(false, !!l.find(42));
    }

    {
        dsa::list<dsa::str> l;

        EXPECT_EQ(false, !!l.find(""));
        l.push_back("2");
        l.push_back("3");
        l.push_back("1");
        auto found = l.find("1");
        EXPECT_EQ(true, !!found);
        EXPECT_STREQ("1", found.value().to_c_str());
        EXPECT_EQ(false, !!found.next());
        EXPECT_STREQ("3", found.prev().value().to_c_str());
        EXPECT_STREQ("2", found.prev().prev().value().to_c_str());
        EXPECT_EQ(false, !!l.find("42"));
    }

    {
        struct dummy
        {
            dsa::str key;
            int value;
        };

        dsa::list<dummy> l;

        EXPECT_EQ(false, !!l.find([](const dummy& d)
        {
            return d.key == "foo";
        }));

        l.push_back(dummy{"1", 100});
        l.push_back(dummy{"2", 200});
        l.push_back(dummy{"3", 300});

        auto found = l.find([](const dummy &d)
        {
          return d.key == "1";
        });

        EXPECT_EQ(true, !!found);
        EXPECT_STREQ("1", found.value().key.to_c_str());
        EXPECT_EQ(100, found.value().value);

        EXPECT_EQ(false, !!found.prev());
        EXPECT_STREQ("2", found.next().value().key.to_c_str());
        EXPECT_EQ(200, found.next().value().value);

        EXPECT_STREQ("3", found.next().next().value().key.to_c_str());
        EXPECT_EQ(300, found.next().next().value().value);

        EXPECT_EQ(false, !!l.find([](const dummy& d) { return d.key == "42";}));
    }
}

TEST(list, move_ctor)
{
    {
        dsa::list<dsa::str> l1;
        l1.push_back("hello");

        // Call move ctor
        dsa::list<dsa::str> l2(std::move(l1));

        EXPECT_TRUE(l1.is_empty());
        EXPECT_FALSE(l2.is_empty());
        EXPECT_STREQ("hello", l2.head().value().to_c_str());


        l2.push_back("world");
        EXPECT_TRUE(l1.is_empty());
        EXPECT_FALSE(l2.is_empty());
        EXPECT_STREQ("world", l2.head().next().value().to_c_str());

        l1.push_back("foo");

        EXPECT_EQ(1, l1.size());
        EXPECT_EQ(2, l2.size());

        EXPECT_STREQ("foo", l1.head().value().to_c_str());
        EXPECT_STREQ("hello", l2.head().value().to_c_str());
        EXPECT_STREQ("world", l2.head().next().value().to_c_str());
    }
}

TEST(list, copy_ctor)
{
    {
        dsa::list<dsa::str> l1;
        l1.push_back("hello");

        // Call copy ctor
        dsa::list<dsa::str> l2(l1);

        EXPECT_FALSE(l1.is_empty());
        EXPECT_FALSE(l2.is_empty());
        EXPECT_STREQ("hello", l1.head().value().to_c_str());
        EXPECT_STREQ("hello", l2.head().value().to_c_str());
        EXPECT_NE(l1.head().value().to_c_str(), l2.head().value().to_c_str());


        l2.push_back("world");
        EXPECT_EQ(1, l1.size());
        EXPECT_EQ(2, l2.size());
        EXPECT_STREQ("world", l2.head().next().value().to_c_str());

        l1.push_back("foo");

        EXPECT_EQ(2, l1.size());
        EXPECT_EQ(2, l2.size());

        EXPECT_STREQ("hello", l1.head().value().to_c_str());
        EXPECT_STREQ("foo", l1.head().next().value().to_c_str());
        EXPECT_STREQ("hello", l2.head().value().to_c_str());
        EXPECT_STREQ("world", l2.head().next().value().to_c_str());

        l2.head().value() += "bar";
        EXPECT_STREQ("hello", l1.head().value().to_c_str());
        EXPECT_STREQ("hellobar", l2.head().value().to_c_str());
    }

    {
        dsa::list<int> l1;

        l1.push_back(1);
        l1.push_back(2);
        l1.push_back(3);

        dsa::list<int> l2(l1);

        for (auto it = l2.head(); it; it = it.next())
        {
            it.value() *= 2;
        }

        auto n1 = l1.head();
        auto n2 = l2.head();

        for (std::size_t i = 0; i < 3; i++)
        {
            EXPECT_EQ(i + 1, n1.value());
            EXPECT_EQ((i + 1) * 2, n2.value());

            ++n1;
            ++n2;
        }
    }

    {
        dsa::list<int> l1;
        dsa::list<int> l2(l1);

        EXPECT_EQ(0, l1.size());
        EXPECT_EQ(0, l2.size());

        l2.push_back(42);
        EXPECT_EQ(0, l1.size());
        EXPECT_EQ(1, l2.size());
    }
}