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
#include "Core/Containers/Vector.h"

#include <cstring>
#include <cstdarg>
#include <cctype>
#include <iostream>

NAMESPACE_CORE_BEGIN

static const int CONVERSION_bufferLENGTH = 128;

class WString;
class UTF8String;

/// String class.
class API_CORE FLD_IGNORE_GEN String
{
public:
    friend class String;
    typedef RandomAccessIterator<char> Iterator;
    typedef RandomAccessConstIterator<char> ConstIterator;

    /// Construct empty.
    String() :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
    }

    /**
     * Construct from another string.
     * @param str string to construct from
     */
    String(const String& str) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = str;
    }

    /**
     * Move construct from another string.
     * @param str string to construct from
     */
     String(String&& str) :
        length(str.length),
        capacity(str.capacity),
        buffer(str.buffer)
    {
        str.buffer = nullptr;
        str.capacity = 0;
        str.length = 0;
    }

    /**
     * Construct from a C string.
     * @param str string to construct from
     */
     String(const char* str) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = str;
    }

    /**
     * Construct from a C string.
     * @param str string to construct from
     */
    String(char* str) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = (const char*)str;
    }

    /**
     * Construct from a char array and length.
     * @param str string to construct from
     * @param length length of str to use
     */
    String(const char* str, unsigned length) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        Resize(length);
        CopyChars(buffer, str, length);
    }

    /**
     * Construct from an utf8 string.
     * @param str string to construct from
     */
    String(const UTF8String& str);

    /**
     * Construct from an integer.
     * @param value value to construct from
     */
    explicit String(int value);

    /**
     * Construct from a short integer.
     * @param value value to construct from
     */
    explicit String(short value);

    /**
     * Construct from a long integer.
     * @param value value to construct from
     */
    explicit String(long value);

    /**
     * Construct from a long long integer.
     * @param value value to construct from
     */
    explicit String(long long value);

    /**
     * Construct from an unsigned integer.
     * @param value value to construct from
     */
    explicit String(unsigned value);

    /**
     * Construct from an unsigned short integer.
     * @param value value to construct from
     */
    explicit String(unsigned short value);

    /**
     * Construct from an unsigned long integer.
     * @param value value to construct from
     */
    explicit String(unsigned long value);

    /**
     * Construct from an unsigned long long integer.
     * @param value value to construct from
     */
    explicit String(unsigned long long value);

    /**
     * Construct from a float.
     * @param value value to construct from
     */
    explicit String(float value);

    /**
     * Construct from a double.
     * @param value value to construct from
     */
    explicit String(double value);

    /**
     * Construct from a bool.
     * @param value value to construct from
     */
    explicit String(bool value);

    /**
     * Construct from a character.
     * @param value value to construct from
     */
    explicit String(char value);

    /**
     * Construct from a character and fill length.
     * @param value value to construct from
     * @param length lenght to repeat value
     */
    explicit String(char value, unsigned length);

    /**
     * Construct from a convertable value.
     * @param value value to construct from
     */
    template <class T> 
    explicit String(const T& valuIe) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = value.ToString();
    }

    /// Destruct.
    ~String()
    {
        if (capacity)
            Deallocate(buffer);
    }

    /**
     * Assign a string.
     * @param rhs string to assign from
     * @return resulting string
     */
    String& operator = (const String& rhs)
    {
        Resize(rhs.length);
        CopyChars(buffer, rhs.buffer, rhs.length);

        return *this;
    }

    /**
     * Move assign a string.
     * @param rhs string to assign from
     * @return resulting string
     */
    String& operator = (String&& rhs)
    {
        Resize(0);
        length = rhs.length;
        capacity = rhs.capacity;
        buffer = rhs.buffer;
        rhs.buffer = nullptr;
        rhs.length = 0;
        rhs.capacity = 0;

        return *this;
    }

    /**
     * Assign a C string.
     * @param rhs string to assign
     * @return resulting string
     */
    String& operator = (const char* rhs)
    {
        unsigned rhsLength = CStringLength(rhs);
        Resize(rhsLength);
        CopyChars(buffer, rhs, rhsLength);

        return *this;
    }

    /**
     * Append a string.
     * @param rhs string to append
     * @return resulting string
     */
    String& operator += (const String& rhs)
    {
        unsigned oldLength = length;
        Resize(length + rhs.length);
        CopyChars(buffer + oldLength, rhs.buffer, rhs.length);

        return *this;
    }

    /**
     * Append a C string.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (const char* rhs)
    {
        unsigned rhsLength = CStringLength(rhs);
        unsigned oldLength = length;
        Resize(length + rhsLength);
        CopyChars(buffer + oldLength, rhs, rhsLength);

        return *this;
    }

    /**
     * Append a character.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (char rhs)
    {
        unsigned oldLength = length;
        Resize(length + 1);
        buffer[oldLength]  = rhs;

        return *this;
    }

    /**
     * Append a character.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (const uint8 rhs)
    {
        unsigned oldLength = length;
        Resize(length + 1);
        buffer[oldLength]  = static_cast<char>(rhs);

        return *this;
    }

    /**
     * Append an integer.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (int rhs);

    /**
     * Append a short integer.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (short rhs);

    /**
     * Append an unsigned integer.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (unsigned rhs);

    /**
     * Append a short unsigned integer.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (unsigned short rhs);

    /**
     * Append a long long.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (long long rhs);

    /**
     * Append a float.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (float rhs);

    /**
     * Append a double.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (double rhs);

    /**
     * Append a bool.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (bool rhs);

    /**
     * Append an utf8 string.
     * @param rhs value to append
     * @return resulting string
     */
    String& operator += (const UTF8String& rhs);

    /**
     * Append an arbitraty type.
     * @param rhs value to append
     * @return resulting string
     */
    template <class T> 
    String operator += (const T& rhs) { return *this += rhs.ToString(); }

    /**
     * Add a string.
     * @param rhs value to add
     * @return resulting string
     * @remark this string is not modified
     */
    String operator + (const String& rhs) const
    {
        String ret;
        ret.Resize(length + rhs.length);
        CopyChars(ret.buffer, buffer, length);
        CopyChars(ret.buffer + length, rhs.buffer, rhs.length);

        return ret;
    }

    /**
     * Add a C string.
     * @param rhs value to add
     * @return resulting string
     * @remark this string is not modified
     */
    String operator + (const char* rhs) const
    {
        unsigned rhsLength = CStringLength(rhs);
        String ret;
        ret.Resize(length + rhsLength);
        CopyChars(ret.buffer, buffer, length);
        CopyChars(ret.buffer + length, rhs, rhsLength);

        return ret;
    }

    /**
     * Add a character.
     * @param rhs value to add
     * @return resulting string
     * @remark this string is not modified
     */
    String operator + (char rhs) const
    {
        String ret(*this);
        ret += rhs;

        return ret;
    }

    /**
     * Test for equality with another string.
     * @param rhs string to compare with
     * @return whether strings are equal
     */
    bool operator == (const String& rhs) const { return strcmp(CString(), rhs.CString()) == 0; }

    /**
     * Test for inequality with another string.
     * @param rhs string to compare with
     * @return whether strings are different
     */
    bool operator != (const String& rhs) const { return strcmp(CString(), rhs.CString()) != 0; }

    /**
     * Test if string is less than another string.
     * @param rhs string to compare with
     * @return whether string is lesser than rhs
     */
    bool operator < (const String& rhs) const { return strcmp(CString(), rhs.CString()) < 0; }

    /**
     * Test if string is less or equal to another string.
     * @param rhs string to compare with
     * @return whether string is lesser than or equal to rhs
     */
    bool operator <= (const String& rhs) const { return strcmp(CString(), rhs.CString()) <= 0; }

    /**
     * Test if string is greater than another string.
     * @param rhs string to compare with
     * @return whether string is greater than rhs
     */
    bool operator > (const String& rhs) const { return strcmp(CString(), rhs.CString()) > 0; }

    /**
     * Test if string is greater or equal to another string.
     * @param rhs string to compare with
     * @return whether string is greater than or equal to rhs
     */
    bool operator >= (const String& rhs) const { return strcmp(CString(), rhs.CString()) >= 0; }

    /**
     * Test for equality with a C string.
     * @param rhs string to compare with
     * @return whether strings are equal
     */
    bool operator == (const char* rhs) const { return strcmp(CString(), rhs) == 0; }

    /**
     * Test for inequality with a C string.
     * @param rhs string to compare with
     * @return whether strings are different
     */
    bool operator != (const char* rhs) const { return strcmp(CString(), rhs) != 0; }

    /**
     * Test if string is less than a C string.
     * @param rhs string to compare with
     * @return whether string is lesser than rhs
     */
    bool operator < (const char* rhs) const { return strcmp(CString(), rhs) < 0; }

    /**
     * Test if string is less or equal to a C string.
     * @param rhs string to compare with
     * @return whether string is lesser than or equal to rhs
     */
    bool operator <= (const char* rhs) const { return strcmp(CString(), rhs) <= 0; }

    /**
     * Test if string is greater than a C string.
     * @param rhs string to compare with
     * @return whether string is greater than rhs
     */
    bool operator > (const char* rhs) const { return strcmp(CString(), rhs) > 0; }

    /**
     * Test if string is greater or equal to a C string.
     * @param rhs string to compare with
     * @return whether string is greater than or equal to rhs
     */
    bool operator >= (const char* rhs) const { return strcmp(CString(), rhs) >= 0; }

    /**
     * Return char at index.
     * @param index index of char
     * @return char at index
     */
    char& operator [] (unsigned index) { assert(index < length); return buffer[index]; }

    /**
     * Return const char at index.
     * @param index index of char
     * @return char at index
     */
    const char& operator [] (unsigned index) const { assert(index < length); return buffer[index]; }

    /**
     * Return char at index.
     * @param index index of char
     * @return char at index
     */
    char& At(unsigned index) { assert(index < length); return buffer[index]; }

    /**
     * Return const char at index.
     * @param index index of char
     * @return char at index
     */
    const char& At(unsigned index) const { assert(index < length); return buffer[index]; }

    /**
     * Assigns this string a string from an input stream.
     * @param is input stream to extract string from
     * @return input stream
     */
    std::istream& operator>> (std::istream& is);

    /**
     * Outputs this string into an output stream.
     * @param os output stream to output string to
     * @return output stream
     */
    std::ostream& operator<< (std::ostream& os);

    /**
     * Replace all occurrences of a character.
     * @param replaceThis char to be replaced
     * @param replaceWith char to replace with
     * @param caseSensitive whether char to be replaced capitalization's matters
     */
    void Replace(char replaceThis, char replaceWith, bool caseSensitive = true);

    /**
     * Replace all occurrences of a string.
     * @param replaceThis string to be replaced
     * @param replaceWith string to replace with
     * @param caseSensitive whether string to be replaced capitalization's matters
     */
    void Replace(const String& replaceThis, const String& replaceWith, bool caseSensitive = true);

    /**
     * Replace a substring.
     * @param pos index to start replacing
     * @param length length to be replaced
     * @param replaceWith string to replace with
     */
    void Replace(unsigned pos, unsigned length, const String& replaceWith);

    /**
     * Replace a substring with a C string.
     * @param pos index to start replacing
     * @param length length to be replaced
     * @param replaceWith string to replace with
     */
    void Replace(unsigned pos, unsigned length, const char* replaceWith);

    /**
     * Replace a substring by iterators.
     * @param start iterator to the beginning
     * @param end iterator to the end
     * @param replaceWith string to replace with
     */
    Iterator Replace(const Iterator& start, const Iterator& end, const String& replaceWith);

    /**
     * Return a string with all occurrences of a character replaced.
     * @param replaceThis char to be replaced
     * @param replaceWith char to replace with
     * @param caseSensitive whether char to be replaced capitalization's matters
     * @return resulting string
     */
    String Replaced(char replaceThis, char replaceWith, bool caseSensitive = true) const;

    /**
     * Return a string with all occurrences of a string replaced.
     * @param replaceThis string to be replaced
     * @param replaceWith string to replace with
     * @param caseSensitive whether string to be replaced capitalization's matters
     * @return resulting string
     */
    String Replaced(const String& replaceThis, const String& replaceWith, bool caseSensitive = true) const;

    /**
     * Append a string.
     * @param str string to append
     * @return resulting string
     */
    String& Append(const String& str);

    /**
     * Append a C string.
     * @param str string to append
     * @return resulting string
     */
    String& Append(const char* str);

    /**
     * Append a character.
     * @param c char to append
     * @return resulting string
     */
    String& Append(char c);

    /**
     * Append characters.
     * @param str string to append
     * @param length number of chars of str to append
     * @return resulting string
     */
    String& Append(const char* str, unsigned length);

    /**
     * Insert a string.
     * @param pos index to insert string at
     * @param str string to insert
     */
    void Insert(unsigned pos, const String& str);

    /**
     * Insert a character.
     * @param pos index to insert char at
     * @param c char to insert
     */
    void Insert(unsigned pos, char c);

    /**
     * Insert a string by iterator.
     * @param dest iterator to position to insert string at
     * @param str string to insert
     * @return iterator to dest
     */
    Iterator Insert(const Iterator& dest, const String& str);

    /**
     * Insert a string partially by iterators.
     * @param dest iterator to position to insert string at
     * @param start iterator to starting point in string to insert
     * @param end iterator to ending point in string to insert
     * @return iterator to dest
     */
    Iterator Insert(const Iterator& dest, const Iterator& start, const Iterator& end);

    /**
     * Insert a character by iterator.
     * @param c char to insert
     * @return iterator to dest
     */
    Iterator Insert(const Iterator& dest, char c);

    /**
     * Erase a substring.
     * @param pos index to star erasing from
     * @param length number of characters to erase, default is 1
     */
    void Erase(unsigned pos, unsigned length = 1);

    /**
     * Erase a character by iterator.
     * @param it iterator of char to erase
     * @return iterator to position of erased character
     */
    Iterator Erase(const Iterator& it);

    /**
     * Erase a substring by iterators.
     * @param start iterator to start of substring to erase
     * @param end iterator to ending of substring to erase
     * @return iterator to beginning of erased substring
     */
    Iterator Erase(const Iterator& start, const Iterator& end);

    /**
     * Resize the string.
     * @param newLength new size of string
     */
    void Resize(unsigned newLength);

    /**
     * Set new capacity.
     * @param newCapacity new capacity of string
     */
    void Reserve(unsigned newCapacity);


    /// Reallocate so that no extra memory is used.
    void Compact();

    
    /// Clear the string.
    void Clear();

    /**
     * Swap with another string.
     * @param str string to swap with
     */
    void Swap(String& str);

    /**
     * Get iterator to the beginning of the string.
     * @return iterator to the beginning
     */
    Iterator Begin() { return Iterator(buffer); }

    /**
     * Get const iterator to the beginning of the string.
     * @return iterator to the beginning
     */
    ConstIterator Begin() const { return ConstIterator(buffer); }

    /**
     * Get iterator to the end of the string.
     * @return iterator to the end
     */
    Iterator End() { return Iterator(buffer + length); }

    /**
     * Get const iterator to the end of the string.
     * @return iterator to the end
     */
    ConstIterator End() const { return ConstIterator(buffer + length); }

    /**
     * Get first char of string
     * @return first char, or 0 if empty
     */
    char Front() const { return buffer[0]; }

    /**
     * Get last char of string
     * @return last char, or 0 if empty
     */
    char Back() const { return length ? buffer[length - 1] : buffer[0]; }

    /**
     * Return a substring from position to end.
     * @param pos index of the beginning of the substring
     * @return resulting substring
     */
    String Substring(unsigned pos) const;

    /**
     * Return a substring with length from position.
     * @param pos index of the beginning of the substring
     * @param length number of chars of the substring
     * @return resulting substring
     */
    String Substring(unsigned pos, unsigned length) const;

    /**
     * Return string with whitespace trimmed from the beginning and the end.
     * @return resulting string
     */
    String Trimmed() const;

    /**
     * Return string in uppercase.
     * @return resulting string
     */
    String ToUpper() const;

    /**
     * Return string in lowercase.
     * @return resulting string
     */
    String ToLower() const;

    /**
     * Return substrings split by a separator char.
     * @param separator bounding character
     * @return vector containing resulting substrings
     */
    Vector<String> Split(char separator) const;

    /**
     * Join substrings with a 'glue' string.
     * @param subStrings vector containing substrings to join
     * @param glue binding character
     * @remark resulting string is stored in this string
     */
     void Join(const Vector<String>& subStrings, String glue);

    /**
     * Find first occurrence of a string.
     * @param str string to find
     * @param startPos index to start search from
     * @param caseSensitive whether search is case sensitive or not
     * @return index to the first occurrence of a string, or NPOS if not found
     */
    unsigned Find(const String& str, unsigned startPos = 0, bool caseSensitive = true) const;

    /**
     * Find first occurrence of a char.
     * @param c char to find
     * @param startPos index to start search from
     * @param caseSensitive whether search is case sensitive or not
     * @return index to the first occurrence of a character, or NPOS if not found
     */
    unsigned Find(char c, unsigned startPos = 0, bool caseSensitive = true) const;

    /**
     * Find last occurrence of a string.
     * @param str string to find
     * @param startPos index to start search from
     * @param caseSensitive whether search is case sensitive or not
     * @return index to the last occurrence of a string, or NPOS if not found
     */
    unsigned FindLast(const String& str, unsigned startPos = NPOS, bool caseSensitive = true) const;

    /**
     * Find last occurrence of a char.
     * @param c char to find
     * @param startPos index to start search from
     * @param caseSensitive whether search is case sensitive or not
     * @return index to the last occurrence of a character, or NPOS if not found
     */
    unsigned FindLast(char c, unsigned startPos = NPOS, bool caseSensitive = true) const;

    /**
     * Check if this string starts with given substring
     * @param str substring to check
     * @param caseSensitive whether check is case sensitive or not
     * @return whether this string starts with a specific substring
     */
    bool StartsWith(const String& str, bool caseSensitive = true) const;

    /**
     * Check if this string ends with given substring
     * @param str substring to check
     * @param caseSensitive whether check is case sensitive or not
     * @return whether this string ends with a specific substring
     */
    bool EndsWith(const String& str, bool casSensitive = true) const;

    /**
     * Get C string.
     * @return the C string
     */
    const char* CString() const { return buffer; }

    /**
     * Get buffer pointer.
     * @return pointer to buffer
     */
    char* CStringPtr() { return buffer; }

    /**
     * Get length.
     * @return string length
     */
    unsigned Length() const { return length; }

    /**
     * Get string capacity.
     * @return buffer capacity.
     */
    unsigned Capacity() const { return capacity; }

    /**
     * Check if string is empty.
     * @return whether the string is empty
     */
    bool Empty() const { return length == 0; }

    /**
     * Compare this string to another string.
     * @param str string to compare to
     * @param caseSensitive check if comparison is case sensitive
     * @return comparison result with a string
     */
    int Compare(const String& str, bool caseSensitive = true) const;

    /**
     * Compare this string to a C string.
     * @param str string to compare to
     * @param caseSensitive check if comparison is case sensitive
     * @return comparison result with a C string
     */
    int Compare(const char* str, bool caseSensitive = true) const;

    /**
     * Check if this string contains a specific string
     * @param str string to check
     * @return whether this string contains a specific occurence of string
     */
    bool Contains(const String& str) const { return Find(str) != NPOS; }

    /**
     * Check if this string contains a character
     * @param c character to check
     * @return whether string contains a specific character
     */
    bool Contains(char c) const { return Find(c) != NPOS; }

    /**
     * Hash string.
     * @return hash value for HashSet & HashMap
     */
    unsigned ToHash() const
    {
        unsigned hash = 0;
        const char* ptr = buffer;
        while (*ptr)
        {
            hash = *ptr + (hash << 6) + (hash << 16) - hash;
            ++ptr;
        }

        return hash;
    }

    /**
     * Split a string by a separator char.
     * @param str string to separate
     * @param separator bounding character
     * @return vector of resulting substrings
     */
    static Vector<String> Split(const char* str, char separator);

    /**
     * Join a group of substrings with a 'glue' string
     * @param subStrings vector of substrings to join
     * @param glue binding string 
     * @return a string by joining substrings with a 'glue' string
     */
    static String Joined(const Vector<String>& subStrings, String glue);

    /**
     * Get length of a C string.
     * @param str string to get length of
     * @return length of a C string
     */
    static unsigned CStringLength(const char* str)
    {
        if (!str)
            return 0;
        #ifdef _MSC_VER
        return (unsigned)strlen(str);
        #else
        const char* ptr = str;
        while (*ptr)
            ++ptr;
        return ptr - str;
        #endif
    }

    /**
     * Append to string using formatting.
     * @param formatString formatting string
     * @return resulting string
     */
    String& AppendWithFormat(const char* formatString, ... );

    /**
     * Append to string using variable arguments.
     * @param formatString formatting string
     * @param args formatting arguments
     * @return resulting string
     */
    String& AppendWithFormatArgs(const char* formatString, va_list args);

    /**
     * Compare two C strings.
     * @param str1 first string to compare
     * @param str2 second string to compare
     * @param caseSensitive whether comparison is case sensitive
     * @return value of comparison, 0 if equal, <0 if str1 < str2, >0 otherwise
     */
    static int Compare(const char* str1, const char* str2, bool caseSensitive);

    static const unsigned NPOS = 0xffffffff; //!< Position for "not found."
    static const unsigned MIN_CAPACITY = 8; //!< Initial dynamic allocation size.
    static const String EMPTY; //!< Empty string.

