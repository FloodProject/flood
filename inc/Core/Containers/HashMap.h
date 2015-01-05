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
#include "Core/Containers/HashBase.h"
#include "Core/Containers/Pair.h"
#include "Core/Containers/Sort.h"
#include "Core/Containers/Vector.h"

#include <cassert>

NAMESPACE_CORE_BEGIN

/// Hash map template class.
template <class T, class U, class HashFunctions = Hash>
class HashMap : public HashBase
{
public:
    /// Hash map key-value pair with const key.
    class KeyValue
    {
    public:
        /// Construct with default key.
        KeyValue() 
            : first(T())
        {
        }

        /**
         * Construct with key and value.
         * @param first key
         * @param second value
         */
        KeyValue(const T& first, const U& second) 
            : first(first)
            , second(second)
        {
        }

        /**
         * Test for equality with another pair.
         * @param rhs pair to compare with
         * @return whether pairs are equal
         */
        bool operator == (const KeyValue& rhs) const { return first == rhs.first && second == rhs.second; }

        /**
         * Test for inequality with another pair.
         * @param rhs pair to compare with
         * @return whether pairs are different
         */
        bool operator != (const KeyValue& rhs) const { return first != rhs.first || second != rhs.second; }

        const T first; //!< Key.
        U second; //!< Value.
    };
    
    /// Hash map node.
    struct Node : public HashNodeBase
    {
        /// Construct undefined.
        Node()
        {
        }
        
        /**
         * Construct with key and value.
         * @param key pair key
         * @param value pair value
         */
        Node(const T& key, const U& value) :
            pair(key, value)
        {
        }

        KeyValue pair; //!< Key-value pair.

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
    
    /// Hash map node iterator.
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
         * Pointer to the pair.
         * @return pair pointer
         */
        KeyValue* operator -> () const { return &(static_cast<Node*>(ptr))->pair; }

        /**
         * Dereference the pair.
         * @return pair
         */
        KeyValue& operator * () const { return (static_cast<Node*>(ptr))->pair; }
    };
    
    /// Hash map node const iterator.
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
         * Pointer to the pair.
         * @return pair pointer
         */
        const KeyValue* operator -> () const { return &(static_cast<Node*>(ptr))->pair; }

