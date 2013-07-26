/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Types/Containers.h"

#include <memory>

NAMESPACE_CORE_BEGIN

namespace array
{
    /// The number of elements in the array.
    template<typename T> size_t size(const Array<T>& a) ;
    /// Returns true if there are any elements in the array.
    template<typename T> bool any(const Array<T>& a);
    /// Returns true if the array is empty.
    template<typename T> bool empty(const Array<T>& a);
        
    /// Used to iterate over the array.
    template<typename T> T* begin(Array<T>& a);
    template<typename T> const T* begin(const Array<T>& a);
    template<typename T> T* end(Array<T>& a);
    template<typename T> const T* end(const Array<T>& a);
        
    /// Returns the first/last element of the array. Don't use these on an
    /// empty array.
    template<typename T> T& front(Array<T>& a);
    template<typename T> const T& front(const Array<T>& a);
    template<typename T> T& back(Array<T>& a);
    template<typename T> const T& back(const Array<T>& a);

    /// Changes the size of the array (does not reallocate memory unless necessary).
    template <typename T> void resize(Array<T>& a, size_t new_size);
    /// Removes all items in the array (does not free memory).
    template <typename T> void clear(Array<T>& a);
    /// Reallocates the array to the specified capacity.
    template<typename T> void set_capacity(Array<T>& a, size_t new_capacity);
    /// Makes sure that the array has at least the specified capacity.
    /// (If not, the array is grown.)
    template <typename T> void reserve(Array<T>& a, size_t new_capacity);
    /// Grows the array using a geometric progression formula, so that the amortized
    /// cost of push_back() is O(1). If a min_capacity is specified, the array will
    /// grow to at least that capacity.
    template<typename T> void grow(Array<T>& a, size_t min_capacity = 0);
    /// Trims the array so that its capacity matches its size.
    template <typename T> void trim(Array<T>& a);

    /// Pushes the item to the end of the array.
    template<typename T> void push_back(Array<T>& a, const T& item);
    /// Pops the last item from the array. The array cannot be empty.
    template<typename T> void pop_back(Array<T>& a);

    /// Removes a given item from an array. The item does not have to be in the array.
    template<typename T> void remove(Array<T>& a, const T* item);
}

namespace array
{
    template<typename T> inline size_t size(const Array<T> &a) 		{return a._size;}
    template<typename T> inline bool any(const Array<T> &a) 			{return a._size != 0;}
    template<typename T> inline bool empty(const Array<T> &a) 			{return a._size == 0;}
        
    template<typename T> inline T* begin(Array<T> &a) 					{return a._data;}
    template<typename T> inline const T* begin(const Array<T> &a) 		{return a._data;}
    template<typename T> inline T* end(Array<T> &a) 					{return a._data + a._size;}
    template<typename T> inline const T* end(const Array<T> &a) 		{return a._data + a._size;}
        
    template<typename T> inline T& front(Array<T> &a) 					{return a._data[0];}
    template<typename T> inline const T& front(const Array<T> &a) 		{return a._data[0];}
    template<typename T> inline T& back(Array<T> &a) 					{return a._data[a._size-1];}
    template<typename T> inline const T& back(const Array<T> &a) 		{return a._data[a._size-1];}

    template <typename T> inline void clear(Array<T> &a) {resize(a,0);}
    template <typename T> inline void trim(Array<T> &a) {set_capacity(a,a._size);}

    template <typename T> void resize(Array<T> &a, size_t new_size)
    {
        if (new_size > a._capacity)
            grow(a, new_size);
        a._size = new_size;
    }

    template <typename T> inline void reserve(Array<T> &a, size_t new_capacity)
    {
        if (new_capacity > a._capacity)
            set_capacity(a, new_capacity);
    }

    template<typename T> void set_capacity(Array<T> &a, size_t new_capacity)
    {
        if (new_capacity == a._capacity)
            return;

        if (new_capacity < a._size)
            resize(a, new_capacity);

        T *new_data = 0;
        if (new_capacity > 0) {
            new_data = (T*) AllocatorAllocate(a._allocator, sizeof(T) * new_capacity, alignof(T));
#ifdef _DEBUG
            memset(new_data, 0, sizeof(T));
#endif
            memcpy(new_data, a._data, sizeof(T)*a._size);
        }
            
        AllocatorDeallocate(a._data);
        a._data = new_data;
        a._capacity = new_capacity;
    }

