/**
 * @file LList.hpp
 * @author Daniel O'Neill
 * @date 11/13/2024
 * @section 1003
 *
 * @note I pledge my word of honor that I have abided by the
 * CSN Academic Integrity Policy while completing this assignment.
 * @class Vector
 * @brief this class makes a linked list that can move forwards and backwards.
 */

#ifndef LLIST_HPP
#define LLIST_HPP

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <initializer_list>


template <class T>
class LList{

public:
    struct Node {
        T     data{};
        Node* prev{};
        Node* next{};
    };

    class BiDirectionalIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        BiDirectionalIterator(Node* ptr = nullptr) : current(ptr) {}

        BiDirectionalIterator& operator++();
        BiDirectionalIterator  operator++(int);
        BiDirectionalIterator& operator--();
        BiDirectionalIterator  operator--(int);

        reference operator*() const {return current->data; }
        Node* operator->() const {return current; }

        bool operator==(const BiDirectionalIterator& rhs) const {
            return current == rhs.current; }
        bool operator!=(const BiDirectionalIterator& rhs) const {
            return current != rhs.current; }

    private:
        Node* current;
    };

    // Public aliases
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = value_type&;
    using iterator        = BiDirectionalIterator;

    // Constructors and destructor
    /**
     * @brief sets all the values to a null state or 0
     */
    LList() : head(nullptr) , tail(nullptr), count(0) {}
    /**
     * @brief Performs a deep copy of the elements of another LList
     */
    LList(const LList& other);
    /**
     * @brief Transfers ownership of elements from another list to this one
     */
    LList(LList&& other) noexcept;
    /**
     * @brief calls push_back for each element in the list
     */
    LList(const std::initializer_list<T>& init_list);
    /**
     * @brief deletes the LList and sets all the values to a null state or 0
     */
    virtual ~LList() {clear(); }

    // Assignment operators
    /**
     * @brief overides the = operator to copy information
     */
    LList& operator=(const LList& other);
    /**
     * @brief overides the = operator to copy information with no exceptions
     */
    LList& operator=(LList&& other) noexcept;

    // Accessor functions
    /**
     * @brief returns the front of the LList
     */
    reference front();
    /**
     * @brief returns the back of the LList
     */
    reference back();

    // Capacity functions
    /**
     * @brief checks to see if the LList is empty
     */
    bool empty() const {return count == 0; }
    /**
     * @brief returns the count for the LList
     */
    size_type size() const {return count; }

    // Iterator functions
    /**
     * @brief returns the head of the LList
     */
    iterator begin() {return iterator(head); }
    /**
     * @brief rerturns the tail of the LList
     */
    iterator end() {return iterator(nullptr);}

    // Modifiers
    /**
     * @brief removes the elements from the list
     */
    void clear() noexcept;
    /**
     * @brief inserts a value at a specified position
     */
    iterator insert(iterator pos, const T& value);
    /**
     * @brief erases a value at a specified position
     */
    iterator erase(iterator pos);
    /**
     * @brief adds a value to the end of the LList
     */
    void push_back(const T& value);
    /**
     * @brief removes the last elemtent
     */
    void pop_back();
    /**
     * @brief adds an element to the begining
     */
    void push_front(const T& value);
    /**
     * @brief removes the first element
     */
    void pop_front();
    /**
     * @brief swaps the contents of this list with the other list
     */
    void swap(LList& other) noexcept;

    // Friend template functions (comparison operators)
        /**
         * @brief compares this list with the other list to see if they are the same
         */
    template <class U>
    friend bool operator==(const LList<U>& lhs, const LList<U>& rhs);
        /**
         * @brief compares this list with the other list to see if they are
         * not the same
         */
    template <class U>
    friend bool operator!=(const LList<U>& lhs, const LList<U>& rhs);

private:
    Node* head;
    Node* tail;
    size_type count;

};

// =========================================================================
// =========================================================================
// FUNCTION DEFINITIONS
// =========================================================================
// =========================================================================

//// Constructors and destructor

template <class T>
LList<T>::LList(const LList& other) : LList()
{
    for (const Node* node = other.head ; node != nullptr; node = node->next)
    {
        push_back(node->data);
    }
}

template <class T>
LList<T>::LList(LList&& other) noexcept : LList()
{
    this->swap(other);
}

template <class T>
LList<T>::LList(const std::initializer_list<T>& init_list) : LList()
{
    for (const auto value : init_list)
    {
        push_back(value);
    }
}


//// Assignment operators
template <class T>
LList<T>& LList<T>::operator=(const LList& other)
{
    if (this != &other)
    {
        clear();
        count = 0;
        for (const Node* node = other.head; node != nullptr; node = node->next)
        {
            push_back(node->data);
        }
    }
    return *this;
}

template <class T>
LList<T>& LList<T>::operator=(LList&& other) noexcept
{
    if (this != &other)
    {
        clear();
        this->swap(other);
    }
    return *this;
}

