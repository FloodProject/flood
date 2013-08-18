/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include <type_traits>

NAMESPACE_CORE_BEGIN

/**
 *	A dynamic array container.
 */
template<typename T> class Array
{
public:
    
    /**
     *	Creates the Array.
     *	@param a allocator serving any memory requests
     */
    Array(Allocator& a);
    ~Array();

    Array(const Array& other);
    Array(Array&& other);
    
    Array& operator=(const Array &other);
    Array& operator=(Array&& other);
        
    /**
     *	Indexing operator.
     *	@param i index of item
     */
    T &operator[](size_t i);
    const T &operator[](size_t i) const;

    /**
     *	Returns a pointer to the head of the array.
     */
    T * begin();
    T const * begin() const;
    T const * cbegin() const;

    /**
     *	Returns a pointer past the end of the array.
     */
    T * end();
    T const * end() const;
    T const * cend() const;

    /**
     *	Returns a pointer to the head of the array.
     */
    T * data();
    T const * data() const;

    /**
     *	Returns a reference to the first item.
     */
    T& front();
    T const & front() const;

    /**
     *	Returns a reference to the last item.
     */
    T& back();
    T const & back() const;

    /**
     *	Returns the number of elements.
     */
    size_t size() const;

    /**
     *	Returns the number of allocated elements.
     */
    size_t capacity() const;

    /**
     *	Returns size() != 0.
     */
    bool any() const;

    /**
     *	Returns size() == 0.
     */
    bool empty() const;
    
    /**
     *	Resizes the array to contain the specified number of elements.
     *	@param new_size number of elements
     */
    void resize(size_t new_size);

    /**
     *	Allocates the memory required to hold the provided number of elements.
     *	@param new_capacity number of elements
     */
    void reserve(size_t new_capacity);

    /**
     *	Sets the capacity equivalent to the size.
     */
    void trim();

    /**
     *	Sets the size to zero.
     */
    void clear();

    /**
     *	Appends the item to the back of the array.
     *	@param item item
     */
    void push_back(T const & item);

    /**
     *	Removes the last item from the array.
     */
    void pop_back();

    /**
     *	Inserts a sequence of items into the array at the specified position.
     *	@param pos position to insert items
     *	@param first iterator to the first item in the source sequence
     *	@param last iterator to the last item in the source sequence
     */
    template <typename Iter>
    void insert(T * pos, Iter first, Iter last);

    /**
     *	Removes the specified element.
     *	@param item item
     */
    void remove(T const * item);

    /**
     *	Returns the allocator serving the array's memory requests.
     */
    Allocator * allocator();
    Allocator const * allocator() const;
    
private:
    void set_capacity(size_t new_capacity);
    void grow(size_t min_capacity = 0);

    void construct_range(T * data, size_t count, std::true_type);
    void construct_range(T * data, size_t count, std::false_type);
    void copy_range(T * data, T * src, size_t count, std::true_type);
    void copy_range(T * data, T * src, size_t count, std::false_type);
    void move_range(T * data, T * src, size_t count, std::true_type);
    void move_range(T * data, T * src, size_t count, std::false_type);
    void destruct_range(T * data, size_t count, std::true_type);
    void destruct_range(T * data, size_t count, std::false_type);

private:
    Allocator *_allocator;
    size_t _size;
    size_t _capacity;
    T *_data;
};

NAMESPACE_CORE_END