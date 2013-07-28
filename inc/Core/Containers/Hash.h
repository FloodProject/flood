/************************************************************************
*
* Flood Project � (2008-201x)
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

namespace hash
{
	/// Returns the number of elements within the data array in the hash map.
	template<typename T> size_t size(const Hash<T>& h);

	/// Returns true if size() returns 0.
	template<typename T> bool empty(const Hash<T>& h);

	/// Returns true if the specified key exists in the hash.
	template<typename T> bool has(const Hash<T> &h, uint64 key);

	/// Returns the value stored for the specified key, or deffault if the key
	/// does not exist in the hash.
	template<typename T> const T& get(const Hash<T> &h, uint64 key, const T &deffault);

	/// Sets the value for the key.
	template<typename T> void set(Hash<T> &h, uint64 key, const T &value);

	/// Removes the key from the hash if it exists.
	template<typename T> void remove(Hash<T> &h, uint64 key);

	/// Resizes the hash lookup table to the specified size.
	/// (The table will grow automatically when 70 % full.)
	template<typename T> void reserve(Hash<T> &h, size_t size);

	/// Clears the arrays within the hash table.
	template<typename T> void clear(Hash<T>& h);

	/// Returns a pointer to the first entry in the hash table, can be used to
	/// efficiently iterate over the elements (in random order).
	template<typename T> const typename Hash<T>::Entry *begin(const Hash<T> &h);
	template<typename T> const typename Hash<T>::Entry *end(const Hash<T> &h);
}

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

namespace hash_internal
{
	const size_t END_OF_LIST = SIZE_MAX;
		
	struct FindResult
	{
		size_t hash_i;
		size_t data_prev;
		size_t data_i;
	};	

	template<typename T> size_t add_entry(Hash<T> &h, uint64 key)
	{
		typename Hash<T>::Entry e;
		e.key = key;
		e.next = END_OF_LIST;
		size_t ei = h._data.size();
		h._data.push_back(e);
		return ei;
	}

	template<typename T> void erase(Hash<T> &h, const FindResult &fr)
	{
		if (fr.data_prev == END_OF_LIST)
			h._hash[fr.hash_i] = h._data[fr.data_i].next;
		else
			h._data[fr.data_prev].next = h._data[fr.data_i].next;

		if (fr.data_i == h._data.size() - 1)
		{
			h._data.pop_back();
			return;
		}

		h._data[fr.data_i] = h._data[h._data.size() - 1];
		FindResult last = find(h, h._data[fr.data_i].key);

		if (last.data_prev != END_OF_LIST)
			h._data[last.data_prev].next = fr.data_i;
		else
			h._hash[last.hash_i] = fr.data_i;
	}

	template<typename T> FindResult find(const Hash<T> &h, uint64 key)
	{
		FindResult fr;
		fr.hash_i = END_OF_LIST;
		fr.data_prev = END_OF_LIST;
		fr.data_i = END_OF_LIST;

		if (h._hash.size() == 0)
			return fr;

		fr.hash_i = key % h._hash.size();
		fr.data_i = h._hash[fr.hash_i];
		while (fr.data_i != END_OF_LIST)
		{
			if (h._data[fr.data_i].key == key)
				return fr;
			fr.data_prev = fr.data_i;
			fr.data_i = h._data[fr.data_i].next;
		}
		return fr;
	}

	template<typename T> FindResult find(const Hash<T> &h, const typename Hash<T>::Entry *e)
	{
		FindResult fr;
		fr.hash_i = END_OF_LIST;
		fr.data_prev = END_OF_LIST;
		fr.data_i = END_OF_LIST;

		if (h._hash.size() == 0)
			return fr;

		fr.hash_i = e->key % h._hash.size();
		fr.data_i = h._hash[fr.hash_i];
		while (fr.data_i != END_OF_LIST)
		{
			if (&h._data[fr.data_i] == e)
				return fr;
			fr.data_prev = fr.data_i;
			fr.data_i = h._data[fr.data_i].next;
		}
		return fr;
	}

	template<typename T> size_t find_or_fail(const Hash<T> &h, uint64 key)
	{
		return find(h, key).data_i;
	}

	template<typename T> size_t find_or_make(Hash<T> &h, uint64 key)
	{
		const FindResult fr = find(h, key);
		if (fr.data_i != END_OF_LIST)
			return fr.data_i;

		size_t i = add_entry(h, key);
		if (fr.data_prev == END_OF_LIST)
			h._hash[fr.hash_i] = i;
		else
			h._data[fr.data_prev].next = i;
		return i;
	}

	template<typename T> size_t make(Hash<T> &h, uint64 key)
	{
		const FindResult fr = find(h, key);
		const size_t i = add_entry(h, key);

		if (fr.data_prev == END_OF_LIST)
			h._hash[fr.hash_i] = i;
		else
			h._data[fr.data_prev].next = i;

		h._data[i].next = fr.data_i;
		return i;
	}	

	template<typename T> void find_and_erase(Hash<T> &h, uint64 key)
	{
		const FindResult fr = find(h, key);
		if (fr.data_i != END_OF_LIST)
			erase(h, fr);
	}

	template<typename T> void rehash(Hash<T> &h, size_t new_size)
	{
		Hash<T> nh(*h._hash.allocator());
		nh._hash.resize(new_size);
		nh._data.reserve(h._data.size());
		for (size_t i=0; i<new_size; ++i)
			nh._hash[i] = END_OF_LIST;
		for (size_t i=0; i<h._data.size(); ++i)
		{
			const typename Hash<T>::Entry &e = h._data[i];
			multi_hash::insert(nh, e.key, e.value);
		}

		Hash<T> empty(*h._hash.allocator());
		h.~Hash<T>();
		memcpy(&h, &nh, sizeof(Hash<T>));
		memcpy(&nh, &empty, sizeof(Hash<T>));
	}

	template<typename T> bool full(const Hash<T> &h)
	{
		const float max_load_factor = 0.7f;
		return h._data.size() >= h._hash.size() * max_load_factor;
	}

	template<typename T> void grow(Hash<T> &h)
	{
		const size_t new_size = h._data.size() * 2 + 10;
		rehash(h, new_size);
	}
}

namespace hash
{
	template<typename T> size_t size(const Hash<T>& h)
	{
		return h._data.size();
	}

	template<typename T> bool empty(const Hash<T>& h)
	{
		return size(h) == 0;
	}

	template<typename T> bool has(const Hash<T> &h, uint64 key)
	{
		return hash_internal::find_or_fail(h, key) != hash_internal::END_OF_LIST;
	}

	template<typename T> const T &get(const Hash<T> &h, uint64 key, const T &deffault)
	{
		const size_t i = hash_internal::find_or_fail(h, key);
		return i == hash_internal::END_OF_LIST ? deffault : h._data[i].value;
	}

	template<typename T> void set(Hash<T> &h, uint64 key, const T &value)
	{
		if (h._hash.size() == 0)
			hash_internal::grow(h);

		const size_t i = hash_internal::find_or_make(h, key);
		h._data[i].value = value;
		if (hash_internal::full(h))
			hash_internal::grow(h);
	}

	template<typename T> void remove(Hash<T> &h, uint64 key)
	{
		hash_internal::find_and_erase(h, key);
	}

	template<typename T> void reserve(Hash<T> &h, size_t size)
	{
		hash_internal::rehash(h, size);
	}

	template<typename T> void clear(Hash<T>& h)
	{
		h._hash.clear();
		h._data.clear();
	}

	template<typename T> const typename Hash<T>::Entry *begin(const Hash<T> &h)
	{
		return h._data.begin();
	}

	template<typename T> const typename Hash<T>::Entry *end(const Hash<T> &h)
	{
		return h._data.end();
	}
}

namespace multi_hash
{
	template<typename T> const typename Hash<T>::Entry *find_first(const Hash<T> &h, uint64 key)
	{
		const size_t i = hash_internal::find_or_fail(h, key);
		return i == hash_internal::END_OF_LIST ? 0 : &h._data[i];
	}

	template<typename T> const typename Hash<T>::Entry *find_next(const Hash<T> &h, const typename Hash<T>::Entry *e)
	{
		size_t i = e->next;
		while (i != hash_internal::END_OF_LIST)
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
			hash_internal::grow(h);

		const size_t i = hash_internal::make(h, key);
		h._data[i].value = value;
		if (hash_internal::full(h))
			hash_internal::grow(h);
	}

	template<typename T> void remove(Hash<T> &h, const typename Hash<T>::Entry *e)
	{
		const hash_internal::FindResult fr = hash_internal::find(h, e);
		if (fr.data_i != hash_internal::END_OF_LIST)
			hash_internal::erase(h, fr);
	}

	template<typename T> void remove_all(Hash<T> &h, uint64 key)
	{
		while (hash::has(h, key))
			hash::remove(h, key);
	}
}

//////////////////////////////////////////////////////////////////////////

template <typename T> Hash<T>::Hash(Allocator &a)
	: _hash(a)
	, _data(a)
{}

template <typename T>
typename Hash<T>::Entry const * Hash<T>::begin() const
{
	return hash::begin(*this);
}

template<typename T>
typename Hash<T>::Entry const * Hash<T>::end() const
{
	return hash::end(*this);
}

template <typename T>
size_t Hash<T>::size() const
{
	return hash::size(*this);
}

template <typename T>
bool Hash<T>::empty() const
{
	return hash::empty(*this);
}

template <typename T>
bool Hash<T>::has(uint64 key) const
{
	return hash::has(*this, key);
}

template <typename T>
T const & Hash<T>::get(uint64 key, T const & default_value) const
{
	return hash::get(*this, key, default_value);
}

template <typename T>
void Hash<T>::set(uint64 key, T const & value)
{
	hash::set(*this, key, value);
}

template <typename T>
void Hash<T>::remove(uint64 key)
{
	hash::remove(*this, key);
}

template <typename T>
void Hash<T>::reserve(size_t size)
{
	hash::reserve(*this, size);
}

template <typename T>
void Hash<T>::clear()
{
	hash::clear(*this);
}

NAMESPACE_CORE_END