private:
    /// Move a range of characters within the string.
    void MoveRange(unsigned dest, unsigned src, unsigned count)
    {
        if (count)
            memmove(buffer + dest, buffer + src, count);
    }
    
    /// Copy chars from one buffer to another.
    static void CopyChars(char* dest, const char* src, unsigned count)
    {
        #ifdef _MSC_VER
        if (count)
            memcpy(dest, src, count);
        #else
        char* end = dest + count;
        while (dest != end)
        {
            *dest = *src;
            ++dest;
            ++src;
        }
        #endif
    }
    
    /// Replace a substring with another substring.
    void Replace(unsigned pos, unsigned length, const char* srcStart, unsigned srcLength);
    
    /// String length.
    unsigned length;
    /// Capacity, zero if buffer not allocated.
    unsigned capacity;
    /// String buffer, null if not allocated.
    char* buffer;
    
    /// End zero for empty strings.
    static char endZero;
};

/**
 * Add a string to a C string.
 * @param lhs c string
 * @param rhs string to append
 * @return appended string
 */
inline String operator + (const char* lhs, const String& rhs)
{
    String ret(lhs);
    ret += rhs;
    return ret;
}

/**
 * Compares a C string to a string.
 * @param lhs first string to compare
 * @param rhs second string to compare
 * @return whether strings are equal
 */
