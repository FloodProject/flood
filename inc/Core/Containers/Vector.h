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
		: onHeap(false)
    {
    }
    
    /// Construct with initial size.
    explicit Vector(unsigned size)
		: onHeap(false)
    {
        Resize(size, 0);
    }
    
    /// Construct with initial data.
    Vector(const T* data, unsigned size)
		: onHeap(false)
    {
        Resize(size, data);
    }
    
    /// Construct from another vector.
    Vector(const Vector<T,N>& vector)
    {
        *this = vector;
    }

	/// Move construct from another vector.
    Vector(Vector<T,N>&& vector)
    {
		buffer = vector.buffer;
		size = vector.size;
		capacity = vector.capacity;
		
		vector.buffer = nullptr;
		vector.size = 0;
		vector.capacity = 0;
		onHeap = vector.onHeap;
    }
    
    /// Destruct.
    ~Vector()
    {
        Clear();
		if(onHeap)
			Deallocate(buffer);
    }
    
    /// Assign from another vector.
    Vector<T,N>& operator = (const Vector<T,N>& rhs)
    {
        Clear();
        Resize(rhs.size, rhs.Buffer());
        return *this;
    }
	
	/// Assign from another vector.
    Vector<T,N>& operator = (Vector<T,N>&& rhs)
    {
        Clear();
		buffer = rhs.buffer;
		size = rhs.size;
		capacity = rhs.capacity;
		
		rhs.buffer = nullptr;
		rhs.size = 0;
		rhs.capacity = 0;
		onHeap = rhs.onHeap;

        return *this;
    }
    
    /// Add-assign an element.
    Vector<T,N>& operator += (const T& rhs)
    {
        Push(rhs);
        return *this;
    }
    
    /// Add-assign another vector.
    Vector<T,N>& operator += (const Vector<T,N>& rhs)
    {
        Push(rhs);
        return *this;
    }
    
    /// Add an element.
    Vector<T,N> operator + (const T& rhs) const
    {
        Vector<T,N> ret(*this);
        ret.Push(rhs);
        return ret;
    }
    
    /// Add another vector.
    Vector<T,N> operator + (const Vector<T,N>& rhs) const
    {
        Vector<T,N> ret(*this);
        ret.Push(rhs);
        return ret;
    }
    
    /// Test for equality with another vector.
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
    
    /// Test for inequality with another vector.
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
    
    /// Return element at index.
    T& operator [] (unsigned index) { assert(index < size); return Buffer()[index]; }
    /// Return const element at index.
    const T& operator [] (unsigned index) const { assert(index < size); return Buffer()[index]; }
    /// Return element at index.
    T& At(unsigned index) { assert(index < size); return Buffer()[index]; }
    /// Return const element at index.
    const T& At(unsigned index) const { assert(index < size); return Buffer()[index]; }

    /// Add an element at the end.
    void Push(const T& value) { Resize(size + 1, &value); }
    /// Add another vector at the end.
    void Push(const Vector<T,N>& vector) { Resize(size + vector.size, vector.Buffer()); }
    
    /// Remove the last element.
    void Pop()
    {
        if (size)
            Resize(size - 1, 0);
    }
    
    /// Insert an element at position.
    void Insert(unsigned pos, const T& value)
    {
        if (pos > size)
            pos = size;
        
        unsigned oldSize = size;
        Resize(size + 1, 0);
        MoveRange(pos + 1, pos, oldSize - pos);
        Buffer()[pos] = value;
    }
    
    /// Insert another vector at position.
    void Insert(unsigned pos, const Vector<T,N>& vector)
    {
        if (pos > size)
            pos = size;
        
        unsigned oldSize = size;
        Resize(size + vector.size, 0);
        MoveRange(pos + vector.size, pos, oldSize - pos);
        CopyElements(Buffer() + pos, vector.Buffer(), vector.size);
    }
    
    /// Insert an element using an iterator.
    Iterator Insert(const Iterator& dest, const T& value)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        Insert(pos, value);
        
        return begin() + pos;
    }
    
    /// Insert a vector using an iterator.
    Iterator Insert(const Iterator& dest, const Vector<T,N>& vector)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        Insert(pos, vector);
        
        return begin() + pos;
    }
    
    /// Insert a vector partially by iterators.
    Iterator Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        unsigned length = end - start;
        Resize(size + length, 0);
        MoveRange(pos + length, pos, size - pos - length);
        
        T* destPtr = Buffer() + pos;
        for (Iterator it = start; it != end; ++it)
            *destPtr++ = *it;
        
        return begin() + pos;
    }
    
    /// Insert elements.
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
    
    /// Erase a range of elements.
    void Erase(unsigned pos, unsigned length = 1)
    {
        // Return if the range is illegal
        if (pos + length > size || !length)
            return;
        
        MoveRange(pos, pos + length, size - pos - length);
        Resize(size - length, 0);
    }
    
    /// Erase an element by iterator.
    Iterator Erase(const Iterator& it)
    {
        unsigned pos = it - begin();
        if (pos >= size)
            return end();
        Erase(pos);
        
        return begin() + pos;
    }
    
    /// Erase a range by iterators.
    Iterator Erase(const Iterator& start, const Iterator& end)
    {
        unsigned pos = start - begin();
        if (pos >= size)
            return end();
        unsigned length = end - start;
        Erase(pos, length);
        
        return begin() + pos;
    }
    
    /// Erase an element if found.
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
    /// Resize the vector.
    void Resize(unsigned newSize) { Resize(newSize, 0); }
    
    /// Set new capacity.
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
                newBuffer = (onHeap || capacity > N)? reinterpret_cast<T*>(AllocateHeapBuffer(capacity * sizeof(T))) : 
									reinterpret_cast<T*>(AllocateStackBuffer(capacity * sizeof(T)));
                // Move the data into the new buffer
                ConstructElements(newBuffer, Buffer(), size);
            }
            
            // Delete the old buffer
            DestructElements(Buffer(), size);
            if(onHeap)
				Deallocate(buffer);
			onHeap = onHeap || capacity > N;
            buffer = reinterpret_cast<unsigned char*>(newBuffer);
        }
    }
    
    /// Reallocate so that no extra memory is used.
    void Compact() { Reserve(size); }
    
    /// Return iterator to value, or to the end if not found.
    Iterator Find(const T& value)
    {
        Iterator it = begin();
        while (it != end() && *it != value)
            ++it;
        return it;
    }
    
    /// Return const iterator to value, or to the end if not found.
    ConstIterator Find(const T& value) const
    {
        ConstIterator it = begin();
        while (it != end() && *it != value)
            ++it;
        return it;
    }
    
    /// Return whether contains a specific value.
    bool Contains(const T& value) const { return Find(value) != end(); }
    
    /// Return iterator to the beginning.
    Iterator begin() { return Iterator(Buffer()); }
    /// Return const iterator to the beginning.
    ConstIterator begin() const { return ConstIterator(Buffer()); }
    /// Return iterator to the end.
    Iterator end() { return Iterator(Buffer() + size); }
    /// Return const iterator to the end.
    ConstIterator end() const { return ConstIterator(Buffer() + size); }
    /// Return first element.
    T& Front() { assert(size); return Buffer()[0]; }
    /// Return const first element.
    const T& Front() const { assert(size); return Buffer()[0]; }
    /// Return last element.
    T& Back() { assert(size); return Buffer()[size - 1]; }
    /// Return const last element.
    const T& Back() const { assert(size); return Buffer()[size - 1]; }
    /// Return size of vector.
    unsigned Size() const { return size; }
    /// Return capacity of vector.
    unsigned Capacity() const { return capacity; }
    /// Return whether vector is empty.
    bool Empty() const { return size == 0; }
    /// Return the buffer with right type.
    T* Buffer() const { return reinterpret_cast<T*>(buffer); }
    
