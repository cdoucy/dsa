//
// Created by cdoucy on 11/16/24.
//

#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <functional>
#include "str.hpp"

namespace dsa
{
    template <class T>
    class list
    {
    private:
        struct node;

    public:
        class itr;

    public:
        using find_func = std::function<bool (const T&)>;

        list() : _size(0), _head(NULL), _tail(NULL)
        {
        };

        list(const list& other) : list()
        {
            if (other._size == 0)
                return;
            this->_size = other._size;
            this->_head = new_node();
            node *n = this->_head;
            node *prev = NULL;

            for (auto node = other._head; node != NULL; node = node->next)
            {
                n->val = node->val;
                n->prev = prev;
                prev = n;

                if (node->next)
                {
                    n->next = new_node();
                    n = n->next;
                } else
                    this->_tail = n;
            }
        }

        list(list&& other)
            : _size(other._size),
              _head(other._head),
              _tail(other._tail)
        {
            other._size = 0;
            other._head = NULL;
            other._tail = NULL;
        }

        ~list()
        {
            if (this->_size == 0)
                return;

            for (auto node = this->_head; node;)
            {
                auto next = node->next;
                delete node;
                node = next;
            }
        };

        list<T>& push_back(const T& v)
        {
            return this->push_back_impl(v);
        }

        list<T>& push_back(T&& v)
        {
            return this->push_back_impl(std::move(v));
        }

        list<T>& push_front(const T& v)
        {
            return this->push_front_impl(v);
        }

        list<T>& push_front(T&& v)
        {
            return this->push_front_impl(std::move(v));
        }

        T pop_back()
        {
            if (this->is_empty())
                throw std::runtime_error("list::pop_back(): list is empty");

            node* n = this->_tail;

            if (this->_size == 1)
            {
                this->_head = NULL;
                this->_tail = NULL;
            }
            else
            {
                this->_tail = n->prev;
                this->_tail->next = NULL;
            }

            auto val = std::move(n->val);
            delete n;

            --this->_size;

            return val;
        }

        T pop_front()
        {
            if (this->is_empty())
                throw std::runtime_error("list::pop_front(): list is empty");

            auto* n = this->_head;

            if (this->_size == 1)
            {
                this->_head = NULL;
                this->_tail = NULL;
            }
            else
            {
                this->_head = n->next;
                this->_head->prev = NULL;
            }

            auto val = std::move(n->val);
            delete n;

            --this->_size;

            return val;
        }

        std::size_t size() const
        {
            return this->_size;
        }

        itr head()
        {
            if (this->is_empty())
                return itr();

            return itr(this->_head);
        }

        itr head() const
        {
            if (this->is_empty())
                return itr();

            return itr(this->_head);
        }

        itr tail()
        {
            if (this->is_empty())
                return itr();

            return itr(this->_tail);
        }

        itr tail() const
        {
            if (this->is_empty())
                return itr();

            return itr(this->_tail);
        }

        bool is_empty() const
        {
            return this->_size == 0;
        };

        itr find(const T& target) const
        {
            return this->find([&target](const T& v)
            {
                return v == target;
            });
        }

        itr find(find_func f) const
        {
            for (auto it = this->head(); it; it = it.next())
            {
                if (f(it.value()))
                    return it;
            }

            return itr();
        }

    private:
        std::size_t _size;
        node* _head;
        node* _tail;

        template <typename U>
        list<T>& push_back_impl(U&& v)
        {
            auto n = new_node(std::forward<U>(v));

            if (this->is_empty())
            {
                this->_head = n;
                this->_tail = n;
            }
            else
            {
                n->prev = this->_tail;
                this->_tail->next = n;
                this->_tail = n;
            }

            ++this->_size;
            return *this;
        }

        template <typename U>
        list<T>& push_front_impl(U&& v)
        {
            auto n = new_node(std::forward<U>(v));

            if (this->is_empty())
            {
                this->_head = n;
                this->_tail = n;
            }
            else
            {
                n->next = this->_head;
                this->_head->prev = n;
                this->_head = n;
            }

            ++this->_size;
            return *this;
        }


        struct node
        {
            node* next;
            node* prev;
            T val;

            node() : next(nullptr), prev(nullptr), val()
            {
            };
        };

        node* new_node()
        {
            node* n = new node;
            n->val = T{};
            return n;
        }

        node* new_node(const T& val)
        {
            node* n = new node;
            n->val = val;
            return n;
        }

        node* new_node(T&& val)
        {
            node* n = new node;
            n->val = std::move(val);
            return n;
        }

    public:
        class itr
        {
        public:
            itr() : _node(nullptr)
            {
            };

            itr(node* node) : _node(node)
            {
            };

            ~itr() = default;

            itr& operator++()
            {
                this->check_node("operator++");
                this->_node = this->_node->next;
                return *this;
            }

            itr& operator--()
            {
                this->check_node("operator--");
                this->_node = this->_node->prev;
                return *this;
            }

            itr next() const
            {
                this->check_node("next()");
                return itr(this->_node->next);
            }

            itr prev() const
            {
                this->check_node("prev()");
                return itr(this->_node->prev);
            }

            explicit operator bool() const
            {
                return this->_node != NULL;
            }

            T& value()
            {
                this->check_node("T value()");

                return this->_node->val;
            }

            const T& value() const
            {
                this->check_node("const T &value()");
                return this->_node->val;
            }

        private:
            node* _node;

            void check_node(const char* where) const
            {
                const str msg = str(where) + " : node is null";
                if (this->_node == NULL)
                    throw std::runtime_error(msg.to_c_str());
            }
        };
    };
} // dsa

#endif //LIST_HPP