inline bool operator == (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) == 0;
}

/**
 * Compares a C string to a string.
 * @param lhs first string to compare
 * @param rhs second string to compare
 * @return whether strings are different
 */
inline bool operator != (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) != 0;
}

/**
 * Compares a C string to a string.
 * @param lhs first string to compare
 * @param rhs second string to compare
 * @return whether lhs is lesser than rhs
 */
inline bool operator < (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) > 0;
}

/**
 * Compares a C string to a string.
 * @param lhs first string to compare
 * @param rhs second string to compare
 * @return whether lhs is greater than rhs
 */
inline bool operator > (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) < 0;
}

/**
 * Compares a C string to a string.
 * @param lhs first string to compare
 * @param rhs second string to compare
 * @return whether lhs is greater than or equal to rhs
 */
inline bool operator >= (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) <= 0;
}

/**
 * Compares a C string to a string.
 * @param lhs first string to compare
 * @param rhs second string to compare
 * @return whether lhs is lesser than or equal to rhs
 */
inline bool operator <= (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) >= 0;
}

/**
 * Assigns a string a string from an input stream.
 * @param is input stream to extract string from
 * @param rhs string to assign to
 * @return input stream
 */
inline std::istream& operator>> (std::istream& is, String& rhs)
{
    return is >> rhs.CStringPtr();
}

