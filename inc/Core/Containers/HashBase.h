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
#include "Core/Containers/Hash.h"
#include "Core/Containers/Swap.h"

NAMESPACE_CORE_BEGIN

/// Hash set/map node base class.
struct HashNodeBase
{
    /// Construct.
    HashNodeBase() :
        down(nullptr),
        prev(nullptr),
        next(nullptr)
    {
    }
    
    /// Next node in the bucket.
    HashNodeBase* down;
    /// Previous node.
    HashNodeBase* prev;
    /// Next node.
    HashNodeBase* next;
};

/// Hash set/map iterator base class.
struct HashIteratorBase
{
    /// Construct.
    HashIteratorBase() :
        ptr(nullptr)
    {
    }
    
    /// Construct with a node pointer.
    explicit HashIteratorBase(HashNodeBase* ptr) :
        ptr(ptr)
    {
    }
    
    /// Test for equality with another iterator.
    bool operator == (const HashIteratorBase& rhs) const { return ptr == rhs.ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const HashIteratorBase& rhs) const { return ptr != rhs.ptr; }
    
    /// Go to the next node.
    void GotoNext()
    {
        if (ptr)
            ptr = ptr->next;
    }
    
    /// Go to the previous node.
    void GotoPrev()
    {
        if (ptr)
            ptr = ptr->prev;
    }
    
    /// Node pointer.
    HashNodeBase* ptr;
};

/// Hash set/map base class.
class API_CORE HashBase
{
	template <class T, class U> 
	friend class HashMap;

public:
    /// Initial amount of buckets.
    static const unsigned MIN_BUCKETS = 8;
    /// Maximum load factor.
    static const unsigned MAX_LOAD_FACTOR = 4;
    
    /// Construct.
    HashBase() :
        ptrs(nullptr),
        allocator(nullptr)
    {
    }

    /// Destruct.
    virtual ~HashBase()
    {
        Deallocate(ptrs);
    }
    
    /// Swap with another hash set or map.
    void Swap(HashBase& rhs)
    {
        ::Swap(head, rhs.head);
        ::Swap(tail, rhs.tail);
        ::Swap(ptrs, rhs.ptrs);
        ::Swap(allocator, rhs.allocator);
    }
    
    /// Return number of elements.
    unsigned Size() const { return ptrs ? (reinterpret_cast<unsigned*>(ptrs))[0] : 0; }
    /// Return number of buckets.
    unsigned NumBuckets() const { return ptrs ? (reinterpret_cast<unsigned*>(ptrs))[1] : 0; }
    /// Return whether has no elements.
    bool Empty() const { return Size() == 0; }
    
protected:
    /// Allocate bucket head pointers + room for size and bucket count variables.
    void AllocateBuckets(unsigned size, unsigned numBuckets);
    /// Reset bucket head pointers.
    void ResetPtrs();
    /// Set new size.
    void SetSize(unsigned size) { if (ptrs) (reinterpret_cast<unsigned*>(ptrs))[0] = size; }
    /// Return bucket head pointers.
    HashNodeBase** Ptrs() const { return ptrs ? ptrs + 2 : 0; }
    
    /// List head node pointer.
    HashNodeBase* head;
    /// List tail node pointer.
    HashNodeBase* tail;
    /// Bucket head pointers.
    HashNodeBase** ptrs;
    /// Node allocator.
    Allocator* allocator;
};

NAMESPACE_CORE_END
