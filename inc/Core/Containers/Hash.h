/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Containers/Array.h"

NAMESPACE_CORE_BEGIN

/// The hash function stores its data in a "list-in-an-array" where
/// indices are used instead of pointers. 
///
/// When items are removed, the array-list is repacked to always keep
/// it tightly ordered.

namespace multi_hash
{
    /// Finds the first entry with the specified key.
    template<typename T> const typename Hash<T>::Entry *find_first(const Hash<T> &h, uint64 key);

    /// Finds the next entry with the same key as e.
    template<typename T> const typename Hash<T>::Entry *find_next(const Hash<T> &h, const typename Hash<T>::Entry *e);

    /// Returns the number of entries with the key.
    template<typename T> size_t count(const Hash<T> &h, uint64 key);

    /// Returns all the entries with the specified key.
    /// Use a TempAllocator for the array to avoid allocating memory.
    template<typename T> void get(const Hash<T> &h, uint64 key, Array<T> &items);

    /// Inserts the value as an additional value for the key.
    template<typename T> void insert(Hash<T> &h, uint64 key, const T &value);

    /// Removes the specified entry.
    template<typename T> void remove(Hash<T> &h, const typename Hash<T>::Entry *e);

    /// Removes all entries with the specified key.
    template<typename T> void remove_all(Hash<T> &h, uint64 key);
}

template <typename T>
const size_t Hash<T>::END_OF_LIST = SIZE_MAX;

template <typename T>
const float Hash<T>::MAX_LOAD_FACTOR = 0.7f;

template <typename T>
Hash<T>::Hash(Allocator &a)
    : _hash(a)
    , _data(a)
{}

template <typename T>
typename Hash<T>::Entry const * Hash<T>::begin() const
{
    return _data,begin();
}

template<typename T>
typename Hash<T>::Entry const * Hash<T>::end() const
{
    return _data.end();
}

template <typename T>
size_t Hash<T>::size() const
{
    return _data.size();
}

template <typename T>
bool Hash<T>::empty() const
{
    return size() == 0;
}

template <typename T>
void Hash<T>::reserve(size_t size)
{
    rehash(size);
}

template <typename T>
void Hash<T>::clear()
{
    _hash.clear();
    _data.clear();
}

template <typename T>
T const & Hash<T>::get(uint64 key, T const & default_value) const
{
    const size_t i = find_or_fail(key);
    return i == END_OF_LIST ? default_value : _data[i].value;
}

template <typename T>
void Hash<T>::set(uint64 key, T const & value)
{
    if (_hash.size() == 0)
        grow();

    const size_t i = find_or_make(key);
    _data[i].value = value;
    if (full())
        grow();
}

template <typename T>
bool Hash<T>::has(uint64 key) const
{
    return find_or_fail(key) != END_OF_LIST;
}

template <typename T>
void Hash<T>::remove(uint64 key)
{
    find_and_erase(key);
}

template<typename T>
bool Hash<T>::full() const
{
    return _data.size() >= _hash.size() * MAX_LOAD_FACTOR;
}

template<typename T>
void Hash<T>::rehash(size_t new_size)
{
    Hash<T> nh(*_hash.allocator());
    nh._hash.resize(new_size);
    nh._data.reserve(_data.size());
    for (size_t i = 0; i < new_size; ++i)
        nh._hash[i] = END_OF_LIST;
    for (size_t i = 0; i < _data.size(); ++i)
    {
        const typename Hash<T>::Entry &e = _data[i];
        multi_hash::insert(nh, e.key, e.value);
    }

    Hash<T> empty(*_hash.allocator());
    clear();
    memcpy(this, &nh, sizeof(Hash<T>));
    memcpy(&nh, &empty, sizeof(Hash<T>));
}

template<typename T>
size_t Hash<T>::add_entry(uint64 key)
{
    typename Hash<T>::Entry e;
    e.key = key;
    e.next = END_OF_LIST;
    size_t ei = _data.size();
    _data.push_back(e);
    return ei;
}

template<typename T>
size_t Hash<T>::make(uint64 key)
{
    const FindResult fr = find(key);
    const size_t i = add_entry(key);

    if (fr.data_prev == END_OF_LIST)
        _hash[fr.hash_i] = i;
    else
        _data[fr.data_prev].next = i;

    _data[i].next = fr.data_i;
    return i;
}

