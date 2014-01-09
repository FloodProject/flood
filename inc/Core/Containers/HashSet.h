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

#include "HashBase.h"
#include "Sort.h"

#include <cassert>

/// Hash set template class.
 template <class T> class HashSet : public HashBase
{
public:
    /// Hash set node.
    struct Node : public HashNodeBase
    {
        /// Construct undefined.
        Node()
        {
        }

        /**
         * Construct with key.
         * @param key key value 
         */
        Node(const T& key) :
            key(key)
        {
        }
        
        
        T key; //!< Key.

        /**
         * Return next node.
         * @return pointer to next node
         */
        Node* Next() const { return static_cast<Node*>(next); }

        /**
         * Return previous node.
         * @return pointer to previous node
         */
        Node* Prev() const { return static_cast<Node*>(prev); }

        /**
         * Return next node in the bucket.
         * @return pointer to next node in the bucket
         */
        Node* Down() const { return static_cast<Node*>(down); }
    };

    /// Hash set node iterator.
    struct Iterator : public HashIteratorBase
    {
        /// Construct.
        Iterator()
        {
        }

        /**
         * Construct with a node pointer.
         * @param ptr node pointer
         */
        Iterator(Node* ptr) :
            HashIteratorBase(ptr)
        {
        }

        /**
         * Preincrement the pointer.
         * @return next iterator
         */
        Iterator& operator ++ () { GotoNext(); return *this; }

        /**
         * Postincrement the pointer.
         * @return current iterator
         */
        Iterator operator ++ (int) { Iterator it = *this; GotoNext(); return it; }

        /**
         * Predecrement the pointer.
         * @return previous iterator
         */
        Iterator& operator -- () { GotoPrev(); return *this; }

        /**
         * Postdecrement the pointer.
         * @return current iterator
         */
        Iterator operator -- (int) { Iterator it = *this; GotoPrev(); return it; }

        /**
         * Pointer to the key.
         * @return key pointer
         */
        const T* operator -> () const { return &(static_cast<Node*>(ptr))->key; }

        /**
         * Dereference the key.
         * @return key
         */
        const T& operator * () const { return (static_cast<Node*>(ptr))->key; }
    };

     /// Hash set node const iterator.
    struct ConstIterator : public HashIteratorBase
    {
        /// Construct.
        ConstIterator()
        {
        }

        /**
         * Construct with a node pointer.
         * @param ptr node pointer
         */
         ConstIterator(Node* ptr) :
            HashIteratorBase(ptr)
        {
        }

        /**
         * Construct from a non-const iterator.
         * @param rhs iterator
         */
         ConstIterator(const Iterator& rhs) :
            HashIteratorBase(rhs.ptr)
        {
        }

        /**
         * Assign from a non-const iterator.
         * @param rhs iterator
         * @return current iterator
         */
        ConstIterator& operator = (const Iterator& rhs) { ptr = rhs.ptr; return *this; }

        /**
         * Preincrement the pointer.
         * @return next iterator
         */
        ConstIterator& operator ++ () { GotoNext(); return *this; }

        /**
         * Postincrement the pointer.
         * @return current iterator
         */
        ConstIterator operator ++ (int) { ConstIterator it = *this; GotoNext(); return it; }

        /**
         * Predecrement the pointer.
         * @return previous iterator
         */
        ConstIterator& operator -- () { GotoPrev(); return *this; }

        /**
         * Postdecrement the pointer.
         * @return current iterator
         */
        ConstIterator operator -- (int) { ConstIterator it = *this; GotoPrev(); return it; }

        /**
         * Pointer to the key.
         * @return key pointer
         */
        const T* operator -> () const { return &(static_cast<Node*>(ptr))->key; }

        /**
         * Dereference the key.
         * @return key
         */
        const T& operator * () const { return (static_cast<Node*>(ptr))->key; }
    };

    /// Construct empty.
    HashSet()
    {
        /// Reserve the tail node
        allocator = AllocatorGetHeap();
        head = tail = ReserveNode();
    }

    /**
     * Construct from another hash set.
     * @param set hash set to construct from
     */
    HashSet(const HashSet<T>& set)
    {
        /// Reserve the tail node
        allocator = AllocatorGetHeap();
        head = tail = ReserveNode();
        *this = set;
    }

    /**
     * Move construct from another hash set.
     * @param set hash set to move construct from
     */
    HashSet(HashSet<T>&& set)
    {
        allocator = set.allocator;
        ptrs = set.ptrs;
        head = set.head;
        tail = set.tail;

        set.allocator = nullptr;
        set.ptrs = nullptr;
        set.head = nullptr;
        set.tail = nullptr;
    }

     /// Destruct.
    ~HashSet()
    {
        Clear();
        FreeNode(Tail());
    }

    /**
     * Assign a hash set.
     * @param rhs hash set to assign from
     * @return resulting hash set
     */
    HashSet& operator = (const HashSet<T>& rhs)
    {
        Clear();
        Insert(rhs);
        return *this;
    }

    /**
     * Move assign a hash set.
     * @param rhs hash set to move assign from
     * @return resulting hash set
     */
    HashSet& operator = (HashSet<T>&& rhs)
    {
        Clear();
        allocator = rhs.allocator;
        ptrs = rhs.ptrs;
        head = rhs.head;
        tail = rhs.tail;

        rhs.allocator = nullptr;
        rhs.ptrs = nullptr;
        rhs.head = nullptr;
        rhs.tail = nullptr;
        return *this;
    }

    /**
     * Add-assign a value.
     * @param rhs value to add assign
     * @return resulting hash set
     */
    HashSet& operator += (const T& rhs)
    {
        Insert(rhs);
        return *this;
    }

    /**
     * Add-assign a hash set.
     * @param rhs hash set to add assign
     * @return resulting hash set
     */
    HashSet& operator += (const HashSet<T>& rhs)
    {
        Insert(rhs);
        return *this;
    }

    /**
     * Test for equality with another hash set.
     * @param rhs hash set to compare with
     * @return whether sets are equal
     */
    bool operator == (const HashSet<T>& rhs) const
    {
        if (rhs.Size() != Size())
            return false;
        
        ConstIterator it = Begin();
        while (it != End())
        {
            if (!rhs.Contains(*it))
                return false;
            ++it;
        }
        
        return true;
    }

    /**
     * Test for inequality with another hash set.
     * @param rhs hash set to compare with
     * @return whether sets are different
     */
    bool operator != (const HashSet<T>& rhs) const
    {
        if (rhs.Size() != Size())
            return true;
        
        ConstIterator it = Begin();
        while (it != End())
        {
            if (!rhs.Contains(*it))
                return true;
            ++it;
        }
        
        return false;
    }

    /**
     * Insert a key.
     * @param key key to insert
     * @return an iterator to it
     */
    Iterator Insert(const T& key)
    {
        /// If no pointers yet, allocate with minimum bucket count
        if (!ptrs)
        {
            AllocateBuckets(Size(), MIN_BUCKETS);
            Rehash();
        }
        
        unsigned hashKey = Hash(key);
        
        Node* existing = FindNode(key, hashKey);
        if (existing)
            return Iterator(existing);
        
        Node* newNode = InsertNode(Tail(), key);
        newNode->down = Ptrs()[hashKey];
        Ptrs()[hashKey] = newNode;
        
        /// Rehash if the maximum load factor has been exceeded
        if (Size() > NumBuckets() * MAX_LOAD_FACTOR)
        {
            AllocateBuckets(Size(), NumBuckets() << 1);
            Rehash();
        }
        
        return Iterator(newNode);
    }

    /**
     * Insert a set.
     * @param set hash set to insert
     */
    void Insert(const HashSet<T>& set)
    {
        ConstIterator it = set.Begin();
        ConstIterator end = set.End();
        while (it != end)
            Insert(*it++);
    }

    /**
     * Insert a key by iterator.
     * @param it key iterator
     * @return an iterator to the value
     */
    Iterator Insert(const ConstIterator& it)
    {
        return Iterator(InsertNode(*it));
    }

    /**
     * Erase a key.
     * @param key key to erase
     * @return true if key was found
     */
    bool Erase(const T& key)
    {
        if (!ptrs)
            return false;
        
        unsigned hashKey = Hash(key);
        
        Node* previous;
        Node* node = FindNode(key, hashKey, previous);
        if (!node)
            return false;
        
        if (previous)
            previous->down = node->down;
        else
            Ptrs()[hashKey] = node->down;
        
        EraseNode(node);
        return true;
    }

    /**
     * Erase a key by iterator.
     * @param it key iterator
     * @return iterator to the next key
     */
    Iterator Erase(const Iterator& it)
    {
        if (!ptrs || !it.ptr)
            return End();
        
        Node* node = static_cast<Node*>(it.ptr);
        Node* next = node->Next();
        
        unsigned hashKey = Hash(node->key);
        
        Node* previous = 0;
        Node* current = static_cast<Node*>(Ptrs()[hashKey]);
        while (current && current != node)
        {
            previous = current;
            current = current->Down();
        }
        
        assert(current == node);
        
        if (previous)
            previous->down = node->down;
        else
            Ptrs()[hashKey] = node->down;
        
        EraseNode(node);
        return Iterator(next);
    }

    /// Clear the set.
    void Clear()
    {
        if (Size())
        {
            for (Iterator i = Begin(); i != End(); )
            {
                FreeNode(static_cast<Node*>(i++.ptr));
                i.ptr->prev = 0;
            }
            
            head = tail;
            SetSize(0);
        }
        
        ResetPtrs();
    }

    /// Sort keys. After sorting the set can be iterated in order until new elements are inserted.
    void Sort()
    {
        unsigned numKeys = Size();
        if (!numKeys)
            return;
        
        Node** ptrs = new Node*[numKeys];
        Node* ptr = Head();
        
        for (unsigned i = 0; i < numKeys; ++i)
        {
            ptrs[i] = ptr;
            ptr = ptr->Next();
        }
        
        ::Sort(RandomAccessIterator<Node*>(ptrs), RandomAccessIterator<Node*>(ptrs + numKeys), CompareNodes);
        
        head = ptrs[0];
        ptrs[0]->prev = 0;
        for (unsigned i = 1; i < numKeys; ++i)
        {
            ptrs[i - 1]->next = ptrs[i];
            ptrs[i]->prev = ptrs[i - 1];
        }
        ptrs[numKeys - 1]->next = tail;
        tail->prev = ptrs[numKeys - 1];
        
        Deallocate(ptrs);
    }

    /**
     * Rehash to a specific bucket count, which must be a power of two. 
     * @param numBuckets number of buckets to rehash to
     * @return true if successful
     */
    bool Rehash(unsigned numBuckets)
    {
        if (numBuckets == NumBuckets())
            return true;
        if (!numBuckets || numBuckets < Size() / MAX_LOAD_FACTOR)
            return false;
        
        /// Check for being power of two
        unsigned check = numBuckets;
        while (!(check & 1))
            check >>= 1;
        if (check != 1)
            return false;
        
        AllocateBuckets(Size(), numBuckets);
        Rehash();
        return true;
    }

    /**
     * Find a key.
     * @param key key to find
     * @return iterator to the key, or end iterator if not found
     */
    Iterator Find(const T& key)
    {
        if (!ptrs)
            return End();
        
        unsigned hashKey = Hash(key);
        Node* node = FindNode(key, hashKey);
        if (node)
            return Iterator(node);
        else
            return End();
    }

    /**
     * Find a key.
     * @param key key to find
     * @return const iterator to the key, or end iterator if not found
     */
    ConstIterator Find(const T& key) const
    {
        if (!ptrs)
            return End();
        
        unsigned hashKey = Hash(key);
        Node* node = FindNode(key, hashKey);
        if (node)
            return ConstIterator(node);
        else
            return End();
    }

    /**
     * Check if set contains a key.
     * @param key key to check
     * @return whether set contains the key
     */
    bool Contains(const T& key) const
    {
        if (!ptrs)
            return false;
        
        unsigned hashKey = Hash(key);
        return FindNode(key, hashKey) != 0;
    }

    /**
     * Get first iterator.
     * @return iterator to the beginning
     */
    Iterator Begin() { return Iterator(Head()); }

    /**
     * Get first iterator.
     * @return iterator to the beginning
     */
    ConstIterator Begin() const { return ConstIterator(Head()); }

    /**
     * Get last iterator.
     * @return iterator to the end
     */
    Iterator End() { return Iterator(Tail()); }

    /**
     * Get last iterator.
     * @return iterator to the end
     */
    ConstIterator End() const { return ConstIterator(Tail()); }

    /**
     * Get first set key.
     * @return first key
     */
    const T& Front() const { return *Begin(); }

    /**
     * Get last set key.
     * @return last key
     */
    const T& Back() const { return *(--End()); }
    
private:
    /// Return the head node.
    Node* Head() const { return static_cast<Node*>(head); }
    /// Return the tail node.
    Node* Tail() const { return static_cast<Node*>(tail); }

    /// Find a node from the buckets. Do not call if the buckets have not been allocated.
    Node* FindNode(const T& key, unsigned hashKey) const
    {
        Node* node = static_cast<Node*>(Ptrs()[hashKey]);
        while (node)
        {
            if (node->key == key)
                return node;
            node = node->Down();
        }

        return 0;
    }

    /// Find a node and the previous node from the buckets. Do not call if the buckets have not been allocated.
    Node* FindNode(const T& key, unsigned hashKey, Node*& previous) const
    {
        previous = 0;

        Node* node = static_cast<Node*>(Ptrs()[hashKey]);
        while (node)
        {
            if (node->key == key)
                return node;
            previous = node;
            node = node->Down();
        }

        return 0;
    }

    /// Insert a node into the list. Return the new node.
    Node* InsertNode(Node* dest, const T& key)
    {
        if (!dest)
            return 0;

        Node* newNode = ReserveNode(key);
        Node* prev = dest->Prev();
        newNode->next = dest;
        newNode->prev = prev;
        if (prev)
            prev->next = newNode;
        dest->prev = newNode;

        /// Reassign the head node if necessary
        if (dest == Head())
            head = newNode;

        SetSize(Size() + 1);

        return newNode;
    }

    /// Erase a node from the list. Return pointer to the next element, or to the end if could not erase.
    Node* EraseNode(Node* node)
    {
        /// The tail node can not be removed
        if (!node || node == tail)
            return Tail();

        Node* prev = node->Prev();
        Node* next = node->Next();
        if (prev)
            prev->next = next;
        next->prev = prev;

        /// Reassign the head node if necessary
        if (node == Head())
            head = next;

        FreeNode(node);
        SetSize(Size() - 1);

        return next;
    }
    
    /// Reserve a node.
    Node* ReserveNode()
    {
        assert(allocator);
        return static_cast<Node*>(Allocate(allocator, Node));
    }

    /// Reserve a node with specified key.
    Node* ReserveNode(const T& key)
    {
        assert(allocator);
        return static_cast<Node*>(Allocate(allocator, Node, key));
    }

    /// Free a node.
    void FreeNode(Node* node)
    {
        Deallocate(node);
    }

    /// Rehash the buckets.
    void Rehash()
    {
        for (Iterator it = Begin(); it != End(); ++it)
        {
            Node* node = static_cast<Node*>(it.ptr);
            unsigned hashKey = Hash(*it);
            node->down = Ptrs()[hashKey];
            Ptrs()[hashKey] = node;
        }
    }

    /// Compare two nodes.
    static bool CompareNodes(Node*& lhs, Node*& rhs) { return lhs->key < rhs->key; }

    /// Compute a hash based on the key and the bucket size
    unsigned Hash(const T& key) const { return MakeHash(key) & (NumBuckets() - 1); }
};