        /**
         * Dereference the pair.
         * @return pair
         */
        const KeyValue& operator * () const { return (static_cast<Node*>(ptr))->pair; }
    };

    /// Construct empty.
    HashMap()
    {
        /// Reserve the tail node
        allocator = AllocatorGetHeap();
        head = tail = ReserveNode();
    }

    /**
     * Construct from another hash map.
     * @param map hash map to construct from
     */
    HashMap(const HashMap<T, U>& map)
    {
        /// Reserve the tail node
        allocator = AllocatorGetHeap();
        head = tail = ReserveNode();
        *this = map;
    }

    /**
     * Move construct from another hash map.
     * @param map hash map to move construct from
     */
    HashMap(HashMap<T, U>&& map)
    {
        allocator = map.allocator;
        ptrs = map.ptrs;
        head = map.head;
        tail = map.tail;

        map.allocator = nullptr;
        map.ptrs = nullptr;
        map.head = nullptr;
        map.tail = nullptr;
    }

    /// Destruct.
    ~HashMap()
    {
        Clear();
        FreeNode(Tail());
    }

    /**
     * Assign a hash map.
     * @param rhs hash map to assign from
     * @return resulting hash map
     */
    HashMap& operator = (const HashMap<T, U>& rhs)
    {
        Clear();
        Insert(rhs);
        return *this;
    }

    /**
     * Move assign a hash map.
     * @param rhs hash map to move assign from
     * @return resulting hash map
     */
    HashMap& operator = (HashMap<T, U>&& rhs)
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
     * Add-assign a pair.
     * @param rhs pair to add assign
     * @return resulting hash map
     */
    HashMap& operator += (const Pair<T, U>& rhs)
    {
        Insert(rhs);
        return *this;
    }

    /**
     * Add-assign a hash map.
     * @param rhs hash map to add assign
     * @return resulting hash map
     */
    HashMap& operator += (const HashMap<T, U>& rhs)
    {
        Insert(rhs);
        return *this;
    }

    /**
     * Test for equality with another hash map.
     * @param rhs hash map to compare with
     * @return whether maps are equal
     */
    bool operator == (const HashMap<T, U>& rhs) const
    {
        if (rhs.Size() != Size())
            return false;
        
        ConstIterator i = Begin();
        while (i != End())
        {
            ConstIterator j = rhs.Find(i->first);
            if (j == rhs.End() || j->second != i->second)
                return false;
            ++i;
        }
        
        return true;
    }

    /**
     * Test for inequality with another hash map.
     * @param rhs hash map to compare with
     * @return whether maps are different
     */
    bool operator != (const HashMap<T, U>& rhs) const
    {
        if (rhs.Size() != Size())
            return true;
        
        ConstIterator i = Begin();
        while (i != End())
        {
            ConstIterator j = rhs.Find(i->first);
            if (j == rhs.End() || j->second != i->second)
                return true;
            ++i;
        }
        
        return false;
    }

    /**
     * Index the map, create a new pair if key not found.
     * @param key map key
     * @return value corresponding to key
     */
    U& operator [] (const T& key)
    {
        if (!ptrs)
            return InsertNode(key, U(), false)->pair.second;
        
        unsigned hashKey = Hash(key);
        
        Node* node = FindNode(key, hashKey);
        if (node)
            return node->pair.second;
        else
            return InsertNode(key, U(), false)->pair.second;
    }

    /**
     * Insert a pair.
     * @param pair pair to insert
     * @return an iterator to it
     */
    Iterator Insert(const Pair<T, U>& pair)
    {
        return Iterator(InsertNode(pair.first, pair.second));
    }

    /**
     * Insert a map.
     * @param map hash map to insert
     */
    void Insert(const HashMap<T, U>& map)
    {
        ConstIterator it = map.Begin();
        ConstIterator end = map.End();
        while (it != end)
        {
            InsertNode(it->first, it->second);
            ++it;
        }
    }

    /**
     * Insert a pair by iterator.
     * @param it pair iterator
     * @return an iterator to the pair
     */
    Iterator Insert(const ConstIterator& it) { return Iterator(InsertNode(it->first, it->second)); }

    /**
     * Insert a range by iterators.
     * @param start iterator to range start
     * @param end iterator to range ending
     */
    void Insert(const ConstIterator& start, const ConstIterator& end)
    {
        ConstIterator it = start;
        while (it != end)
            InsertNode(*it++);
    }

    /**
     * Erase a pair by key.
     * @param key key to pair to erase
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
     * Erase a pair by iterator.
     * @param it pair iterator
     * @return iterator to the next pair
     */
    Iterator Erase(const Iterator& it)
    {
        if (!ptrs || !it.ptr)
            return End();
        
        Node* node = static_cast<Node*>(it.ptr);
        Node* next = node->Next();
        
        unsigned hashKey = Hash(node->pair.first);
        
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

    /// Clear the map.
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

    /// Sort pairs. After sorting the map can be iterated in order until new elements are inserted.
    void Sort()
    {
        Sort(CompareNodes);
    }

    /**
     * Sort pairs.
     * @param CmpFPtr pointer to node comparison function
     * @remark After sorting the map can be iterated in order until new elements are inserted.
     */
    void Sort(bool(*CmpFPtr)(Node*&, Node*&))
    {
        unsigned numKeys = Size();
        if (!numKeys)
            return;
        
        Node** ptrs = static_cast<Node**>(AllocatorAllocate(AllocatorGetHeap(), numKeys*sizeof(Node*), 0));
        Node* ptr = Head();
        
        for (unsigned i = 0; i < numKeys; ++i)
        {
            ptrs[i] = ptr;
            ptr = ptr->Next();
        }
        
        ::Sort(RandomAccessIterator<Node*>(ptrs), RandomAccessIterator<Node*>(ptrs + numKeys), CmpFPtr);
        
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
        
        // Check for being power of two
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
     * Find a pair.
     * @param key key of the pair to find
     * @return iterator to the pair with the key, or end iterator if not found
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
     * Find a pair.
     * @param key key of the pair to find
     * @return const iterator to the pair with the key, or end iterator if not found
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
     * Check if map contains a pair with the specified key.
     * @param key key of the pair to check
     * @return whether map contains the pair
     */
    bool Contains(const T& key) const
    {
        if (!ptrs)
            return false;
        
        unsigned hashKey = Hash(key);
        return FindNode(key, hashKey) != 0;
    }

    /**
     * Get all the map keys.
     * @return all the map keys
     */
    Vector<T> Keys() const
    {
        Vector<T> result;
        result.Reserve(Size());
        for (ConstIterator i = Begin(); i != End(); ++i)
            result.Push(i->first);
        return result;
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
    Iterator begin() { return Iterator(Head()); }

    /**
     * Get first iterator.
     * @return iterator to the beginning
     */
    ConstIterator Begin() const { return ConstIterator(Head()); }

    /**
     * Get first iterator.
     * @return iterator to the beginning
     */
    ConstIterator begin() const { return ConstIterator(Head()); }

    /**
     * Get last iterator.
     * @return iterator to the end
     */
    Iterator End() { return Iterator(Tail()); }

    /**
     * Get last iterator.
     * @return iterator to the end
     */
    Iterator end() { return Iterator(Tail()); }

    /**
     * Get last iterator.
     * @return iterator to the end
     */
    ConstIterator End() const { return ConstIterator(Tail()); }

    /**
     * Get last iterator.
     * @return iterator to the end
     */
    ConstIterator end() const { return ConstIterator(Tail()); }

    /**
     * Get first map key.
     * @return first key
     */
    const T& Front() const { return Begin()->first; }

    /**
     * Get last map key.
     * @return last key
     */
    const T& Back() const { return (--End())->first; }
    
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
            if (HashFunctions::Equals(node->pair.first, key))
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
            if (HashFunctions::Equals(node->pair.first, key))
                return node;
            previous = node;
            node = node->Down();
        }
        
        return 0;
    }

    /// Insert a key and value and return either the new or existing node.
    Node* InsertNode(const T& key, const U& value, bool findExisting = true)
    {
        // If no pointers yet, allocate with minimum bucket count
        if (!ptrs)
        {
            AllocateBuckets(Size(), MIN_BUCKETS);
            Rehash();
        }

        unsigned hashKey = Hash(key);

        if (findExisting)
        {
            /// If exists, just change the value
            Node* existing = FindNode(key, hashKey);
            if (existing)
            {
                existing->pair.second = value;
                return existing;
            }
        }

        Node* newNode = InsertNode(Tail(), key, value);
        /// runs through same bucket values
        newNode->down = Ptrs()[hashKey];
        Ptrs()[hashKey] = newNode;

        /// Rehash if the maximum load factor has been exceeded
        if (Size() > NumBuckets() * MAX_LOAD_FACTOR)
        {
            AllocateBuckets(Size(), NumBuckets() << 1);
            Rehash();
        }

        return newNode;
    }

    /// Insert a node into the list. Return the new node.
    Node* InsertNode(Node* dest, const T& key, const U& value)
    {
        if (!dest)
            return 0;

        Node* newNode = ReserveNode(key, value);
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
        // The tail node can not be removed
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

    /// Reserve a node with specified key and value.
    Node* ReserveNode(const T& key, const U& value)
    {
        assert(allocator);
        return static_cast<Node*>(Allocate(allocator, Node, key, value));
    }

    /// Free a node.
    void FreeNode(Node* node)
    {
        Deallocate(node);
    }

    /// Rehash the buckets.
    void Rehash()
    {
        for (Iterator i = Begin(); i != End(); ++i)
        {
            Node* node = static_cast<Node*>(i.ptr);
            unsigned hashKey = Hash(i->first);
            node->down = Ptrs()[hashKey];
            Ptrs()[hashKey] = node;
        }
    }

    /// Compare two nodes.
    static bool CompareNodes(Node*& lhs, Node*& rhs) { return lhs->pair.first < rhs->pair.first; }

    /// Compute a hash based on the key and the bucket size
    unsigned Hash(const T& key) const { return HashFunctions::MakeHash(key) & (NumBuckets() - 1); }
};

NAMESPACE_CORE_END
