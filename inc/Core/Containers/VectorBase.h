//
// Copyright (c) 2008-2013 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Core/API.h"
#include "Core/Memory.h"
#include "Core/Containers/Swap.h"

NAMESPACE_CORE_BEGIN

/// Random access iterator.
template <class T> struct RandomAccessIterator
{
    /// Construct.
    RandomAccessIterator() :
        ptr(0)
    {
    }
    
    /// Construct with an object pointer.
    explicit RandomAccessIterator(T* ptr) :
        ptr(ptr)
    {
    }
    
    /// Point to the object.
    T* operator -> () const { return ptr; }

    /// Dereference the object.
    T& operator * () const { return *ptr; }

    /// Preincrement the pointer.
    RandomAccessIterator<T>& operator ++ () { ++ptr; return *this; }

    /// Postincrement the pointer.
    RandomAccessIterator<T> operator ++ (int) { RandomAccessIterator<T> it = *this; ++ptr; return it; }

    /// Predecrement the pointer.
    RandomAccessIterator<T>& operator -- () { --ptr; return *this; }

    /// Postdecrement the pointer.
    RandomAccessIterator<T> operator -- (int) { RandomAccessIterator<T> it = *this; --ptr; return it; }

    /// Add an offset to the pointer.
    RandomAccessIterator<T>& operator += (int value) { ptr += value; return *this; }

    /// Subtract an offset from the pointer.
    RandomAccessIterator<T>& operator -= (int value) { ptr -= value; return *this; }

    /// Add an offset to the pointer.
    RandomAccessIterator<T> operator + (int value) const { return RandomAccessIterator<T>(ptr + value); }

    /// Subtract an offset from the pointer.
    RandomAccessIterator<T> operator - (int value) const { return RandomAccessIterator<T>(ptr - value); }

    /// Calculate offset to another iterator.
    int operator - (const RandomAccessIterator& rhs) const { return (int)(ptr - rhs.ptr); }

    /// Test for equality with another iterator.
    bool operator == (const RandomAccessIterator& rhs) const { return ptr == rhs.ptr; }

    /// Test for inequality with another iterator.
    bool operator != (const RandomAccessIterator& rhs) const { return ptr != rhs.ptr; }

    /// Test for less than with another iterator.
    bool operator < (const RandomAccessIterator& rhs) const { return ptr < rhs.ptr; }

    /// Test for greater than with another iterator.
    bool operator > (const RandomAccessIterator& rhs) const { return ptr > rhs.ptr; }

    /// Test for less than or equal with another iterator.
    bool operator <= (const RandomAccessIterator& rhs) const { return ptr <= rhs.ptr; }

    /// Test for greater than or equal with another iterator.
    bool operator >= (const RandomAccessIterator& rhs) const { return ptr >= rhs.ptr; }
    
    /// Pointer.
    T* ptr;
};

/// Random access const iterator.
template <class T> struct RandomAccessConstIterator
{
    /// Construct.
    RandomAccessConstIterator() :
        ptr(0)
    {
    }
    
    /// Construct with an object pointer.
    explicit RandomAccessConstIterator(T* ptr) :
        ptr(ptr)
    {
    }
    
    /// Construct from a non-const iterator.
    RandomAccessConstIterator(const RandomAccessIterator<T>& rhs) :
        ptr(rhs.ptr)
    {
    }
    
    /// Assign from a non-const iterator.
    RandomAccessConstIterator<T>& operator = (const RandomAccessIterator<T>& rhs) { ptr = rhs.ptr; return *this; }
    /// Point to the object.
    const T* operator -> () const { return ptr; }
    /// Dereference the object.
    const T& operator * () const { return *ptr; }
    /// Preincrement the pointer.
    RandomAccessConstIterator<T>& operator ++ () { ++ptr; return *this; }
    /// Postincrement the pointer.
    RandomAccessConstIterator<T> operator ++ (int) { RandomAccessConstIterator<T> it = *this; ++ptr; return it; }
    /// Predecrement the pointer.
    RandomAccessConstIterator<T>& operator -- () { --ptr; return *this; }
    /// Postdecrement the pointer.
    RandomAccessConstIterator<T> operator -- (int) { RandomAccessConstIterator<T> it = *this; --ptr; return it; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<T>& operator += (int value) { ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<T>& operator -= (int value) { ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<T> operator + (int value) const { return RandomAccessConstIterator<T>(ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<T> operator - (int value) const { return RandomAccessConstIterator<T>(ptr - value); }
    /// Calculate offset to another iterator.
    int operator - (const RandomAccessConstIterator& rhs) const { return (int)(ptr - rhs.ptr); }
    /// Test for equality with another iterator.
    bool operator == (const RandomAccessConstIterator& rhs) const { return ptr == rhs.ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const RandomAccessConstIterator& rhs) const { return ptr != rhs.ptr; }
    /// Test for less than with another iterator.
    bool operator < (const RandomAccessConstIterator& rhs) const { return ptr < rhs.ptr; }
    /// Test for greater than with another iterator.
    bool operator > (const RandomAccessConstIterator& rhs) const { return ptr > rhs.ptr; }
    /// Test for less than or equal with another iterator.
    bool operator <= (const RandomAccessConstIterator& rhs) const { return ptr <= rhs.ptr; }
    /// Test for greater than or equal with another iterator.
    bool operator >= (const RandomAccessConstIterator& rhs) const { return ptr >= rhs.ptr; }
    
    /// Pointer.
    T* ptr;
};

/// %Vector base class.
class API_CORE VectorBase
{
template <class T, unsigned N> 
friend class Vector;
public:
    /// Construct.
    VectorBase() :
        size(0),
        capacity(0),
        buffer(0)
    {
    }
    
    /// Swap with another vector.
    void Swap(VectorBase& rhs)
    {
        ::Swap(size, rhs.size);
        ::Swap(capacity, rhs.capacity);
        ::Swap(buffer, rhs.buffer);
    }
    
protected:
    static unsigned char* AllocateHeapBuffer(unsigned size);
    static unsigned char* AllocateStackBuffer(unsigned size);
    
    /// Size of vector.
    unsigned size;
    /// Buffer capacity.
    unsigned capacity;
    /// Buffer.
    unsigned char* buffer;
};

NAMESPACE_CORE_END
