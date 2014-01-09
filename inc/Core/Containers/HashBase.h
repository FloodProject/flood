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

    HashNodeBase* down; //!< Next node in the bucket.
    HashNodeBase* prev; //!< Previous node.
    HashNodeBase* next; //!< Next node.
};

/// Hash set/map iterator base class.
struct HashIteratorBase
{
    /// Construct.
    HashIteratorBase() :
        ptr(nullptr)
    {
    }

    /**
     * Construct with a node pointer.
     * @param ptr pointer to node to construct from
     */
    explicit HashIteratorBase(HashNodeBase* ptr) :
        ptr(ptr)
    {
    }

    /**
     * Test for equality with another iterator.
     * @param rhs iterator to compare with
     * @return whether iterators are equal
     */
    bool operator == (const HashIteratorBase& rhs) const { return ptr == rhs.ptr; }

    /**
     * Test for inequality with another iterator.
     * @param rhs iterator to compare with
     * @return whether iterators are different
     */
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

    HashNodeBase* ptr; //!< Node pointer.
};

/// Hash set/map base class.
class API_CORE HashBase
{
    template <class T, class U> 
    friend class HashMap;

public:
    static const unsigned MIN_BUCKETS = 8; //!< Initial amount of buckets.
    static const unsigned MAX_LOAD_FACTOR = 4; //!< Maximum load factor.
    
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

    /**
     * Swap with another hash set or map.
     * @param rhs hash set or map to swap with
     */
    void Swap(HashBase& rhs)
    {
        ::Swap(head, rhs.head);
        ::Swap(tail, rhs.tail);
        ::Swap(ptrs, rhs.ptrs);
        ::Swap(allocator, rhs.allocator);
    }

    /**
     * Get number of elements.
     * @return number of elements
     */
    unsigned Size() const { return ptrs ? (reinterpret_cast<unsigned*>(ptrs))[0] : 0; }

    /**
     * Get number of buckets.
     * @return number of buckets
     */
    unsigned NumBuckets() const { return ptrs ? (reinterpret_cast<unsigned*>(ptrs))[1] : 0; }

    /**
     * Check if container is empty.
     * @return whether container has no elements
     */
    bool Empty() const { return Size() == 0; }
    
protected:

    /**
     * Allocate bucket head pointers + room for size and bucket count variables.
     * @param size size to allocate
     * @param numBuckets number of buckets
     */
    void AllocateBuckets(unsigned size, unsigned numBuckets);

    /// Reset bucket head pointers.
    void ResetPtrs();

    /**
     * Set new size.
     * @param size new size
     */
    void SetSize(unsigned size) { if (ptrs) (reinterpret_cast<unsigned*>(ptrs))[0] = size; }

    /**
     *  Get bucket head pointers.
     *  @return bucket head pointers
     */
    HashNodeBase** Ptrs() const { return ptrs ? ptrs + 2 : 0; }
    
    HashNodeBase* head; //!< List head node pointer.
    HashNodeBase* tail; //!< List tail node pointer.
    HashNodeBase** ptrs; //!< Bucket head pointers.
    Allocator* allocator; //!< Node allocator.
};

NAMESPACE_CORE_END
