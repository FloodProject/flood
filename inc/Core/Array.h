/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Containers.h"
#include "Core/Memory.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

template <typename T>
Array<T>::Array()
    : _allocator(AllocatorGetHeap())
    , _size(0)
    , _capacity(0)
    , _data(0)
{}

//-----------------------------------//

template <typename T>
Array<T>::Array(Allocator& allocator)
    : _allocator(&allocator)
    , _size(0)
    , _capacity(0)
    , _data(0) 
{}

//-----------------------------------//

template <typename T>
Array<T>::Array(const Array<T>& other) 
    : _allocator(other._allocator)
    , _size(0)
    , _capacity(0)
    , _data(0)
{
    const size_t n = other._size;
    setCapacity(n);
    copyRange(_data, other._data, n, std::integral_constant<bool, std::is_pod<T>::value>());
    _size = n;
}

//-----------------------------------//

template <typename T>
Array<T>::Array(Array&& other)
    : _allocator(other._allocator)
    , _size(other._size)
    , _capacity(other._capacity)
    , _data(other._data)
{
    other._allocator = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;
}

//-----------------------------------//

template <typename T>
Array<T>::~Array()
{
    destructRange(_data, _size, std::integral_constant<bool, std::is_pod<T>::value>());
    AllocatorDeallocate(_data);
}

//-----------------------------------//

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
    if(this != &other)
    {
        const size_t n = other._size;
        resize(n);
        copyRange(_data, other._data, n, std::integral_constant<bool, std::is_pod<T>::value>());
    }
    
    return *this;
}

//-----------------------------------//

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& other)
{
    destructRange(_data, _size, std::integral_constant<bool, std::is_pod<T>::value>());
    AllocatorDeallocate(_data);

    _allocator = other._allocator;
    _size = other._size;
    _capacity = other._capacity;
    _data = other._data;

    other._allocator = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;

    return *this;
}

//-----------------------------------//

template <typename T>
T& Array<T>::operator[](size_t i)
{
    return _data[i];
}

//-----------------------------------//

template <typename T>
const T& Array<T>::operator[](size_t i) const
{
    return _data[i];
}

//-----------------------------------//

template <typename T>
T * Array<T>::begin()
{
    return _data;
}

//-----------------------------------//

template <typename T>
T const * Array<T>::begin() const
{
    return _data;
}

//-----------------------------------//

template <typename T>
T const * Array<T>::cbegin() const
{
    return _data;
}

//-----------------------------------//

template <typename T>
T * Array<T>::end()
{
    return _data + _size;
}

//-----------------------------------//

template <typename T>
T const * Array<T>::end() const
{
    return _data + _size;
}

//-----------------------------------//

template <typename T>
T const * Array<T>::cend() const
{
    return _data + _size;
}

//-----------------------------------//

template <typename T>
T * Array<T>::data()
{
    return _data;
}

//-----------------------------------//

template <typename T>
T const * Array<T>::data() const
{
    return _data;
}

//-----------------------------------//

template <typename T>
T& Array<T>::front()
{
    return _data[0];
}

//-----------------------------------//

template <typename T>
T const & Array<T>::front() const
{
    return _data[0];
}

//-----------------------------------//

template <typename T>
T& Array<T>::back()
{
    return _data[_size - 1];
}

//-----------------------------------//

template <typename T>
T const & Array<T>::back() const
{
    return _data[_size - 1];
}

//-----------------------------------//

template <typename T>
size_t Array<T>::size() const
{
    return _size;
}

//-----------------------------------//

template <typename T>
size_t Array<T>::capacity() const
{
    return _capacity;
}

//-----------------------------------//

template <typename T>
bool Array<T>::any() const
{
    return _size != 0;
}

//-----------------------------------//

template <typename T>
bool Array<T>::empty() const
{
    return _size == 0;
}

//-----------------------------------//

template <typename T>
void Array<T>::resize(size_t new_size)
{
    if (new_size > _capacity)
        grow(new_size);
    else if (new_size > _size)
        constructRange(_data + _size, new_size - _size, std::integral_constant<bool, std::is_pod<T>::value>());
    else
        destructRange(_data + new_size, _size - new_size, std::integral_constant<bool, std::is_pod<T>::value>());

    _size = new_size;
}

//-----------------------------------//

template <typename T>
void Array<T>::reserve(size_t new_capacity)
{
    if (new_capacity > _capacity)
        setCapacity(new_capacity);
}

//-----------------------------------//

template <typename T>
void Array<T>::trim()
{
    setCapacity(_size);
}

//-----------------------------------//

template <typename T>
void Array<T>::clear()
{
    resize(0);
}