/**
 * Outputs a string into an output stream.
 * @param os output stream to output string to
 * @param rhs string to output
 * @return output stream
 */
inline std::ostream& operator<< (std::ostream& os, const String& rhs)
{
    return os << rhs.CString();
}

class UTF8String;

/// Wide character string. Only meant for converting from String and passing to the operating system where necessary.
class API_CORE FLD_IGNORE_GEN WString
{
public:
    /// Construct empty.
    WString();

    /**
     * Construct from a wchar_t pointer.
     * @param str wchar_t pointer
     */
    WString(const wchar_t* str);

    /**
     * Construct from an utf8 string.
     * @param str utf8 string
     */
    WString(const UTF8String& str);

    /// Destruct.
    ~WString();

    /**
     * Return char at index.
     * @param index index of char
     * @return char at index
     */
    wchar_t& operator [] (unsigned index) { assert(index < length); return buffer[index]; }

    /**
     * Return char at index.
     * @param index index of char
     * @return char at index
     */
    const wchar_t& operator [] (unsigned index) const { assert(index < length); return buffer[index]; }

    /**
     * Return char at index.
     * @param index index of char
     * @return char at index
     */
    wchar_t& At(unsigned index) { assert(index < length); return buffer[index]; }

    /**
     * Return const char at index.
     * @param index index of char
     * @return char at index
     */
    const wchar_t& At(unsigned index) const { assert(index < length); return buffer[index]; }

    /**
     * Resize the string.
     * @param newSize new size of string
     */
    void Resize(unsigned newSize);

    /**
     * Check if string is empty.
     * @return whether the string is empty
     */
    bool Empty() const { return length == 0; }

    /**
     * Get length.
     * @return string length
     */
    unsigned Length() const { return length; }

    /**
     * Get nyte lentgh.
     * @return string byte length
     */
    unsigned Bytes() const { return bytes; }

    /**
     * Get character data.
     * @return character data buffer
     */
    const wchar_t* CString() const { return buffer; }
    
private:
    unsigned length; //!< String length.
    wchar_t* buffer; //!< String buffer, null if not allocated.
    unsigned bytes; //!< String byte length.
};

