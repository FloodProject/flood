/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

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
    Array(Allocator &a);
    virtual ~Array();
    Array(const Array &other);
    Array &operator=(const Array &other);
        
    T &operator[](size_t i);
    const T &operator[](size_t i) const;

    T * begin() { return _data; }
    T const * begin() const { return _data; }
    T const * cbegin() const { return _data; }
    T * end() { return _data + _size; }
    T const * end() const { return _data + _size; }
    T const * cend() const { return _data + _size; }

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
    Hash(Allocator &a);
        
    struct Entry {
        uint64 key;
        size_t next;
        T value;
    };

    Entry const * begin() const;
    Entry const * end() const;

    Array<size_t> _hash;
    Array<Entry> _data;
};

NAMESPACE_CORE_END