//-----------------------------------//

template <typename T>
void Array<T>::pushBack(T const & item)
{
    if (_size + 1 > _capacity)
        grow();
    _data[_size++] = item;
}

//-----------------------------------//

template <typename T>
void Array<T>::popBack()
{
    destructRange(_data + _size - 1, 1, std::integral_constant<bool, std::is_pod<T>::value>());
    _size--;
}

//-----------------------------------//

template <typename T>
template <typename Iter>
void Array<T>::insert(T * pos, Iter first, Iter last)
{
    if(pos < begin() || pos > end() || (last < first))
        return;

    size_t offset_ = (pos - begin());
    size_t newSize_ = size() + (last - first);
    auto newData_ = (byte*) AllocatorAllocate(_allocator, newSize_ * sizeof(T), alignof(T));

    moveRange((T*)newData_, _data, offset_, std::integral_constant<bool, std::is_pod<T>::value>());

    for(auto it = first; it != last; ++it)
        new (&newData_[sizeof(T) * (offset_ + (it - first))]) T(*it);

    moveRange((T*)(newData_ + (offset_ + (last - first)) * sizeof(T)), _data + offset_, size() - offset_, std::integral_constant<bool, std::is_pod<T>::value>());

    AllocatorDeallocate(_data);
    _data = (T*)newData_;
    _size = newSize_;
    _capacity = newSize_;
}

//-----------------------------------//

template <typename T>
void Array<T>::remove(T const * item)
{
    assert(item >= begin() || item < end());

    for(size_t i = (item - begin()); i < size() - 1; ++i) 
        _data[i] = _data[i + 1];
    popBack();
}

//-----------------------------------//

template <typename T>
Allocator * Array<T>::allocator()
{
    return _allocator;
}

//-----------------------------------//

template <typename T>
Allocator const * Array<T>::allocator() const
{
    return _allocator;
}

//-----------------------------------//

template <typename T>
void Array<T>::setCapacity(size_t new_capacity)
{
    if (new_capacity == _capacity)
        return;

    if (new_capacity < _size)
        resize(new_capacity);

    T *new_data = 0;
    if (new_capacity > 0) {
        new_data = (T*) AllocatorAllocate(_allocator, sizeof(T) * new_capacity, alignof(T));

        moveRange(new_data, _data, _size, std::integral_constant<bool, std::is_pod<T>::value>());
        constructRange(new_data + _size, new_capacity - _size, std::integral_constant<bool, std::is_pod<T>::value>());
    }

    destructRange(_data, _size, std::integral_constant<bool, std::is_pod<T>::value>());
    AllocatorDeallocate(_data);
    _data = new_data;
    _capacity = new_capacity;
}

//-----------------------------------//

template <typename T>
void Array<T>::grow(size_t min_capacity)
{
    size_t new_capacity = (_capacity * 2) + 8;
    if (new_capacity < min_capacity)
        new_capacity = min_capacity;
    setCapacity(new_capacity);
}

//-----------------------------------//

template <typename T>
void Array<T>::constructRange(T * data, size_t count, std::true_type)
{
#ifdef _DEBUG
    memset(data, 0, sizeof(T) * count);
#endif
}

//-----------------------------------//

template <typename T>
void Array<T>::constructRange(T * data, size_t count, std::false_type)
{
    for(size_t i = 0; i < count; ++i)
        new (data + i) T();
}

//-----------------------------------//

template <typename T>
void Array<T>::copyRange(T * data, T * src, size_t count, std::true_type)
{
    memcpy(data, src, sizeof(T) * count);
}

//-----------------------------------//

template <typename T>
void Array<T>::copyRange(T * data, T * src, size_t count, std::false_type)
{
    for(size_t i = 0; i < count; ++i)
        new (data + i) T( *(src + i) );
}

//-----------------------------------//

template <typename T>
void Array<T>::moveRange(T * data, T * src, size_t count, std::true_type)
{
    memcpy(data, src, sizeof(T) * count);
}

//-----------------------------------//

template <typename T>
void Array<T>::moveRange(T * data, T * src, size_t count, std::false_type)
{
    for(size_t i = 0; i < count; ++i)
        new (data + i) T(std::move( *(src + i) ));
}

//-----------------------------------//

template <typename T>
void Array<T>::destructRange(T * data, size_t count, std::true_type)
{
#ifdef _DEBUG
    memset(data, 0, sizeof(T) * count);
#endif
}

//-----------------------------------//

template <typename T>
void Array<T>::destructRange(T * data, size_t count, std::false_type)
{
    for(size_t i = 0; i < count; ++i)
        data[i].~T();
}

NAMESPACE_CORE_END