template<typename T>
void Hash<T>::erase(const FindResult &fr)
{
    if (fr.data_prev == END_OF_LIST)
        _hash[fr.hash_i] = _data[fr.data_i].next;
    else
        _data[fr.data_prev].next = _data[fr.data_i].next;

    if (fr.data_i == _data.size() - 1)
    {
        _data.pop_back();
        return;
    }

    _data[fr.data_i] = _data[_data.size() - 1];
    FindResult last = find(_data[fr.data_i].key);

    if (last.data_prev != END_OF_LIST)
        _data[last.data_prev].next = fr.data_i;
    else
        _hash[last.hash_i] = fr.data_i;
}

template<typename T>
void Hash<T>::grow()
{
    const size_t new_size = _data.size() * 2 + 10;
    rehash(new_size);
}

template<typename T>
typename Hash<T>::FindResult Hash<T>::find(uint64 key) const
{
    FindResult fr;
    fr.hash_i = END_OF_LIST;
    fr.data_prev = END_OF_LIST;
    fr.data_i = END_OF_LIST;

    if (_hash.size() == 0)
        return fr;

    fr.hash_i = key % _hash.size();
    fr.data_i = _hash[fr.hash_i];
    while (fr.data_i != END_OF_LIST)
    {
        if (_data[fr.data_i].key == key)
            return fr;
        fr.data_prev = fr.data_i;
        fr.data_i = _data[fr.data_i].next;
    }
    return fr;
}

template<typename T>
typename Hash<T>::FindResult Hash<T>::find(const typename Hash<T>::Entry *e) const
{
    FindResult fr;
    fr.hash_i = END_OF_LIST;
    fr.data_prev = END_OF_LIST;
    fr.data_i = END_OF_LIST;

    if (_hash.size() == 0)
        return fr;

    fr.hash_i = e->key % _hash.size();
    fr.data_i = _hash[fr.hash_i];
    while (fr.data_i != END_OF_LIST)
    {
        if (&_data[fr.data_i] == e)
            return fr;
        fr.data_prev = fr.data_i;
        fr.data_i = _data[fr.data_i].next;
    }
    return fr;
}

template<typename T>
size_t Hash<T>::find_or_fail(uint64 key) const
{
    return find(key).data_i;
}

template<typename T>
size_t Hash<T>::find_or_make(uint64 key)
{
    const FindResult fr = find(key);
    if (fr.data_i != END_OF_LIST)
        return fr.data_i;

    size_t i = add_entry(key);
    if (fr.data_prev == END_OF_LIST)
        _hash[fr.hash_i] = i;
    else
        _data[fr.data_prev].next = i;
    return i;
}

template <typename T>
void Hash<T>::find_and_erase(uint64 key)
{
    const FindResult fr = find(key);
    if (fr.data_i != END_OF_LIST)
        erase(fr);
}

namespace multi_hash
{
    template<typename T> const typename Hash<T>::Entry *find_first(const Hash<T> &h, uint64 key)
    {
        const size_t i = h.find_or_fail(key);
        return i == Hash<T>::END_OF_LIST ? 0 : &h._data[i];
    }

    template<typename T> const typename Hash<T>::Entry *find_next(const Hash<T> &h, const typename Hash<T>::Entry *e)
    {
        size_t i = e->next;
        while (i != Hash<T>::END_OF_LIST)
        {
            if (h._data[i].key == e->key)
                return &h._data[i];
            i = h._data[i].next;
        }
        return 0;
    }

    template<typename T> size_t count(const Hash<T> &h, uint64 key)
    {
        size_t i = 0;
        const typename Hash<T>::Entry *e = find_first(h, key);
        while (e)
        {
            ++i;
            e = find_next(h, e);
        }
        return i;
    }

    template<typename T> void get(const Hash<T> &h, uint64 key, Array<T> &items)
    {
        const typename Hash<T>::Entry *e = find_first(h, key);
        while (e)
        {
            items.push_back(e->value);
            e = find_next(h, e);
        }
    }

    template<typename T> void insert(Hash<T> &h, uint64 key, const T &value)
    {
        if (h._hash.size() == 0)
            h.grow();

        const size_t i = h.make(key);
        h._data[i].value = value;
        if (h.full())
            h.grow();
    }

    template<typename T> void remove(Hash<T> &h, const typename Hash<T>::Entry *e)
    {
        const typename Hash<T>::FindResult fr = h.find(e);
        if (fr.data_i != Hash<T>::END_OF_LIST)
            h.erase(fr);
    }

    template<typename T> void remove_all(Hash<T> &h, uint64 key)
    {
        while (h.has(key))
            h.remove(key);
    }
}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_CORE_END
