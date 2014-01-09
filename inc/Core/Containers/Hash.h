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
#include <cstddef>

NAMESPACE_CORE_BEGIN

/**
 * Pointer hash function.
 * @param value value to get hash of
 * @return hash
 */
template <class T> 
unsigned MakeHash(T* value)
{
    return ((unsigned)(size_t)value) / sizeof(T);
}

/**
 * Const pointer hash function.
 * @param value value to get hash of
 * @return hash
 */
template <class T> 
unsigned MakeHash(const T* value)
{
    return ((unsigned)(size_t)value) / sizeof(T);
}

/**
 * Generic hash function.
 * @param value value to get hash of
 * @return hash
 */
template <class T> 
unsigned MakeHash(const T& value)
{
    return value.ToHash();
}

/**
 * Void pointer hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(void* value)
{
    return (unsigned)(size_t)value;
}

/**
 * Const void pointer hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const void* value)
{
    return (unsigned)(size_t)value;
}

/**
 * Long long hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const long long& value)
{
    return (value >> 32) | (value & 0xffffffff);
}

/**
 * Unsigned long long hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const unsigned long long& value)
{
    return (value >> 32) | (value & 0xffffffff);
}

/**
 * Int hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const int& value)
{
    return value;
}

/**
 * Unsigned hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const unsigned& value)
{
    return value;
}

/**
 * Short hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const short& value)
{
    return value;
}

/**
 * Unsigned short hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const unsigned short& value)
{
    return value;
}

/**
 * Char hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const char& value)
{
    return value;
}

/**
 * Unsigned char hash function.
 * @param value value to get hash of
 * @return hash
 */
template<>
inline unsigned MakeHash(const unsigned char& value)
{
    return value;
}

NAMESPACE_CORE_END
