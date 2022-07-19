#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <iostream>
#include <initializer_list>

template <typename Type>
class SingleLinkedList
{
    struct Node
    {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next)
        {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator
    {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) : node_(node)
        {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
        {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept
        {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept
        {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept
        {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept
        {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept
        {
            if (node_ != nullptr)
            {
                node_ = node_->next_node;
            }
            return *this;
        }

        BasicIterator operator++(int) noexcept
        {
            auto old_iter(*this);
            ++(*this);
            return old_iter;
        }

        [[nodiscard]] reference operator*() const noexcept
        {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };


public:

    // ITERATOR PUBLIC...
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept
    {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        if (IsEmpty())
        {
            return ConstIterator(nullptr);
        }
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        if (IsEmpty())
        {
            return ConstIterator(nullptr);
        }
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return ConstIterator(&head_);
    }
    // ...ITERATOR PUBLIC

    SingleLinkedList() : head_(), size_(0)
    {
    }

    SingleLinkedList(std::initializer_list<Type> values)
    {
        InitFromList(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other)
    {
        InitFromList(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs)
    {
        if (this != &rhs)
        {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept
    {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    void PushFront(const Type& value)
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() noexcept
    {
        assert(!IsEmpty());
        Node* tmp = head_.next_node;
        head_.next_node = tmp->next_node;
        --size_;
        delete tmp;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value)
    {
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        Node* tmp = pos.node_->next_node;
        if (tmp != nullptr)
        {
            pos.node_->next_node = tmp->next_node;
            --size_;
            delete tmp;
        }
        return Iterator(pos.node_->next_node);
    }

    void Clear() noexcept
    {
        while (head_.next_node != nullptr)
        {
            Node* tmp = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete tmp;
        }
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        return size_ == 0;
    }

    ~SingleLinkedList()
    {
        Clear();
    }



private:
    Node head_;
    size_t size_;

    template <typename ListIterator>
    void InitFromList(ListIterator begin, ListIterator end)
    {
        SingleLinkedList<Type> tmp;
        Node* prev_node_ptr = &tmp.head_;
        Node* new_node_ptr;

        ListIterator i = begin;
        while (i != end)
        {
            new_node_ptr = new Node(*i, nullptr);
            prev_node_ptr->next_node = new_node_ptr;
            prev_node_ptr = new_node_ptr;
            ++tmp.size_;
            ++i;
        }

        swap(tmp);
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