class API_CORE FLD_IGNORE_GEN UTF8String
{
public:

    /// Construct empty.
    UTF8String();

    /**
     * Construct from a null-terminated wide character array.
     * @param str string to construct from
     */
    UTF8String(wchar_t* str);

    /**
     * Construct from a wide character string.
     * @param str string to construct from
     */
     UTF8String(const WString& str);

    /**
     * Construct UTF8 content from char buffer.
     * @param str buffer to construct from
     * @param isUTF8 whether buffer is encoded in Latin1 or UTF8
     */
    UTF8String(const char* str, bool isUTF8 = false);

    /**
     * Construct UTF8 content from String.
     * @param str string to construct from
     */
    UTF8String(const String& str);

    /**
     * Construct UTF8 content from UTF8String.
     * @param str string to construct from
     */
    UTF8String(const UTF8String& str);

    /**
     * Move construct UTF8 content from UTF8String.
     * @param str string to construct from
     */
    UTF8String(UTF8String&& str);

    /**
     * Construct UTF8 content from wide characters.
     * @param str string to construct from
     */
    UTF8String(const wchar_t* str);
    
    /**
     * Construct from a char array and length.
     * @param str char buffer to construct from
     * @param length length of str to use
     * @param isUTF8 whether buffer is encoded in Latin1 or UTF8
     */
    UTF8String(const char* str, unsigned length, bool isUTF8 = false) 
        : length(0)
        , capacity(0)
        , buffer(&endZero)
    {
        if(isUTF8)
        {
            Resize(length);
            CopyChars(buffer, str, length);

        }
        else
            for(unsigned i = 0; i < length; i++)
                Append(str[i]);

    }

    /**
     * Construct from an integer.
     * @param value value to construct from
     */
    explicit UTF8String(int value);

    /**
     * Construct from a short integer.
     * @param value value to construct from
     */
    explicit UTF8String(short value);

    /**
     * Construct from a long integer.
     * @param value value to construct from
     */
    explicit UTF8String(long value);

    /**
     * Construct from a long long integer.
     * @param value value to construct from
     */
    explicit UTF8String(long long value);

    /**
     * Construct from an unsigned integer.
     * @param value value to construct from
     */
    explicit UTF8String(unsigned value);

    /**
     * Construct from an unsigned short integer.
     * @param value value to construct from
     */
    explicit UTF8String(unsigned short value);

    /**
     * Construct from an unsigned long integer.
     * @param value value to construct from
     */
    explicit UTF8String(unsigned long value);

    /**
     * Construct from an unsigned long long integer.
     * @param value value to construct from
     */
    explicit UTF8String(unsigned long long value);

    /**
     * Construct from a float.
     * @param value value to construct from
     */
    explicit UTF8String(float value);

    /**
     * Construct from a double.
     * @param value value to construct from
     */
    explicit UTF8String(double value);

    /**
     * Construct from a bool.
     * @param value value to construct from
     */
    explicit UTF8String(bool value);

    /**
     * Construct from a character.
     * @param value value to construct from
     */
    explicit UTF8String(char value);

    /**
     * Construct from a character and fill length.
     * @param value value to construct from
     * @param length lenght to repeat value
     */
    explicit UTF8String(char value, unsigned length);

