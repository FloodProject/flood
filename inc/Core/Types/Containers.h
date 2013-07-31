/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"

/// All collection types assume that they are used to store POD objects. I.e. they:
///
/// * Don't call constructors and destructors on elements.
/// * Move elements with memmove().
///
/// If you want to store items that are not PODs, use something other than these collection
/// classes.

NAMESPACE_CORE_BEGIN

/// Dynamically resizable array of POD objects.
template<typename T> struct Array
{
public:
    Array(Allocator &a);
    Array(const Array &other);
    ~Array();
    Array &operator=(const Array &other);
        
    T &operator[](size_t i);
    const T &operator[](size_t i) const;

    T * begin();
    T const * begin() const;
    T const * cbegin() const;
    T * end();
    T const * end() const;
    T const * cend() const;

    T& front();
    T const & front() const;
    T& back();
    T const & back() const;

    size_t size() const;
    size_t capacity() const;
    bool any() const;
    bool empty() const;
    
    void resize(size_t new_size);
    void reserve(size_t new_capacity);
    void trim();
    void clear();

    void push_back(T const & item);
    void pop_back();

    template <typename Iter>
    void insert(T * pos, Iter first, Iter last);
    void remove(T const * item);

    Allocator * allocator();
    Allocator const * allocator() const;
    
private:
    void set_capacity(size_t new_capacity);
    void grow(size_t min_capacity = 0);

private:
    Allocator *_allocator;
    size_t _size;
    size_t _capacity;
    T *_data;
};

/// A double-ended queue/ring buffer.
template <typename T> struct Queue
{
    Queue(Allocator &a);

    T &operator[](size_t i);
    const T &operator[](size_t i) const;

    Array<T> _data;
    size_t _size;
    size_t _offset;
};

/// Hash from an uint64_t to POD objects. If you want to use a generic key
/// object, use a hash function to map that object to an uint64_t.
template<typename T> struct Hash
{
public:
    struct Entry
    {
        uint64 key;
        size_t next;
        T value;
    };

public:
    Hash(Allocator &a);

    Entry const * begin() const;
    Entry const * end() const;

    size_t size() const;
    bool empty() const;
    void reserve(size_t size);
    void clear();
    
    T const & get(uint64 key, T const & default_value) const;
    void set(uint64 key, T const & value);
    bool has(uint64 key) const;
    void remove(uint64 key);

private:
    const static size_t END_OF_LIST;
    const static float MAX_LOAD_FACTOR;
    struct FindResult
    {
        size_t hash_i;
        size_t data_prev;
        size_t data_i;
    };

    bool full() const;
    void rehash(size_t new_size);
    size_t add_entry(uint64 key);
    size_t make(uint64 key);
    void insert(uint64 key, const T &value);
    void erase(FindResult const & fr);
    void grow();

    FindResult find(uint64 key) const;
    FindResult find(Entry const * e) const;
    size_t find_or_fail(uint64 key) const;
    size_t find_or_make(uint64 key);
    void find_and_erase(uint64 key);

private:
    Array<size_t> _hash;
    Array<Entry> _data;
};

NAMESPACE_CORE_END