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
#include "Core/Containers/Hash.h"


NAMESPACE_CORE_BEGIN

/// %Pair template class.
template <class T, class U> class Pair
{
public:
    /// Construct undefined.
    Pair()
    {
    }

    /**
     * Construct with values.
     * @param first first element of the pair
     * @param second second element of the pair
     */
    Pair(const T& first, const U& second) :
        first(first),
        second(second)
    {
    }

    /**
     * Test for equality than with another pair.
     * @param rhs pair to compare to
     * @return whether pair is equal to rhs
     */
    bool operator == (const Pair<T, U>& rhs) const { return first == rhs.first && second == rhs.second; }

    /**
     * Test for inequality than with another pair.
     * @param rhs pair to compare to
     * @return whether pair is different from rhs
     */
    bool operator != (const Pair<T, U>& rhs) const { return first != rhs.first || second != rhs.second; }

    /**
     * Test for lesser than with another pair.
     * @param rhs pair to compare to
     * @return whether pair is lesser than rhs
     */
    bool operator < (const Pair<T, U>& rhs) const
    {
        if (first < rhs.first)
            return true;
        if (first != rhs.first)
            return false;
        return second < rhs.second;
    }

    /**
     * Test for greater than with another pair.
     * @param rhs pair to compare to
     * @return whether pair is greater than rhs
     */
    bool operator > (const Pair<T, U>& rhs) const
    {
        if (first > rhs.first)
            return true;
        if (first != rhs.first)
            return false;
        return second > rhs.second;
    }

    /**
     * Get hash value.
     * @return hash value for HashSet & HashMap.
     */
    unsigned ToHash() const { return (MakeHash(first) & 0xffff) | (MakeHash(second) << 16); }

    T first; //!< First value.
    U second; //!< Second value.
};

/**
 * Construct a pair.
 * @param first first element of the pair
 * @param second second element of the pair
 * @return pair
 */
 template <class T, class U> Pair<T, U> MakePair(const T& first, const U& second)
{
    return Pair<T, U>(first, second);
}

NAMESPACE_CORE_END