    template<typename T> void grow(Array<T> &a, size_t min_capacity)
    {
        size_t new_capacity = a._capacity*2 + 8;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        set_capacity(a, new_capacity);
    }

    template<typename T> inline void push_back(Array<T> &a, const T &item)
    {
        if (a._size + 1 > a._capacity)
            grow(a);
        a._data[a._size++] = item;
    }

    template<typename T> inline void pop_back(Array<T> &a)
    {
        a._size--;
    }

    template <typename T, typename U> void insert(Array<T>& a, T * pos, U * first, U * last)
    {
        if(pos < begin(a) || pos > end(a) || (last < first))
            return;

        size_t offset_ = (pos - begin(a));
        size_t newSize_ = size(a) + (last - first);
        auto newData_ = (byte*) AllocatorAllocate(a._allocator, newSize_ * sizeof(T), alignof(T));
        memcpy(newData_, a._data, offset_ * sizeof(T));
        memcpy(&newData_[(offset_ + (last - first)) * sizeof(T)], &a._data[offset_], (size(a) - offset_) * sizeof(T));
            
        for(auto it = first; it != last; ++it)
            new (&newData_[sizeof(T) * (offset_ + (it - first))]) T(*it);
            
        AllocatorDeallocate(a._data);
        a._data = (T*)newData_;
        a._size = newSize_;
        a._capacity = newSize_;
    }

    template<typename T> void remove(Array<T>& a, const T* item)
    {
        assert(item >= begin(a) || item < end(a));

        for(size_t i = (item - begin(a)); i < size(a) - 1; ++i) 
            a[i] = a[i + 1];
        pop_back(a);
    }
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
Array<T>::Array(Allocator& allocator)
    : _allocator(&allocator)
    , _size(0)
    , _capacity(0)
    , _data(0) 
{}

template <typename T>
Array<T>::~Array()
{
    AllocatorDeallocate(_data);
}

template <typename T>
Array<T>::Array(const Array<T>& other) 
    : _allocator(other._allocator)
    , _size(0)
    , _capacity(0)
    , _data(0)
{
    const size_t n = other._size;
    array::set_capacity(*this, n);
    memcpy(_data, other._data, sizeof(T) * n);
    _size = n;
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
    const size_t n = other._size;
    array::resize(*this, n);
    memcpy(_data, other._data, sizeof(T)*n);
    return *this;
}

template <typename T>
T& Array<T>::operator[](size_t i)
{
    return _data[i];
}

template <typename T>
const T& Array<T>::operator[](size_t i) const
{
    return _data[i];
}

template <typename T>
size_t Array<T>::size() const
{
    return array::size(*this);
}

template <typename T>
bool Array<T>::any() const
{
    return array::any(*this);
}

template <typename T>
bool Array<T>::empty() const
{
    return array::empty(*this);
}

template <typename T>
T& Array<T>::front()
{
    return array::front(*this);
}

template <typename T>
T const & Array<T>::front() const
{
    return array::front(*this);
}

template <typename T>
T& Array<T>::back()
{
    return array::back(*this);
}

template <typename T>
T const & Array<T>::back() const
{
    return array::back(*this);
}

template <typename T>
void Array<T>::resize(size_t size)
{
    array::resize(*this, size);
}

template <typename T>
void Array<T>::clear()
{
    array::clear(*this);
}

template <typename T>
void Array<T>::set_capacity(size_t size)
{
    array::set_capacity(*this, size);
}

template <typename T>
void Array<T>::reserve(size_t size)
{
    array::reserve(*this, size);
}

template <typename T>
void Array<T>::grow(size_t min_capacity)
{
    array::grow(*this, min_capacity);
}

template <typename T>
void Array<T>::trim()
{
    array::trim(*this);
}

template <typename T>
void Array<T>::push_back(T const & item)
{
    array::push_back(*this, item);
}

template <typename T>
void Array<T>::pop_back()
{
    array::pop_back(*this);
}

template <typename T>
void Array<T>::remove(T const * item)
{
    array::remove(*this, item);
}

template <typename T>
template <typename U>
void Array<T>::insert(T * pos, U * first, U * last)
{
    array::insert(*this, pos, first, last);
}

NAMESPACE_CORE_END