private:

    bool onHeap;

   /// Resize the vector and create/remove new elements as necessary.
    void Resize(unsigned newSize, const T* src)
    {
        // If size shrinks, destruct the removed elements
        if (newSize < size)
            DestructElements(Buffer() + newSize, size - newSize);
        else
        {
            // Allocate new buffer if necessary and copy the current elements
            if (newSize > capacity)
            {
                if (!capacity)
                    capacity = newSize;
                else
                {
                    while (capacity < newSize)
                        capacity += (capacity + 1) >> 1;
                }
                
				unsigned char* newBuffer = (onHeap || capacity > N)? AllocateHeapBuffer(capacity * sizeof(T)) : AllocateStackBuffer(capacity * sizeof(T));
                if (buffer)
                {
                    ConstructElements(reinterpret_cast<T*>(newBuffer), Buffer(), size);
                    DestructElements(Buffer(), size);
                    if(onHeap)
						Deallocate(buffer);
                }
				onHeap = onHeap || capacity > N;
                buffer = newBuffer;
            }
            
            // Initialize the new elements
            ConstructElements(Buffer() + size, src, newSize - size);
        }
        
        size = newSize;
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
    
    /// Copy elements from one buffer to another.
    static void CopyElements(T* dest, const T* src, unsigned count)
    {
        for (unsigned i = 0; i < count; ++i)
            dest[i] = src[i];
    }
    
    // Call the elements' destructors.
    static void DestructElements(T* dest, unsigned count)
    {
        for (unsigned i = 0; i < count; ++i)
            (dest + i)->~T();
    }
};

