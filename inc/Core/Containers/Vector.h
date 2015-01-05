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

FLD_IGNORE_FILE

#include "Core/API.h"
#include "Core/Containers/VectorBase.h"

#include <cassert>
#include <cstring>
#include <new>

NAMESPACE_CORE_BEGIN

/// %Vector template class.
template <class T, unsigned N = 0> class Vector : public VectorBase
{
public:
    typedef RandomAccessIterator<T> Iterator;
    typedef RandomAccessConstIterator<T> ConstIterator;

    /// Construct empty.
    Vector()
    {
        buffer = stackBuffer;
    }

    /**
     * Construct with initial size.
     * @param size initial size
     */
    explicit Vector(unsigned size)
    {
         buffer = stackBuffer;
         Resize(size, 0);
    }

    /**
     * Construct with initial data.
     * @param data initial data pointer
     * @param size 
     */
    Vector(const T* data, unsigned size)
    {
        buffer = stackBuffer;
        Resize(size, data);
    }

    /**
     * Construct with n copies of initial data.
     * @param size number of copies
     * @param data element to copy
     */
    Vector(unsigned size, const T& data)
    {
        buffer = stackBuffer;
        Resize(size);
        auto buff = reinterpret_cast<T*>(buffer);
        for(unsigned i = 0; i < size; ++i)
             new(buff + i) T(data);
    }

    /**
     * Construct from another vector.
     * @param vector vector to build from
     */
    Vector(const Vector<T,N>& vector)
    {
        buffer = stackBuffer;
        *this = vector;
    }

    /**
     * Move construct from another vector.
     * @param vector vector to build from
     */
    Vector(Vector<T,N>&& vector)
    {
        if(vector.isOnHeap())
            buffer = vector.buffer;
        else
            Resize(vector.size, vector.Buffer());

        size = vector.size;
        capacity = vector.capacity;

        vector.buffer = nullptr;
        vector.size = 0;
        vector.capacity = 0;
    }

    /// Destruct.
    ~Vector()
    {
        Clear();
        if(isOnHeap())
            Deallocate(buffer);
    }

    /**
     * Assign from another vector.
     * @param rhs vector to assign from
     * @return this vector
     */
    Vector<T,N>& operator = (const Vector<T,N>& rhs)
    {
        Clear();
        Resize(rhs.size, rhs.Buffer());
        return *this;
    }

    /**
     * Move assign from another vector.
     * @param rhs vector to assign from
     * @return this vector
     */
    Vector<T,N>& operator = (Vector<T,N>&& rhs)
    {
        Clear();
        if(rhs.isOnHeap())
            buffer = rhs.buffer;
        else
            Resize(rhs.size, rhs.Buffer());

        size = rhs.size;
        capacity = rhs.capacity;

        rhs.buffer = nullptr;
        rhs.size = 0;
        rhs.capacity = 0;

        return *this;
    }

    /**
     * Append an element.
     * @param rhs element to append
     * @return this vector
     */
    Vector<T,N>& operator += (const T& rhs)
    {
        Push(rhs);
        return *this;
    }

    /**
     * Append another vector.
     * @param rhs vector to append
     * @return this vector
     */
    Vector<T,N>& operator += (const Vector<T,N>& rhs)
    {
        Push(rhs);
        return *this;
    }

    /**
     * Add an element.
     * @param rhs element to add
     * @return resulting vector
     */
    Vector<T,N> operator + (const T& rhs) const
    {
        Vector<T,N> ret(*this);
        ret.Push(rhs);
        return ret;
    }

    /**
     * Add another vector.
     * @param rhs vector to add
     * @return resulting vector
     */
    Vector<T,N> operator + (const Vector<T,N>& rhs) const
    {
        Vector<T,N> ret(*this);
        ret.Push(rhs);
        return ret;
    }

    /**
     * Test for equality with another vector.
     * @param rhs vector to compare to
     * @return whether vectors are equal
     */
    bool operator == (const Vector<T,N>& rhs) const
    {
        if (rhs.size != size)
            return false;

        T* buffer = Buffer();
        T* rhsBuffer = rhs.Buffer();
        for (unsigned i = 0; i < size; ++i)
        {
            if (buffer[i] != rhsBuffer[i])
                return false;
        }

        return true;
    }

    bool Equals (const Vector<T,N>& rhs) const
    {
        return *this == rhs;
    }

    /**
     * Test for inequality with another vector.
     * @param rhs vector to compare to
     * @return whether vectors are different
     */
    bool operator != (const Vector<T,N>& rhs) const
    {
        if (rhs.size != size)
            return true;

        T* buffer = Buffer();
        T* rhsBuffer = rhs.Buffer();
        for (unsigned i = 0; i < size; ++i)
        {
            if (buffer[i] != rhsBuffer[i])
                return true;
        }

        return false;
    }

    /**
     * Return element at index.
     * @param index element position
     * @return element at index
     */
    T& operator [] (unsigned index) { assert(index < size); return Buffer()[index]; }

    /**
     * Return const element at index.
     * @param index element position
     * @return element at index
     */
    const T& operator [] (unsigned index) const { assert(index < size); return Buffer()[index]; }

    /**
     * Return element at index.
     * @param index element position
     * @return element at index
     */
    T& At(unsigned index) { assert(index < size); return Buffer()[index]; }

    /**
     * Return const element at index.
     * @param index element position
     * @return element at index
     */
    const T& At(unsigned index) const { assert(index < size); return Buffer()[index]; }

    /**
     * Add an element at the end.
     * @param value element to insert
     */
    void Push(const T& value) { Resize(size + 1, &value); }

    /**
     * Add another vector at the end.
     * @param value vector to insert
     */
    void Push(const Vector<T,N>& vector) { Resize(size + vector.size, vector.Buffer()); }

    /// Remove the last element.
    void Pop()
    {
        if (size)
            Resize(size - 1, 0);
    }

    /**
     * Hash vector.
     * @return hash value for HashSet & HashMap
     */
    unsigned ToHash() const
    {
        unsigned hash = 0;
        unsigned char* ptr = buffer;
        unsigned bytes = Bytes();

        for(unsigned i = 0; i < bytes; ++i, ++ptr )
            hash = *ptr + (hash << 6) + (hash << 16) - hash;

        return hash;
    }

    /**
     * Insert an element at position.
     * @param pos position to insert element
     * @param value element to insert
     */
    void Insert(unsigned pos, const T& value)
    {
        if (pos > size)
            pos = size;

        unsigned oldSize = size;
        Resize(size + 1, 0);
        MoveRange(pos + 1, pos, oldSize - pos);
        Buffer()[pos] = value;
    }

    /**
     * Insert another vector at position.
     * @param pos position to insert element
     * @param vector vector to insert
     */
    void Insert(unsigned pos, const Vector<T,N>& vector)
    {
        if (pos > size)
            pos = size;

        unsigned oldSize = size;
        Resize(size + vector.size, 0);
        MoveRange(pos + vector.size, pos, oldSize - pos);
        CopyElements(Buffer() + pos, vector.Buffer(), vector.size);
    }

    /**
     * Insert an element using an iterator.
     * @param dest iterator to element insert position
     * @param value element to insert
     * @return iterator to inserted element
     */
    Iterator Insert(const Iterator& dest, const T& value)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        Insert(pos, value);

        return begin() + pos;
    }

    /**
     * Insert a vector using an iterator.
     * @param dest iterator to vector insert position
     * @param vector vector to insert
     * @return iterator to inserted vector
     */
    Iterator Insert(const Iterator& dest, const Vector<T,N>& vector)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        Insert(pos, vector);

        return begin() + pos;
    }

    /**
     * Insert a vector partially by iterators.
     * @param dest iterator to vector insert position
     * @param start iterator to beginning of elements to be inserted
     * @param end iterator to ending of elements to be inserted
     * @return iterator to inserted vector
     */
     Iterator Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        unsigned length = end - start;
        Resize(size + length, 0);
        MoveRange(pos + length, pos, size - pos - length);

        T* destPtr = Buffer() + pos;
        for (auto it = start; it != end; ++it)
            *destPtr++ = *it;

        return begin() + pos;
    }

    /**
     * Insert sequence of memory contiguous elements.
     * @param dest iterator to elements insert position
     * @param start pointer to first element
     * @param end pointer to last element
     * @return iterator to beginning of inserted elements
     */
    Iterator Insert(const Iterator& dest, const T* start, const T* end)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        unsigned length = end - start;
        Resize(size + length, 0);
        MoveRange(pos + length, pos, size - pos - length);

        T* destPtr = Buffer() + pos;
        for (const T* i = start; i != end; ++i)
            *destPtr++ = *i;

        return begin() + pos;
    }

    /**
     * Erase a range of elements.
     * @param pos position of first element to erase
     * @param length number of elements to erase
     */
    void Erase(unsigned pos, unsigned length = 1)
    {
        // Return if the range is illegal
        if (pos + length > size || !length)
            return;

        MoveRange(pos, pos + length, size - pos - length);
        Resize(size - length, 0);
    }

    /**
     * Erase an element by iterator.
     * @param it iterator to element to erase
     */
    Iterator Erase(const Iterator& it)
    {
        unsigned pos = it - begin();
        if (pos >= size)
            return end();
        Erase(pos);

        return begin() + pos;
    }

    /**
     * Erase a range by iterators.
     * @param start iterator to first element to erase
     * @param end iterator to last element to erase
     */
     Iterator Erase(const Iterator& start, const Iterator& end)
    {
        unsigned pos = start - begin();
        if (pos >= size)
            return end();
        unsigned length = end - start;
        Erase(pos, length);

        return begin() + pos;
    }

    /**
     * Erase an element if found.
     * @param value element to be erased
     * @return whether element was erased
     */
    bool Remove(const T& value)
    {
        Iterator i = Find(value);
        if (i != end())
        {
            Erase(i);
            return true;
        }
        else
            return false;
    }

    /// Clear the vector.
    void Clear() { Resize(0); }

    /**
     * Resize the vector.
     * @param newSize new vector size
     */
    void Resize(unsigned newSize) { Resize(newSize, 0); }

    /**
     * Set new capacity.
     * @param newCapacity new vector capacity
     */
    void Reserve(unsigned newCapacity)
    {
        if (newCapacity < size)
            newCapacity = size;

        if (newCapacity != capacity)
        {
            T* newBuffer = 0;
            capacity = newCapacity;

            if (capacity)
            {
                newBuffer = (isOnHeap() || capacity > N)? reinterpret_cast<T*>(AllocateBuffer(capacity * sizeof(T))) : 
                                    reinterpret_cast<T*>(stackBuffer);
                /// Move the data into the new buffer
                ConstructElements(newBuffer, Buffer(), size);
            }

            /// Delete the old buffer
            DestructElements(Buffer(), size);
            if(isOnHeap())
                Deallocate(buffer);
            buffer = reinterpret_cast<unsigned char*>(newBuffer);
        }
    }

    /// Reallocate so that no extra memory is used.
    void Compact() { Reserve(size); }

    /**
     * Find element.
     * @param value value to find
     * @return iterator to value, or to the end if not found
     */
    Iterator Find(const T& value)
    {
        Iterator it = begin();
        while (it != end() && *it != value)
            ++it;
        return it;
    }

    /**
     * Find element.
     * @param value value to find
     * @return const iterator to value, or to the end if not found
     */
    ConstIterator Find(const T& value) const
    {
        ConstIterator it = begin();
        while (it != end() && *it != value)
            ++it;
        return it;
    }

    /**
     * Check if vector contains element.
     * @param value element to check
     * @return whether vector contains a specific value
     */
    bool Contains(const T& value) const { return Find(value) != end(); }

    /**
     * Get iterator to first element.
     * @return iterator to the beginning
     */
    Iterator begin() { return Iterator(Buffer()); }

    /**
     * Get const iterator to first element.
     * @return const iterator to the beginning
     */
    ConstIterator begin() const { return ConstIterator(Buffer()); }

    /**
     * Get iterator to last element.
     * @return iterator to the end
     */
    Iterator end() { return Iterator(Buffer() + size); }

    /**
     * Get const iterator to last element.
     * @return const iterator to the end
     */
    ConstIterator end() const { return ConstIterator(Buffer() + size); }

    /**
     * Get first element.
     * @return first element
     */
    T& Front() { assert(size); return Buffer()[0]; }

    /**
     * Return const first element.
    const T& Front() const { assert(size); return Buffer()[0]; }

    /**
     * Get last element.
     * @return last element
     */
    T& Back() { assert(size); return Buffer()[size - 1]; }

    /**
     * Return const last element.
     * @return const last element
     */
    const T& Back() const { assert(size); return Buffer()[size - 1]; }

    /**
     * Return size of vector.
     * @return number of elements
     */
    unsigned Size() const { return size; }

    /**
     * Return byte size of vector.
     * @return number of bytes in vector
     */
    unsigned Bytes() const { return size * sizeof(T); }

    /**
     * Return capacity of vector.
     * @return vector capacity
     */
    unsigned Capacity() const { return capacity; }

    /**
     * Check if vector is empty.
     * @return whether vector is empty
     */
    bool Empty() const { return size == 0; }

    /**
     * Get vector buffer.
     * @return the buffer with right type
     */
    T* Buffer() const { return reinterpret_cast<T*>(buffer); }

    /**
     * Resize the vector and create/remove new elements as necessary.
     * @param newSize new size of vector
     * @param src pointer to sequence of new elements
     */
    void Resize(unsigned newSize, const T* src)
    {
        /// If size shrinks, destruct the removed elements
        if (newSize < size)
            DestructElements(Buffer() + newSize, size - newSize);
        else
            AddElements(newSize, src);

        size = newSize;
    }

    /**
     * Resize the vector and create/remove new elements as necessary.
     * @param newSize new size of vector
     * @param org element to fill new positions
     */
    void ResizeSingle(unsigned newSize, const T& org)
    {
        /// If size shrinks, destruct the removed elements
        if (newSize < size)
            DestructElements(Buffer() + newSize, size - newSize);
        else
            AddSingleElements(newSize, org);

        size = newSize;
    }