    /**
     * Construct from a convertable value.
     * @param value value to construct from
     */
    template <class T> 
    explicit UTF8String(const T& value) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = value.ToString();
    }

    /// Destruct.
    ~UTF8String()
    {
        if (capacity)
            Deallocate(buffer);
    }

    /**
     * Assign a String.
     * @param rhs String to assign from
     * @return resulting string
     */
    UTF8String& operator = (const String& rhs)
    {
        SetUTF8FromLatin1(rhs.CString());
        return *this;
    }

    /**
     * Assign an utf8 string.
     * @param rhs utf8 string to assign from
     * @return resulting string
     */
    UTF8String& operator = (const UTF8String& rhs)
    {
        Resize(rhs.length);
        CopyChars(buffer, rhs.buffer, rhs.length);

        return *this;
    }

    /**
     * Move assign a string.
     * @param rhs string to assign from
     * @return resulting string
     */
    UTF8String& operator = (UTF8String&& rhs)
    {
        Resize(0);
        length = rhs.length;
        capacity = rhs.capacity;
        buffer = rhs.buffer;
        rhs.buffer = nullptr;
        rhs.length = 0;
        rhs.capacity = 0;

        return *this;
    }

    /**
     * Assign a C string.
     * @param rhs string to assign
     * @return resulting string
     */
    UTF8String& operator = (const char* rhs)
    {
        SetUTF8FromLatin1(rhs);
        return *this;
    }

    /**
     * Assign a character.
     * @param rhs character to assign
     * @return resulting string
     */
    UTF8String& operator = (const char rhs)
    {
        char temp[7];
        char * dest = temp;
        EncodeUTF8(dest, (byte)rhs);
        *dest = 0;
        unsigned tLength = CStringLength(temp);
        Resize(tLength);
        CopyChars(buffer , temp, tLength);

        return *this;
    }

    /**
     * Append an utf8 string.
     * @param rhs utf8 string to append
     * @return resulting string
     */
    UTF8String& operator += (const UTF8String& rhs)
    {
        return Append(rhs);
    }

    /**
     * Append a String.
     * @param rhs String to append
     * @return resulting string
     */
    UTF8String& operator += (const String& rhs)
    {
        return Append(rhs);
    }

    /**
     * Append a C string.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (const char* rhs)
    {
        return Append(rhs);
    }

    /**
     * Append a character.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (char rhs)
    {
        return Append(rhs);
    }

    /**
     * Append an integer.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (int rhs);

    /**
     * Append a short integer.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (short rhs);

    /**
     * Append an unsigned integer.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (unsigned rhs);

    /**
     * Append a short unsigned integer.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (unsigned short rhs);

    /**
     * Append a long long.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (long long rhs);

    /**
     * Append a float.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (float rhs);

    /**
     * Append a double.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (double rhs);

    /**
     * Append a bool.
     * @param rhs value to append
     * @return resulting string
     */
    UTF8String& operator += (bool rhs);

    /**
     * Append an arbitraty type.
     * @param rhs value to append
     * @return resulting string
     */
    template <class T> 
    UTF8String operator += (const T& rhs) { return *this += rhs.ToString(); }

    /**
     * Add a string.
     * @param rhs value to add
     * @return resulting string
     * @remark this string is not modified
     */
    UTF8String operator + (const UTF8String& rhs) const
    {
        UTF8String ret;
        ret.Resize(length + rhs.length);
        CopyChars(ret.buffer, buffer, length);
        CopyChars(ret.buffer + length, rhs.buffer, rhs.length);

        return ret;
    }

    /**
     * Add a C string.
     * @param rhs value to add
     * @return resulting string
     * @remark this string is not modified
     */
    UTF8String operator + (const char* rhs) const
    {
        UTF8String ret(*this);
        ret += rhs;

        return ret;
    }

    /**
     * Add a character.
     * @param rhs value to add
     * @return resulting string
     * @remark this string is not modified
     */
    UTF8String operator + (char rhs) const
    {
        UTF8String ret(*this);
        ret += rhs;

        return ret;
    }

    /**
     * Test for equality with another string.
     * @param rhs string to compare with
     * @return whether strings are equal
     */
    bool operator == (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) == 0; }

    /**
     * Test for inequality with another string.
     * @param rhs string to compare with
     * @return whether strings are different
     */
    bool operator != (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) != 0; }

    /**
     * Test if string is less than another string.
     * @param rhs string to compare with
     * @return whether string is lesser than rhs
     */
    bool operator < (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) < 0; }

    /**
     * Test if string is less or equal to another string.
     * @param rhs string to compare with
     * @return whether string is lesser than or equal to rhs
     */
    bool operator <= (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) <= 0; }

    /**
     * Test if string is greater than another string.
     * @param rhs string to compare with
     * @return whether string is greater than rhs
     */
    bool operator > (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) > 0; }

    /**
     * Test if string is greater or equal to another string.
     * @param rhs string to compare with
     * @return whether string is greater than or equal to rhs
     */
    bool operator >= (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) >= 0; }

    /**
     * Test for equality with a C string.
     * @param rhs string to compare with
     * @return whether strings are equal
     */
    bool operator == (const char* rhs) const 
    { 
        auto str = GetUTF8FromLatin1(rhs);
        auto cmp = str.CString(); 
        return strcmp(buffer, cmp) == 0; 
    }

    /**
     * Test for inequality with a C string.
     * @param rhs string to compare with
     * @return whether strings are different
     */
    bool operator != (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) != 0; }

    /**
     * Test if string is less than a C string.
     * @param rhs string to compare with
     * @return whether string is lesser than rhs
     */
    bool operator < (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) < 0; }

    /**
     * Test if string is less or equal to a C string.
     * @param rhs string to compare with
     * @return whether string is lesser than or equal to rhs
     */
    bool operator <= (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) <= 0; }

    /**
     * Test if string is greater than a C string.
     * @param rhs string to compare with
     * @return whether string is greater than rhs
     */
    bool operator > (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) > 0; }

    /**
     * Test if string is greater or equal to a C string.
     * @param rhs string to compare with
     * @return whether string is greater than or equal to rhs
     */
    bool operator >= (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) >= 0; }

    /**
     * Return const char at index.
     * @param index index of char
     * @return char at index
     */
    const unsigned operator [] (unsigned index) const { return At(index); }

    /**
     * Assigns this string a string from an input stream.
     * @param is input stream to extract string from
     * @return input stream
     */
    std::istream& operator>> (std::istream& is);

    /**
     * Outputs this string into an output stream.
     * @param os output stream to output string to
     * @return output stream
     */
    std::ostream& operator<< (std::ostream& os);

    /**
     * Set this string's UTF8 content from a Latin1 character buffer.
     * @param str character buffer to build from
     */
    void SetUTF8FromLatin1(const char* str);

    /**
     * Get UTF8 string from a Latin1 character buffer.
     * @param str character buffer
     * @return utf8 string built from str
     */
    UTF8String GetUTF8FromLatin1(const char* str) const;

    /**
     * Set this string's UTF8 content from a wide character buffer.
     * @param str character buffer to build from
     */
    void SetUTF8FromWChar(const wchar_t* str);

    /**
     * Calculate number of characters in UTF8 content.
     * @return utf8 string length
     */
    unsigned Length() const;

    /**
     * Calculate number of bytes in UTF8 content.
     * @return utf8 string byte length
     */
    unsigned ByteLength() const { return length; };

    /**
     * Return byte offset to char in UTF8 content.
     * @param index character position
     * @return byte offset
     */
    unsigned ByteOffset(unsigned index) const;

    /**
     * Return next Unicode character from UTF8 content and increase byte offset.
     * @param byteOffset character byte offset
     * @return unicode character at offset
     */
    unsigned NextChar(unsigned& byteOffset) const;

    /**
     * Join a group of substrings with a 'glue' string
     * @param subStrings vector of substrings to join
     * @param glue binding string 
     * @return a string by joining substrings with a 'glue' string
     */
    static UTF8String Joined(const Vector<UTF8String>& subStrings, UTF8String glue);

    /**
     * Join a group of substrings with a 'glue' string
     * @param subStrings vector of substrings to join
     * @param glue binding string 
     * @return a string by joining substrings with a 'glue' string
     */
    static UTF8String Joined(const Vector<UTF8String>& subStrings, const char* glue)
    {
        UTF8String g(glue);
        return Joined(subStrings, g);
    }
    /**
     * Return string with whitespace trimmed from the beginning and the end.
     * @return resulting string
     */
    UTF8String Trimmed() const;

    /**
     * Join substrings with a 'glue' string.
     * @param subStrings vector containing substrings to join
     * @param glue binding character
     * @remark resulting string is stored in this string
     */
    void Join(const Vector<UTF8String>& subStrings, UTF8String glue);

    /**
     * Join substrings with a 'glue' string.
     * @param subStrings vector containing substrings to join
     * @param glue binding character
     * @remark resulting string is stored in this string
     */
    void Join(const Vector<UTF8String>& subStrings, const char* glue)
    {
        UTF8String g(glue);
        Join(subStrings, g);
    }

    /**
     * Find first occurrence of a string.
     * @param str string to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the first occurrence of a string, or NPOS if not found
     */
    unsigned Find(const UTF8String& str, bool caseSensitive = true, int startPos = 0) const;

    /**
     * Find first occurrence of a string.
     * @param str string to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the first occurrence of a string, or NPOS if not found
     */
    unsigned Find(const char* str, bool caseSensitive = true, int startPos = 0) const 
    {
        UTF8String t(str);
        return Find(t, caseSensitive, startPos);
    }

    /**
     * Find first occurrence of a char.
     * @param c char to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the first occurrence of a character, or NPOS if not found
     */
    unsigned Find(unsigned c, bool caseSensitive = true, int startPos = 0) const;

    /**
     * Find first occurrence of a char.
     * @param c char to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the first occurrence of a character, or NPOS if not found
     */
    unsigned Find(char c, bool caseSensitive = true, int startPos = 0) const{return Find((unsigned)(byte)c, caseSensitive, startPos);}

    /**
     * Find last occurrence of a string.
     * @param str string to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the last occurrence of a string, or NPOS if not found
     */
    unsigned FindLast(const UTF8String& str, bool caseSensitive = true, int startPos = -1) const;

    /**
     * Find last occurrence of a string.
     * @param str string to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the last occurrence of a string, or NPOS if not found
     */
    unsigned FindLast(const char* str, bool caseSensitive = true, int startPos = -1) const 
    {
        UTF8String t(str);
        return FindLast(t, caseSensitive, startPos);
    }

    /**
     * Find last occurrence of a char.
     * @param c char to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the last occurrence of a character, or NPOS if not found
     */
    unsigned FindLast(unsigned c, bool caseSensitive = true, int startPos = -1) const;

    /**
     * Find last occurrence of a char.
     * @param c char to find
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the last occurrence of a character, or NPOS if not found
     */
    unsigned FindLast(char c, bool caseSensitive = true, int startPos = -1) const { return FindLast((unsigned)(byte)c, caseSensitive, startPos); }

    /**
     * Find first instance of a char not matching the provided string.
     * @param str string to check against
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the first occurrence of character not matching str, or NPOS if not found
     */
    unsigned FindFirstNotOf(const UTF8String& str, bool caseSensitive = true, int startPos = 0) const;

    /**
     * Find last instance of a char not matching the provided string.
     * @param str string to check against
     * @param caseSensitive whether search is case sensitive or not
     * @param startPos index to start search from
     * @return index to the last occurrence of character not matching str, or NPOS if not found
     */
    unsigned FindLastNotOf(const UTF8String& str, bool caseSensitive = true, int startPos = -1) const;

    /**
     * Convert UTF8STring to String.
     * @return converted string
     */
    String GetString() const;

    /**
     * Get number of bytes an utf8 char in this string occupies.
     * @param utf8Char first byte of the utf8 character
     * @return number of bytes the utf8 character takes
     */
    unsigned CharOffset(char utf8Char);

    /**
     * Return Unicode character at index from UTF8 content.
     * @param index index of char
     * @return char at index
     */
    unsigned At(unsigned index) const;

    /**
     * Split a string by a separator char.
     * @param str string to separate
     * @param separator bounding character
     * @return vector of resulting substrings
     */
    static Vector<UTF8String> Split(const char* str, unsigned separator);

    /**
     * Split this string by a separator char.
     * @param separator bounding character
     * @return vector of resulting substrings
     */
    Vector<UTF8String> Split(unsigned separator) const;

    /**
     * Split a string by a separator char.
     * @param str string to separate
     * @param separator bounding character
     * @param ret vector to store resulting substrings
     */
    static void Split(const char* str, unsigned separator, Vector<UTF8String>& ret);

    /**
     * Split this string by a separator char.
     * @param separator bounding character
     * @param ret vector to store resulting substrings
     */
     void Split(unsigned separator, Vector<UTF8String>& ret) const;

    /**
     * Split a string by a separator char.
     * @param str string to separate
     * @param separator bounding character
     * @return vector of resulting substrings
     */
    static Vector<UTF8String> Split(const char* str, char separator) {return Split(str, (unsigned)(byte)separator);}

    /**
     * Split this string by a separator char.
     * @param separator bounding character
     * @return vector of resulting substrings
     */
    Vector<UTF8String> Split(char separator) const {return Split((unsigned)(byte)separator);}

    /**
     * Split a string by a separator char.
     * @param str string to separate
     * @param separator bounding character
     * @param ret vector to store resulting substrings
     */
    static void Split(const char* str, char separator, Vector<UTF8String>& ret) {Split(str, (unsigned)(byte)separator, ret);}

    /**
     * Split this string by a separator char.
     * @param separator bounding character
     * @param ret vector to store resulting substrings
     */
    void Split(char separator, Vector<UTF8String>& ret) const {Split((unsigned)(byte)separator, ret);}

    /**
     * Replace all occurrences of a character.
     * @param replaceThis char to be replaced
     * @param replaceWith char to replace with
     * @param caseSensitive whether char to be replaced capitalization's matters
     */
    void Replace(char replaceThis, char replaceWith, bool caseSensitive = true) { Replace((unsigned)(byte)replaceThis, (unsigned)(byte)replaceWith, caseSensitive);}

    /**
     * Replace all occurrences of a character.
     * @param replaceThis char to be replaced
     * @param replaceWith char to replace with
     * @param caseSensitive whether char to be replaced capitalization's matters
     */
    void Replace(unsigned replaceThis, unsigned replaceWith, bool caseSensitive = true);

    /**
     * Replace all occurrences of a string.
     * @param replaceThis string to be replaced
     * @param replaceWith string to replace with
     * @param caseSensitive whether string to be replaced capitalization's matters
     */
    void Replace(const char* replaceThis, const char*replaceWith, bool caseSensitive = true)
    {
        UTF8String rThis(replaceThis), rWith(replaceWith);
        Replace(rThis, rWith, caseSensitive);
    }

    /**
     * Replace all occurrences of a string.
     * @param replaceThis string to be replaced
     * @param replaceWith string to replace with
     * @param caseSensitive whether string to be replaced capitalization's matters
     */
    void Replace(const UTF8String& replaceThis, const UTF8String& replaceWith, bool caseSensitive = true);

    /**
     * Replace a substring.
     * @param pos index to start replacing
     * @param length length to be replaced
     * @param replaceWith string to replace with
     */
    void Replace(unsigned pos, unsigned length, const UTF8String& replaceWith);

    /**
     * Replace a substring.
     * @param pos index to start replacing
     * @param length length to be replaced
     * @param replaceWith string to replace with
     */
    void Replace(unsigned pos, unsigned length, const char* replaceWith)
    {
        UTF8String rWith(replaceWith);
        Replace(pos, length, rWith);
    }

    /**
     * Replace Unicode character at index from UTF8 content.
     * @param index index of character to be replaced
     * @param unicodeChar characer to replace with
     */
    void Replace(int index, unsigned unicodeChar);

    /**
     * Replace character at index from UTF8 content.
     * @param index index of character to be replaced
     * @param unicodeChar characer to replace with
     */
    void Replace(int index, char unicodeChar) { Replace(index,(unsigned)(byte)unicodeChar); }

    /**
     * Append a character.
     * @param c char to append
     * @return resulting string
     */
    UTF8String& Append(unsigned unicodeChar);

    /**
     * Append Unicode character at the end as UTF8.
     * @param c char to append
     * @return resulting string
     */
    UTF8String& Append(char unicodeChar) { return Append((unsigned)(byte)unicodeChar);}

    /**
     * Append C string at the end as UTF8.
     * @param str string to append
     * @return resulting string
     */
    UTF8String& Append(const char* str);

    /**
     * Append wide string at the end as UTF8.
     * @param str string to append
     * @return resulting string
     */
    UTF8String& Append(const wchar_t* str);

    /**
     * Append wide string at the end as UTF8.
     * @param str string to append
     * @return resulting string
     */
    UTF8String& Append(const WString str);

    /**
     * Append string at the end as UTF8.
     * @param str string to append
     * @return resulting string
     */
    UTF8String& Append(const String str);

    /**
     * Append UTF8String at the end.
     * @param str string to append
     * @return resulting string
     */
    UTF8String& Append(const UTF8String str);

    /**
     * Append UTF8 byte at the end.
     * @param b byte to append
     * @return resulting string
     */
    UTF8String& Append(const byte b) {return AuxAppend(UTF8String((char)b).CString());}

    /**
     * Return a UTF8 substring from position to end.
     * @param pos index of the beginning of the substring
     * @return resulting substring
     */
    UTF8String Substring(unsigned pos) const;

    /**
     * Return a UTF8 substring with length from position.
     * @param pos index of the beginning of the substring
     * @param length number of chars of the substring
     * @return resulting substring
     */
    UTF8String Substring(unsigned pos, unsigned length) const;

    /**
     * Get C string.
     * @return the C string
     */
    const char* CString() const { return buffer; }

    /**
     * Get buffer pointer.
     * @return pointer to buffer
     */
    char* CStringPtr() { return buffer; }

    /// Clear the string.
    void Clear();

    /**
     * Erase a substring.
     * @param pos index to star erasing from
     * @param length number of characters to erase, default is 1
     */
    void Erase(unsigned pos, unsigned length = 1);

    /**
     * Compare two C strings.
     * @param str1 first string to compare
     * @param str2 second string to compare
     * @param caseSensitive whether comparison is case sensitive
     * @return value of comparison, 0 if equal, <0 if str1 < str2, >0 otherwise
     */
    static int UTF8Compare(const char* str1, const char* str2, bool caseSensitive);

    /**
     * Compare this string to another string.
     * @param str string to compare to
     * @param caseSensitive check if comparison is case sensitive
     * @return comparison result with a string
     */
    int Compare(const UTF8String& str, bool caseSensitive = true) const;

    /**
     * Compare this string to a C string.
     * @param str string to compare to
     * @param caseSensitive check if comparison is case sensitive
     * @return comparison result with a C string
     */
    int Compare(const char* str, bool caseSensitive = true) const;

    /**
     * Hash string.
     * @return hash value for HashSet & HashMap
     */
    unsigned ToHash() const
    {
        unsigned hash = 0;
        const char* ptr = buffer;
        while (*ptr)
        {
            hash = *ptr + (hash << 6) + (hash << 16) - hash;
            ++ptr;
        }

        return hash;
    }

    /**
     * Return string in uppercase.
     * @return resulting string
     */
    UTF8String ToUpper() const;

    /**
     * Return string in lowercase.
     * @return resulting string
     */
    UTF8String ToLower() const;

    /**
     * Resize the string byte size.
     * @param newLength new byte length of the string
     */
    void ByteResize(unsigned newLength) { Resize(newLength); }

    /**
     * Get string capacity.
     * @return buffer capacity.
     */
    unsigned Capacity() const { return capacity; }

    /**
     * Check if string is empty.
     * @return whether the string is empty
     */
    bool Empty() const { return length == 0; }

    /**
     * Encode Unicode character to UTF8. Pointer will be incremented.
     * @param dest pointer to where character will be encoded
     * @param unicodeChar char to be encoded
     */
    static void EncodeUTF8(char*& dest, unsigned unicodeChar);

    /**
     * Decode Unicode character from UTF8. Pointer will be incremented.
     * @param src pointer to character to be decoded
     * @return decoded character in Unicode
     */
    static unsigned DecodeUTF8(const char*& src);

    static const unsigned NPOS = 0xffffffff; //!< Position for "not found."
    static const UTF8String EMPTY; //!< Empty string.

    #ifdef PLATFORM_WINDOWS

    /**
     * Encode Unicode character to UTF16. Pointer will be incremented.
     * @param dest pointer to where character will be encoded
     * @param unicodeChar char to be encoded
     */
     static void EncodeUTF16(wchar_t*& dest, unsigned unicodeChar);

    /**
     * Decode Unicode character from UTF16. Pointer will be incremented.
     * @param src pointer to character to be decoded
     * @return decoded character in Unicode
     */
    static unsigned DecodeUTF16(const wchar_t*& src);
    #endif

