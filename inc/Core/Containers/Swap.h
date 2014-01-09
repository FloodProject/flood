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

NAMESPACE_CORE_BEGIN

class HashBase;
class String;
class VectorBase;

/**
 * Swap two values.
 * @param first first value to be swapped
 * @param second second value to be swapped
 */
template<class T> inline void Swap(T& first, T& second)
{
    T temp = first;
    first = second;
    second = temp;
}

/**
 * Swap two hash bases.
 * @param first first hash base to swap
 * @param second second hash base to swap
 */
template<> void Swap<HashBase>(HashBase& first, HashBase& second);

/**
 * Swap two strings.
 * @param first first string to swap
 * @param second second string to swap
 */
template<> void Swap<String>(String& first, String& second);

/**
 * Swap two vector bases.
 * @param first first vector base to swap
 * @param second second vector base to swap
 */
template<> void Swap<VectorBase>(VectorBase& first, VectorBase& second);

NAMESPACE_CORE_END