//// Accessor functions
template <class T>
typename LList<T>::reference LList<T>::front()
{
    if (empty())
    {
        throw std::out_of_range("cannot call front() on an empty list");
    }
    return head->data;
}

template <class T>
typename LList<T>::reference LList<T>::back()
{
    if (empty())
    {
        throw std::out_of_range("cannot call back() on an empty list");
    }
    return tail->data;
}

//// Capacity functions - defined inline ***************************************

//// Iterator functions - defined inline ***************************************


//// Modifiers
template <class T>
void LList<T>::clear() noexcept
{
    while (!empty())
    {
        try {
            pop_front();  // needs a try due to the clear having noexcept
        } catch (const std::out_of_range& ex) {}  // could have used ...
    }
}

template <class T>
typename LList<T>::iterator LList<T>::insert(iterator pos, const T& value)
{
    Node* current_node = pos.operator->();

    if (current_node == head)
    {
        push_front(value);
        pos = iterator(head);
    }
    else if (current_node == nullptr)
    {
        push_back(value);
        pos = iterator(tail);
    }
    else{
        Node* new_node = new Node{ value, current_node->prev, current_node };

        current_node->prev->next = new_node;
        current_node->prev = new_node;
        ++count;

        pos = iterator(new_node);
    }
    return pos;
}

template <class T>
typename LList<T>::iterator LList<T>::erase(iterator pos)
{
    Node* current_node = pos.operator->();

    if (current_node == nullptr)
    {
        pos = end();
    }
    else if (current_node == head)
    {
        pop_front();
        pos = iterator(head);
    }
    else if (current_node == tail)
    {
        pop_back();
        pos = end();
    }
    else{
        current_node->prev->next = current_node->next;
        current_node->next->prev = current_node->prev;
        --count;

        pos = iterator(current_node->next);
        delete current_node;
    }
    return pos;
}

template <class T>
void LList<T>::push_back(const T& value)
{
    Node* new_node = new Node{value, tail, nullptr} ;

    if (tail)
    {
        tail->next = new_node;
    } else {
        head = new_node;
    }
    tail = new_node;
    ++count;
}

template <class T>
void LList<T>::pop_back()
{
    if (empty())
    {
        throw std::out_of_range("Cannot call pop_back() on an empty list");
    }
    else
    {
        Node* temp = tail;

        tail = tail->prev;

        if (tail)
        {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        --count;
    }
}

template <class T>
void LList<T>::push_front(const T& value)
{
    Node* new_node = new Node{value, nullptr, head} ;

    if (head)
    {
        head->prev = new_node;
    } else {
        tail = new_node;
    }
    head = new_node;
    ++count;
}

template <class T>
void LList<T>::pop_front()
{
    if (empty())
    {
        throw std::out_of_range("Cannot call pop_back() on an empty list");
    }
    else
    {
        Node* temp = head;

        head = head->next;

        if (head)
        {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        --count;
    }
}

template <class T>
void LList<T>::swap(LList& other) noexcept
{
    auto exchange = [](auto& obj1, auto& obj2) {
        auto temp = obj1;
        obj1 = obj2;
        obj2 = temp;
    };
    exchange(head, other.head);
    exchange(tail, other.tail);
    exchange(count, other.count);
}

//// Friend template functions (comparison operators)

template <class U>
bool operator==(const LList<U>& lhs, const LList<U>& rhs)
{
    bool are_equal = true;

    if (lhs.size() != rhs.size())
    {
        are_equal = false;
    } else {
        const typename LList<U>::Node* lit = lhs.head;
        const typename LList<U>::Node* rit = rhs.head;

        for ( ; are_equal && lit != nullptr; lit = lit->next, rit = rit->next)
        {
            if (lit->data != rit->data)
            {
                are_equal = false;
            }
        }
    }

    return are_equal;
}

template <class U>
bool operator!=(const LList<U>& lhs, const LList<U>& rhs)
{
    return !(lhs == rhs);
}

//// Bidirectional operator
template <class T>
typename LList<T>::BiDirectionalIterator&
LList<T>::BiDirectionalIterator::operator++()
{
    if (current)
    {
        current = current->next;
    }
    return *this;
}

template <class T>
typename LList<T>::BiDirectionalIterator
LList<T>::BiDirectionalIterator::operator++(int)
{
    BiDirectionalIterator temp = *this;
    ++(*this);
    return temp;
}

template <class T>
typename LList<T>::BiDirectionalIterator&
LList<T>::BiDirectionalIterator::operator--()
{
    if (current)
    {
        current = current->prev;
    }
    return *this;
}

template <class T>
typename LList<T>::BiDirectionalIterator
LList<T>::BiDirectionalIterator::operator--(int)
{
    BiDirectionalIterator temp = *this;
    ++(*this);
    return temp;
}

#endif /*LLIST_HPP*/