/// %Vector template class for POD types. Does not call constructors or destructors and uses block move.
template <class T> class PODVector : public VectorBase
{
public:
    typedef RandomAccessIterator<T> Iterator;
    typedef RandomAccessConstIterator<T> ConstIterator;
    
    /// Construct empty.
    PODVector()
    {
    }
    
    /// Construct with initial size.
    explicit PODVector(unsigned size)
    {
        Resize(size);
    }
    
    /// Construct with initial data.
    PODVector(const T* data, unsigned size)
    {
        Resize(size);
        CopyElements(Buffer(), data, size);
    }
    
    /// Construct from another vector.
    PODVector(const PODVector<T>& vector)
    {
        *this = vector;
    }
    
    /// Destruct.
    ~PODVector()
    {
        Deallocate(buffer);
    }
    
    /// Assign from another vector.
    PODVector<T>& operator = (const PODVector<T>& rhs)
    {
        Resize(rhs.size);
        CopyElements(Buffer(), rhs.Buffer(), rhs.size);
        return *this;
    }
    
    /// Add-assign an element.
    PODVector<T>& operator += (const T& rhs)
    {
        Push(rhs);
        return *this;
    }
    
    /// Add-assign another vector.
    PODVector<T>& operator += (const PODVector<T>& rhs)
    {
        Push(rhs);
        return *this;
    }
    
    /// Add an element.
    PODVector<T> operator + (const T& rhs) const
    {
        PODVector<T> ret(*this);
        ret.Push(rhs);
        return ret;
    }
    
    /// Add another vector.
    PODVector<T> operator + (const PODVector<T>& rhs) const
    {
        PODVector<T> ret(*this);
        ret.Push(rhs);
        return ret;
    }
    
    /// Test for equality with another vector.
    bool operator == (const PODVector<T>& rhs) const
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
    
    /// Test for inequality with another vector.
    bool operator != (const PODVector<T>& rhs) const
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
    
    /// Return element at index.
    T& operator [] (unsigned index) { assert(index < size); return Buffer()[index]; }
    /// Return const element at index.
    const T& operator [] (unsigned index) const { assert(index < size); return Buffer()[index]; }
    /// Return element at index.
    T& At(unsigned index) { assert(index < size); return Buffer()[index]; }
    /// Return const element at index.
    const T& At(unsigned index) const { assert(index < size); return Buffer()[index]; }
    
    /// Add an element at the end.
    void Push(const T& value)
    {
        if (size < capacity)
            ++size;
        else
            Resize(size + 1);
        Back() = value;
    }
    
    /// Add another vector at the end.
    void Push(const PODVector<T>& vector)
    {
        unsigned oldSize = size;
        Resize(size + vector.size);
        CopyElements(Buffer() + oldSize, vector.Buffer(), vector.size);
    }
    
    /// Remove the last element.
    void Pop()
    {
        if (size)
            Resize(size - 1);
    }
    
    /// Insert an element at position.
    void Insert(unsigned pos, const T& value)
    {
        if (pos > size)
            pos = size;
        
        unsigned oldSize = size;
        Resize(size + 1);
        MoveRange(pos + 1, pos, oldSize - pos);
        Buffer()[pos] = value;
    }
    
    /// Insert another vector at position.
    void Insert(unsigned pos, const PODVector<T>& vector)
    {
        if (pos > size)
            pos = size;
        
        unsigned oldSize = size;
        Resize(size + vector.size);
        MoveRange(pos + vector.size, pos, oldSize - pos);
        CopyElements(Buffer() + pos, vector.Buffer(), vector.size);
    }
    
    /// Insert an element using an iterator.
    Iterator Insert(const Iterator& dest, const T& value)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        Insert(pos, value);
        
        return begin() + pos;
    }
    
    /// Insert a vector using an iterator.
    Iterator Insert(const Iterator& dest, const PODVector<T>& vector)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        Insert(pos, vector);
        
        return begin() + pos;
    }
    
    /// Insert a vector partially by iterators.
    Iterator Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        unsigned length = end - start;
        Resize(size + length);
        MoveRange(pos + length, pos, size - pos - length);
        CopyElements(Buffer() + pos, &(*start), length);
        
        return begin() + pos;
    }
    
    /// Insert elements.
    Iterator Insert(const Iterator& dest, const T* start, const T* end)
    {
        unsigned pos = dest - begin();
        if (pos > size)
            pos = size;
        unsigned length = end - start;
        Resize(size + length);
        MoveRange(pos + length, pos, size - pos - length);
        
        T* destPtr = Buffer() + pos;
        for (const T* i = start; i != end; ++i)
            *destPtr++ = *i;
        
        return begin() + pos;
    }
    
    /// Erase a range of elements.
    void Erase(unsigned pos, unsigned length = 1)
    {
        // Return if the range is illegal
        if (!length || pos + length > size)
            return;
        
        MoveRange(pos, pos + length, size - pos - length);
        Resize(size - length);
    }
    
    /// Erase an element using an iterator.
    Iterator Erase(const Iterator& it)
    {
        unsigned pos = it - begin();
        if (pos >= size)
            return end();
        Erase(pos);
        
        return begin() + pos;
    }
    
    /// Erase a range by iterators.
    Iterator Erase(const Iterator& start, const Iterator& end)
    {
        unsigned pos = start - begin();
        if (pos >= size)
            return end();
        unsigned length = end - start;
        Erase(pos, length);
        
        return begin() + pos;
    }
    
    /// Erase an element if found.
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
    
    /// Resize the vector.
    void Resize(unsigned newSize)
    {
        if (newSize > capacity)
        {
            if (!capacity)
                capacity = newSize;
            else
            {
                while (capacity < newSize)
                    capacity += (capacity + 1) >> 1;
            }
            
            unsigned char* newBuffer = AllocateBuffer(capacity * sizeof(T));
            // Move the data into the new buffer and delete the old
            if (buffer)
            {
                CopyElements(reinterpret_cast<T*>(newBuffer), Buffer(), size);
				Deallocate(buffer);
            }
            buffer = newBuffer;
        }
        
        size = newSize;
    }
    
    /// Set new capacity.
    void Reserve(unsigned newCapacity)
    {
        if (newCapacity < size)
            newCapacity = size;
        
        if (newCapacity != capacity)
        {
            unsigned char* newBuffer = 0;
            capacity = newCapacity;
            
            if (capacity)
            {
                newBuffer = AllocateBuffer(capacity * sizeof(T));
                // Move the data into the new buffer
                CopyElements(reinterpret_cast<T*>(newBuffer), Buffer(), size);
            }
            
            // Delete the old buffer
            Deallocate(buffer);
            buffer = newBuffer;
        }
    }
    
    /// Reallocate so that no extra memory is used.
    void Compact() { Reserve(size); }
    
    /// Return iterator to value, or to the end if not found.
    Iterator Find(const T& value)
    {
        Iterator it = begin();
        while (it != end() && *it != value)
            ++it;
        return it;
    }
    
    /// Return const iterator to value, or to the end if not found.
    ConstIterator Find(const T& value) const
    {
        ConstIterator it = begin();
        while (it != end() && *it != value)
            ++it;
        return it;
    }
    
    /// Return whether contains a specific value.
    bool Contains(const T& value) const { return Find(value) != end(); }
    /// Return iterator to the beginning.
    Iterator begin() { return Iterator(Buffer()); }
    /// Return const iterator to the beginning.
    ConstIterator begin() const { return ConstIterator(Buffer()); }
    /// Return iterator to the end.
    Iterator end() { return Iterator(Buffer() + size); }
    /// Return const iterator to the end.
    ConstIterator end() const { return ConstIterator(Buffer() + size); }
    /// Return first element.
    T& Front() { return Buffer()[0]; }
    /// Return const first element.
    const T& Front() const { return Buffer()[0]; }
    /// Return last element.
    T& Back() { assert(size); return Buffer()[size - 1]; }
    /// Return const last element.
    const T& Back() const { assert(size); return Buffer()[size - 1]; }
    /// Return number of elements.
    unsigned Size() const { return size; }
    /// Return capacity of vector.
    unsigned Capacity() const { return capacity; }
    /// Return whether vector is empty.
    bool Empty() const { return size == 0; }
	/// Return the buffer with right type.
    T* Buffer() const { return reinterpret_cast<T*>(buffer); }

private:

    /// Move a range of elements within the vector.
    void MoveRange(unsigned dest, unsigned src, unsigned count)
    {
        if (count)
            memmove(Buffer() + dest, Buffer() + src, count * sizeof(T));
    }
    
    /// Copy elements from one buffer to another.
    static void CopyElements(T* dest, const T* src, unsigned count)
    {
        if (count)
            memcpy(dest, src, count * sizeof(T));
    }
};

NAMESPACE_CORE_END