private:

    /// Resize the string.
    void Resize(unsigned newLength);

    /// Replace a substring with another substring.
    void AuxReplace(unsigned pos, unsigned length, const char* srcStart, unsigned srcLength);

    /// Move a range of characters within the string.
    void MoveRange(unsigned dest, unsigned src, unsigned count)
    {
        if (count)
            memmove(buffer + dest, buffer + src, count);
    }
    
    /// Copy chars from one buffer to another.
    static void CopyChars(char* dest, const char* src, unsigned count)
    {
        #ifdef _MSC_VER
        if (count)
            memcpy(dest, src, count);
        #else
        char* end = dest + count;
        while (dest != end)
        {
            *dest = *src;
            ++dest;
            ++src;
        }
        #endif
    }

    /// Return length of a C string.
    static unsigned CStringLength(const char* str)
    {
        if (!str)
            return 0;
        #ifdef _MSC_VER
        return (unsigned)strlen(str);
        #else
        const char* ptr = str;
        while (*ptr)
            ++ptr;
        return ptr - str;
        #endif
    }

    /// Append a C string.
    UTF8String& AuxAppend(const char* str);

    /// String length.
    unsigned length;

    /// Capacity, zero if buffer not allocated.
    unsigned capacity;

    /// String buffer, null if not allocated.
    char* buffer;

    static char endZero;

    /// Initial dynamic allocation size.
    static const unsigned MIN_CAPACITY = 8;


};

/**
 * Assigns a string a string from an input stream.
 * @param is input stream to extract string from
 * @param rhs string to assign to
 * @return input stream
 */
inline std::istream& operator>> (std::istream& is, UTF8String& rhs)
{
    return is >> rhs.CStringPtr();
}

/**
 * Outputs a string into an output stream.
 * @param os output stream to output string to
 * @param rhs string to output
 * @return output stream
 */
inline std::ostream& operator<< (std::ostream& os, const UTF8String& rhs)
{
    return os << rhs.CString();
}

NAMESPACE_CORE_END