private:

    bool isOnHeap()
    {
        return buffer != stackBuffer;
    }

    uint8 stackBuffer[N == 0 ? 1 : N * sizeof(T)];

    void AddElements(unsigned newSize, const T* src)
    {
        /// Allocate new buffer if necessary and copy the current elements
        if (newSize > capacity)
        {
            while(capacity < newSize)
                capacity += (!capacity) ? newSize : (capacity + 1) >> 1;

            uint8 * newBuffer = (isOnHeap() || capacity > N)? AllocateBuffer(capacity * sizeof(T))
                : stackBuffer;
            if(newBuffer != stackBuffer)
                AddHeapElements(newBuffer);
        }
        /// Initialize the new elements
        ConstructElements(Buffer() + size, src, newSize - size);
    }

    void AddHeapElements(uint8 * newBuffer)
    {
        if (buffer)
        {
            ConstructElements(reinterpret_cast<T*>(newBuffer), Buffer(), size);
            DestructElements(Buffer(), size);
            if(buffer != stackBuffer)
                Deallocate(buffer);
        }
        buffer = newBuffer;
    }

    void AddSingleElements(unsigned newSize, const T& org)
    {
        /// Allocate new buffer if necessary and copy the current elements
        if (newSize > capacity)
        {
            while(capacity < newSize)
                capacity += (!capacity) ? newSize : (capacity + 1) >> 1;

            uint8 * newBuffer = (isOnHeap() || capacity > N)? AllocateBuffer(capacity * sizeof(T))
                : stackBuffer;
            if(newBuffer != stackBuffer)
                AddHeapElements(newBuffer);
        }
        /// Initialize the new elements
        ConstructSingleElements(Buffer() + size, org, newSize - size);
    }

    /// Move a range of elements within the vector.
    void MoveRange(unsigned dest, unsigned src, unsigned count)
    {
        T* buffer = Buffer();
        if (src < dest)
        {
            for (unsigned i = count - 1; i < count; --i)
                buffer[dest + i] = buffer[src + i];
        }
        if (src > dest)
        {
            for (unsigned i = 0; i < count; ++i)
                buffer[dest + i] = buffer[src + i];
        }
    }

    /// Construct elements, optionally with source data.
    static void ConstructElements(T* dest, const T* src, unsigned count)
    {
        if (!src)
        {
            for (unsigned i = 0; i < count; ++i)
                new(dest + i) T();
        }
        else
        {
            for (unsigned i = 0; i < count; ++i)
                new(dest + i) T(*(src + i));
        }
    }

    /// Construct elements, optionally with source data.
    static void ConstructSingleElements(T* dest, const T& org, unsigned count)
    {
        for (unsigned i = 0; i < count; ++i)
            new(dest + i) T(org);
    }

    /// Copy elements from one buffer to another.
    static void CopyElements(T* dest, const T* src, unsigned count)
    {
        for (unsigned i = 0; i < count; ++i)
            dest[i] = src[i];
    }

    /// Call the elements' destructors.
    static void DestructElements(T* dest, unsigned count)
    {
        for (unsigned i = 0; i < count; ++i)
            (dest + i)->~T();
    }
};

NAMESPACE_CORE_END
