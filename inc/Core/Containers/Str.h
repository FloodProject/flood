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
#include "Core/Containers/Vector.h"

#include <cstring>
#include <cstdarg>
#include <cctype>
#include <iostream>

NAMESPACE_CORE_BEGIN

static const int CONVERSION_bufferLENGTH = 128;

class WString;

/// %String class.
class API_CORE String
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
    
    /// Construct from another string.
    String(const String& str) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = str;
    }
	/// Move construct from another string.
    String(String&& str) :
		length(str.length),
        capacity(str.capacity),
        buffer(str.buffer)
    {
		str.buffer = nullptr;
		str.capacity = 0;
		str.length = 0;
    }
    
    /// Construct from a C string.
    String(const char* str) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = str;
    }
    
    /// Construct from a C string.
    String(char* str) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        *this = (const char*)str;
    }
    
    /// Construct from a char array and length.
    String(const char* str, unsigned length) :
        length(0),
        capacity(0),
        buffer(&endZero)
    {
        Resize(length);
        CopyChars(buffer, str, length);
    }
    

    
    /// Construct from an integer.
    explicit String(int value);
    /// Construct from a short integer.
    explicit String(short value);
    /// Construct from a long integer.
    explicit String(long value);
    /// Construct from a long long integer.
    explicit String(long long value);
    /// Construct from an unsigned integer.
    explicit String(unsigned value);
    /// Construct from an unsigned short integer.
    explicit String(unsigned short value);
    /// Construct from an unsigned long integer.
    explicit String(unsigned long value);
    /// Construct from an unsigned long long integer.
    explicit String(unsigned long long value);
    /// Construct from a float.
    explicit String(float value);
    /// Construct from a double.
    explicit String(double value);
    /// Construct from a bool.
    explicit String(bool value);
    /// Construct from a character.
    explicit String(char value);
    /// Construct from a character and fill length.
    explicit String(char value, unsigned length);
    
    /// Construct from a convertable value.
    template <class T> explicit String(const T& value) :
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
    
    /// Assign a string.
    String& operator = (const String& rhs)
    {
        Resize(rhs.length);
        CopyChars(buffer, rhs.buffer, rhs.length);
        
        return *this;
    }
	/// Move assign a string.
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
    
    /// Assign a C string.
    String& operator = (const char* rhs)
    {
        unsigned rhsLength = CStringLength(rhs);
        Resize(rhsLength);
        CopyChars(buffer, rhs, rhsLength);
        
        return *this;
    }
    
    /// Add-assign a string.
    String& operator += (const String& rhs)
    {
        unsigned oldLength = length;
        Resize(length + rhs.length);
        CopyChars(buffer + oldLength, rhs.buffer, rhs.length);
        
        return *this;
    }
    
    /// Add-assign a C string.
    String& operator += (const char* rhs)
    {
        unsigned rhsLength = CStringLength(rhs);
        unsigned oldLength = length;
        Resize(length + rhsLength);
        CopyChars(buffer + oldLength, rhs, rhsLength);
        
        return *this;
    }
    
    /// Add-assign a character.
    String& operator += (char rhs)
    {
        unsigned oldLength = length;
        Resize(length + 1);
        buffer[oldLength]  = rhs;
        
        return *this;
    }
    
    /// Add-assign an integer.
    String& operator += (int rhs);
    /// Add-assign a short integer.
    String& operator += (short rhs);
    /// Add-assign an unsigned integer.
    String& operator += (unsigned rhs);
    /// Add-assign a short unsigned integer.
    String& operator += (unsigned short rhs);
    /// Add-assign a long long.
	String& operator += (long long rhs);
    /// Add-assign a float.
    String& operator += (float rhs);    
	/// Add-assign a double.
    String& operator += (double rhs);
    /// Add-assign a bool.
    String& operator += (bool rhs);
    /// Add-assign an arbitraty type.
    template <class T> String operator += (const T& rhs) { return *this += rhs.ToString(); }
    
    /// Add a string.
    String operator + (const String& rhs) const
    {
        String ret;
        ret.Resize(length + rhs.length);
        CopyChars(ret.buffer, buffer, length);
        CopyChars(ret.buffer + length, rhs.buffer, rhs.length);
        
        return ret;
    }
    
    /// Add a C string.
    String operator + (const char* rhs) const
    {
        unsigned rhsLength = CStringLength(rhs);
        String ret;
        ret.Resize(length + rhsLength);
        CopyChars(ret.buffer, buffer, length);
        CopyChars(ret.buffer + length, rhs, rhsLength);
        
        return ret;
    }
    
    /// Add a character.
    String operator + (char rhs) const
    {
        String ret(*this);
        ret += rhs;
        
        return ret;
    }

    /// Test for equality with another string.
    bool operator == (const String& rhs) const { return strcmp(CString(), rhs.CString()) == 0; }
    /// Test for inequality with another string.
    bool operator != (const String& rhs) const { return strcmp(CString(), rhs.CString()) != 0; }
    /// Test if string is less than another string.
    bool operator < (const String& rhs) const { return strcmp(CString(), rhs.CString()) < 0; }
	/// Test if string is less or equal to another string.
    bool operator <= (const String& rhs) const { return strcmp(CString(), rhs.CString()) <= 0; }
    /// Test if string is greater than another string.
    bool operator > (const String& rhs) const { return strcmp(CString(), rhs.CString()) > 0; }
	/// Test if string is greater or equal to another string.
    bool operator >= (const String& rhs) const { return strcmp(CString(), rhs.CString()) >= 0; }
    /// Test for equality with a C string.
    bool operator == (const char* rhs) const { return strcmp(CString(), rhs) == 0; }
    /// Test for inequality with a C string.
    bool operator != (const char* rhs) const { return strcmp(CString(), rhs) != 0; }
    /// Test if string is less than a C string.
    bool operator < (const char* rhs) const { return strcmp(CString(), rhs) < 0; }
    /// Test if string is less or equal to a C string.
    bool operator <= (const char* rhs) const { return strcmp(CString(), rhs) <= 0; }
    /// Test if string is greater than a C string.
    bool operator > (const char* rhs) const { return strcmp(CString(), rhs) > 0; }
    /// Test if string is greater or equal to a C string.
    bool operator >= (const char* rhs) const { return strcmp(CString(), rhs) >= 0; }
    /// Return char at index.
    char& operator [] (unsigned index) { assert(index < length); return buffer[index]; }
    /// Return const char at index.
    const char& operator [] (unsigned index) const { assert(index < length); return buffer[index]; }
    /// Return char at index.
    char& At(unsigned index) { assert(index < length); return buffer[index]; }
    /// Return const char at index.
    const char& At(unsigned index) const { assert(index < length); return buffer[index]; }
    
	std::istream& operator>> (std::istream& is);
	std::ostream& operator<< (std::ostream& os);


    /// Replace all occurrences of a character.
    void Replace(char replaceThis, char replaceWith, bool caseSensitive = true);
    /// Replace all occurrences of a string.
    void Replace(const String& replaceThis, const String& replaceWith, bool caseSensitive = true);
    /// Replace a substring.
    void Replace(unsigned pos, unsigned length, const String& replaceWith);
    /// Replace a substring with a C string.
    void Replace(unsigned pos, unsigned length, const char* replaceWith);
    /// Replace a substring by iterators.
    Iterator Replace(const Iterator& start, const Iterator& end, const String& replaceWith);
    /// Return a string with all occurrences of a character replaced.
    String Replaced(char replaceThis, char replaceWith, bool caseSensitive = true) const;
    /// Return a string with all occurrences of a string replaced.
    String Replaced(const String& replaceThis, const String& replaceWith, bool caseSensitive = true) const;
    /// Append a string.
    String& Append(const String& str);
    /// Append a C string.
    String& Append(const char* str);
    /// Append a character.
    String& Append(char c);
    /// Append characters.
    String& Append(const char* str, unsigned length);
    /// Insert a string.
    void Insert(unsigned pos, const String& str);
    /// Insert a character.
    void Insert(unsigned pos, char c);
    /// Insert a string by iterator.
    Iterator Insert(const Iterator& dest, const String& str);
    /// Insert a string partially by iterators.
    Iterator Insert(const Iterator& dest, const Iterator& start, const Iterator& end);
    /// Insert a character by iterator.
    Iterator Insert(const Iterator& dest, char c);
    /// Erase a substring.
    void Erase(unsigned pos, unsigned length = 1);
    /// Erase a character by iterator.
    Iterator Erase(const Iterator& it);
    /// Erase a substring by iterators.
    Iterator Erase(const Iterator& start, const Iterator& end);
    /// Resize the string.
    void Resize(unsigned newLength);
    /// Set new capacity.
    void Reserve(unsigned newCapacity);
    /// Reallocate so that no extra memory is used.
    void Compact();
    /// Clear the string.
    void Clear();
    /// Swap with another string.
    void Swap(String& str);
    
    /// Return iterator to the beginning.
    Iterator Begin() { return Iterator(buffer); }
    /// Return const iterator to the beginning.
    ConstIterator Begin() const { return ConstIterator(buffer); }
    /// Return iterator to the end.
    Iterator End() { return Iterator(buffer + length); }
    /// Return const iterator to the end.
    ConstIterator End() const { return ConstIterator(buffer + length); }
    /// Return first char, or 0 if empty.
    char Front() const { return buffer[0]; }
    /// Return last char, or 0 if empty.
    char Back() const { return length ? buffer[length - 1] : buffer[0]; }
    /// Return a substring from position to end.
    String Substring(unsigned pos) const;
    /// Return a substring with length from position.
    String Substring(unsigned pos, unsigned length) const;
    /// Return string with whitespace trimmed from the beginning and the end.
    String Trimmed() const;
    /// Return string in uppercase.
    String ToUpper() const;
    /// Return string in lowercase.
    String ToLower() const;
    /// Return substrings split by a separator char.
    Vector<String> Split(char separator) const;
    /// Join substrings with a 'glue' string.
    void Join(const Vector<String>& subStrings, String glue);
    /// Return index to the first occurrence of a string, or NPOS if not found.
    unsigned Find(const String& str, unsigned startPos = 0, bool caseSensitive = true) const;
    /// Return index to the first occurrence of a character, or NPOS if not found.
    unsigned Find(char c, unsigned startPos = 0, bool caseSensitive = true) const;
    /// Return index to the last occurrence of a string, or NPOS if not found.
    unsigned FindLast(const String& str, unsigned startPos = NPOS, bool caseSensitive = true) const;
    /// Return index to the last occurrence of a character, or NPOS if not found.
    unsigned FindLast(char c, unsigned startPos = NPOS, bool caseSensitive = true) const;
    /// Return whether starts with a string.
    bool StartsWith(const String& str, bool caseSensitive = true) const;
    /// Return whether ends with a string.
    bool EndsWith(const String& str, bool casSensitive = true) const;
    /// Return the C string.
    const char* CString() const { return buffer; }
	/// Return the pointer to the C string.
    char* CStringPtr() { return buffer; }
    /// Return length.
    unsigned Length() const { return length; }
    /// Return buffer caspacity.
    unsigned Capacity() const { return capacity; }
    /// Return whether the string is empty.
    bool Empty() const { return length == 0; }
    /// Return comparision result with a string.
    int Compare(const String& str, bool caseSensitive = true) const;
    /// Return comparision result with a C string.
    int Compare(const char* str, bool caseSensitive = true) const;
    /// Return whether contains a specific occurences of string.
    bool Contains(const String& str) const { return Find(str) != NPOS; }
    /// Return whether contains a specific character.
    bool Contains(char c) const { return Find(c) != NPOS; }

    /// Return hash value for HashSet & HashMap.
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
    
    /// Return substrings split by a separator char.
    static Vector<String> Split(const char* str, char separator);
    /// Return a string by joining substrings with a 'glue' string.
    static String Joined(const Vector<String>& subStrings, String glue);


    
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
    
    /// Append to string using formatting.
    String& AppendWithFormat(const char* formatString, ... );
    /// Append to string using variable arguments.
    String& AppendWithFormatArgs(const char* formatString, va_list args);
    
    /// Compare two C strings.
    static int Compare(const char* str1, const char* str2, bool caseSensitive);
    
    /// Position for "not found."
    static const unsigned NPOS = 0xffffffff;
    /// Initial dynamic allocation size.
    static const unsigned MIN_CAPACITY = 8;
    /// Empty string.
    static const String EMPTY;
    
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

/// Add a string to a C string.
inline String operator + (const char* lhs, const String& rhs)
{
    String ret(lhs);
    ret += rhs;
    return ret;
}

/// Compares a C string to a string.
inline bool operator == (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) == 0;
}

/// Compares a C string to a string.
inline bool operator != (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) != 0;
}

/// Compares a C string to a string.
inline bool operator < (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) >= 0;
}

/// Compares a C string to a string.
inline bool operator >= (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) < 0;
}

/// Compares a C string to a string.
inline bool operator <= (const char* lhs, const String& rhs)
{
    return rhs.Compare(lhs) > 0;
}

inline std::istream& operator>> (std::istream& is, String& rhs)
{
	return is >> rhs.CStringPtr();
}

inline std::ostream& operator<< (std::ostream& os, const String& rhs)
{
	return os << rhs.CString();
}
class UTF8String;





/// Wide character string. Only meant for converting from String and passing to the operating system where necessary.
class API_CORE WString
{
public:
    /// Construct empty.
    WString();
    /// Construct from a wchar_t pointer.
    WString(const wchar_t* str);
    /// Construct from a string.
    WString(const UTF8String& str);
    /// Destruct.
    ~WString();
    
    /// Return char at index.
    wchar_t& operator [] (unsigned index) { assert(index < length); return buffer[index]; }
    /// Return const char at index.
    const wchar_t& operator [] (unsigned index) const { assert(index < length); return buffer[index]; }
    /// Return char at index.
    wchar_t& At(unsigned index) { assert(index < length); return buffer[index]; }
    /// Return const char at index.
    const wchar_t& At(unsigned index) const { assert(index < length); return buffer[index]; }
    /// Resize the string.
    void Resize(unsigned newSize);
    /// Return whether the string is empty.
    bool Empty() const { return length == 0; }
    /// Return length.
    unsigned Length() const { return length; }
    unsigned Bytes() const { return bytes; }
    /// Return character data.
    const wchar_t* CString() const { return buffer; }
    
private:
    /// String length.
    unsigned length;
    /// String buffer, null if not allocated.
    wchar_t* buffer;
	unsigned bytes;
};

class API_CORE UTF8String
{
public:

	UTF8String();
    /// Construct from a null-terminated wide character array.
    UTF8String(wchar_t* str); 
    /// Construct from a wide character string.
    UTF8String(const WString& str);
	/// Construct UTF8 content from Latin1.
    UTF8String(const char* str, bool isUTF8 = false);
	/// Construct UTF8 content from String.
	UTF8String(const String& str);
	/// Construct UTF8 content from UTF8String.
	UTF8String(const UTF8String& str);
	
	UTF8String(UTF8String&& str);
    /// Construct UTF8 content from wide characters.
    UTF8String(const wchar_t* str);
	
	/// Construct from a char array and length.
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

	/// Construct from an integer.
    explicit UTF8String(int value);
    /// Construct from a short integer.
    explicit UTF8String(short value);
    /// Construct from a long integer.
    explicit UTF8String(long value);
    /// Construct from a long long integer.
    explicit UTF8String(long long value);
    /// Construct from an unsigned integer.
    explicit UTF8String(unsigned value);
    /// Construct from an unsigned short integer.
    explicit UTF8String(unsigned short value);
    /// Construct from an unsigned long integer.
    explicit UTF8String(unsigned long value);
    /// Construct from an unsigned long long integer.
    explicit UTF8String(unsigned long long value);
    /// Construct from a float.
    explicit UTF8String(float value);
    /// Construct from a double.
    explicit UTF8String(double value);
    /// Construct from a bool.
    explicit UTF8String(bool value);
    /// Construct from a character.
    explicit UTF8String(char value);
    /// Construct from a character and fill length.
    explicit UTF8String(char value, unsigned length);
    
    /// Construct from a convertable value.
    template <class T> explicit UTF8String(const T& value) :
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

    /// Assign a string.
    UTF8String& operator = (const String& rhs)
    {
		SetUTF8FromLatin1(rhs.CString());
        return *this;
    }

	/// Assign a string.
    UTF8String& operator = (const UTF8String& rhs)
    {
        Resize(rhs.length);
        CopyChars(buffer, rhs.buffer, rhs.length);
        
        return *this;
    }

	/// Move assign a string.
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
    
    UTF8String& operator = (const char* rhs)
    {
		SetUTF8FromLatin1(rhs);
        return *this;
    }

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

	/// Add-assign a utf8 string.
    UTF8String& operator += (const UTF8String& rhs)
    {
		return Append(rhs);
    }

	/// Add-assign a string.
    UTF8String& operator += (const String& rhs)
    {
		return Append(rhs);
    }
    
    /// Add-assign a C string.
    UTF8String& operator += (const char* rhs)
    {
		return Append(rhs);
    }
    
    /// Add-assign a character.
    UTF8String& operator += (char rhs)
    {
		return Append(rhs);
    }

	/// Add-assign an integer.
    UTF8String& operator += (int rhs);
    /// Add-assign a short integer.
    UTF8String& operator += (short rhs);
    /// Add-assign an unsigned integer.
    UTF8String& operator += (unsigned rhs);
    /// Add-assign a short unsigned integer.
    UTF8String& operator += (unsigned short rhs);
    /// Add-assign a long long.
	UTF8String& operator += (long long rhs);
    /// Add-assign a float.
    UTF8String& operator += (float rhs);    
	/// Add-assign a double.
    UTF8String& operator += (double rhs);
    /// Add-assign a bool.
    UTF8String& operator += (bool rhs);
    /// Add-assign an arbitraty type.
    template <class T> UTF8String operator += (const T& rhs) { return *this += rhs.ToString(); }

	    /// Add a string.
    UTF8String operator + (const UTF8String& rhs) const
    {
        UTF8String ret;
        ret.Resize(length + rhs.length);
        CopyChars(ret.buffer, buffer, length);
        CopyChars(ret.buffer + length, rhs.buffer, rhs.length);
        
        return ret;
    }
    
    /// Add a C string.
    UTF8String operator + (const char* rhs) const
    {
        UTF8String ret(*this);
        ret += rhs;
        
        return ret;    
	}
    
    /// Add a character.
    UTF8String operator + (char rhs) const
    {
        UTF8String ret(*this);
        ret += rhs;
        
        return ret;
    }

	/// Test for equality with another string.
    bool operator == (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) == 0; }
    /// Test for inequality with another string.
    bool operator != (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) != 0; }
    /// Test if string is less than another string.
    bool operator < (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) < 0; }
	/// Test if string is less or equal to another string.
    bool operator <= (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) <= 0; }
    /// Test if string is greater than another string.
    bool operator > (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) > 0; }
	/// Test if string is greater or equal to another string.
    bool operator >= (const UTF8String& rhs) const { return strcmp(CString(), rhs.CString()) >= 0; }
    /// Test for equality with a C string.
	bool operator == (const char* rhs) const 
	{ 
		auto str = GetUTF8FromLatin1(rhs);
		auto cmp = str.CString(); 
		return strcmp(buffer, cmp) == 0; 
	}
    /// Test for inequality with a C string.
    bool operator != (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) != 0; }
    /// Test if string is less than a C string.
    bool operator < (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) < 0; }
    /// Test if string is less or equal to a C string.
    bool operator <= (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) <= 0; }
    /// Test if string is greater than a C string.
    bool operator > (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) > 0; }
    /// Test if string is greater or equal to a C string.
    bool operator >= (const char* rhs) const { return strcmp(CString(), GetUTF8FromLatin1(rhs).CString()) >= 0; }
    /// Return const char at index.
    const unsigned operator [] (unsigned index) const { return At(index); }
    
	std::istream& operator>> (std::istream& is);
	std::ostream& operator<< (std::ostream& os);

	/// Construct UTF8 content from Latin1.
    void SetUTF8FromLatin1(const char* str);
	/// Get UTF8 content from Latin1.
	UTF8String GetUTF8FromLatin1(const char* str) const;
    /// Construct UTF8 content from wide characters.
    void SetUTF8FromWChar(const wchar_t* str);
    /// Calculate number of characters in UTF8 content.
    
	unsigned Length() const;
	unsigned ByteLength() const { return length; };
    /// Return byte offset to char in UTF8 content.
    unsigned ByteOffset(unsigned index) const;
    /// Return next Unicode character from UTF8 content and increase byte offset.
    unsigned NextChar(unsigned& byteOffset) const;

	/// Return a string by joining substrings with a 'glue' string.
    static UTF8String Joined(const Vector<UTF8String>& subStrings, UTF8String glue);
	/// Return a string by joining substrings with a 'glue' string.
	static UTF8String Joined(const Vector<UTF8String>& subStrings, const char* glue)
	{
		UTF8String g(glue);
		return Joined(subStrings, g);
	}

	/// Return string with whitespace trimmed from the beginning and the end.
    UTF8String Trimmed() const;
    /// Join substrings with a 'glue' string.
    void Join(const Vector<UTF8String>& subStrings, UTF8String glue);
    /// Join substrings with a 'glue' string.
	void Join(const Vector<UTF8String>& subStrings, const char* glue)
	{
		UTF8String g(glue);
		Join(subStrings, g);
	}
    /// Return index to the first occurrence of a string, or NPOS if not found.
    unsigned Find(const UTF8String& str, bool caseSensitive = true, int startPos = 0) const;
    /// Return index to the first occurrence of a string, or NPOS if not found.
	unsigned Find(const char* str, bool caseSensitive = true, int startPos = 0) const 
	{
		UTF8String t(str);
		return Find(t, caseSensitive, startPos);
	}
    /// Return index to the first occurrence of a character, or NPOS if not found.
    unsigned Find(unsigned c, bool caseSensitive = true, int startPos = 0) const;
	/// Return index to the first occurrence of a character, or NPOS if not found.
    unsigned Find(char c, bool caseSensitive = true, int startPos = 0) const{return Find((unsigned)(byte)c, caseSensitive, startPos);}
    /// Return index to the last occurrence of a string, or NPOS if not found.
    unsigned FindLast(const UTF8String& str, bool caseSensitive = true, int startPos = -1) const;
    /// Return index to the last occurrence of a character, or NPOS if not found.
    unsigned FindLast(unsigned c, bool caseSensitive = true, int startPos = -1) const;
	/// Return index to the last occurrence of a character, or NPOS if not found.
    unsigned FindLast(char c, bool caseSensitive = true, int startPos = -1) const { return FindLast((unsigned)(byte)c, caseSensitive, startPos); }
	unsigned FindLast(const char* str, bool caseSensitive = true, int startPos = -1) const 
	{
		UTF8String t(str);
		return FindLast(t, caseSensitive, startPos);
	}

	unsigned CharOffset(char utf8Char);
    /// Return Unicode character at index from UTF8 content.
    unsigned At(unsigned index) const;
        
	/// Return substrings split by a separator char.
    static Vector<UTF8String> Split(const char* str, unsigned separator);
	/// Return substrings split by a separator char.
    Vector<UTF8String> Split(unsigned separator) const;	
	/// Return substrings split by a separator char.
    static void Split(const char* str, unsigned separator, Vector<UTF8String>& ret);
	/// Return substrings split by a separator char.
     void Split(unsigned separator, Vector<UTF8String>& ret) const;
	
	 /// Return substrings split by a separator char.
    static Vector<UTF8String> Split(const char* str, char separator) {return Split(str, (unsigned)(byte)separator);}
	/// Return substrings split by a separator char.
    Vector<UTF8String> Split(char separator) const {return Split((unsigned)(byte)separator);}
	/// Return substrings split by a separator char.
    static void Split(const char* str, char separator, Vector<UTF8String>& ret) {Split(str, (unsigned)(byte)separator, ret);}
	/// Return substrings split by a separator char.
	void Split(char separator, Vector<UTF8String>& ret) const {Split((unsigned)(byte)separator, ret);}

	/// Replace all occurrences of a character.
	void Replace(char replaceThis, char replaceWith, bool caseSensitive = true) { Replace((unsigned)(byte)replaceThis, (unsigned)(byte)replaceWith, caseSensitive);}
	/// Replace all occurrences of a character.
    void Replace(unsigned replaceThis, unsigned replaceWith, bool caseSensitive = true);
    /// Replace all occurrences of a string.
    void Replace(const char* replaceThis, const char*replaceWith, bool caseSensitive = true)
	{
		UTF8String rThis(replaceThis), rWith(replaceWith);
		Replace(rThis, rWith, caseSensitive);
	}
    /// Replace all occurrences of a string.
    void Replace(const UTF8String& replaceThis, const UTF8String& replaceWith, bool caseSensitive = true);
    /// Replace a substring.
    void Replace(unsigned pos, unsigned length, const UTF8String& replaceWith);
    /// Replace a substring.
    void Replace(unsigned pos, unsigned length, const char* replaceWith)
	{
		UTF8String rWith(replaceWith);
		Replace(pos, length, rWith);
	}
	/// Replace Unicode character at index from UTF8 content.
    void Replace(int index, unsigned unicodeChar);
	/// Replace Unicode character at index from UTF8 content.
	void Replace(int index, char unicodeChar) { Replace(index,(unsigned)(byte)unicodeChar); }
    /// Append Unicode character at the end as UTF8.
    UTF8String& Append(unsigned unicodeChar);
	/// Append Unicode character at the end as UTF8.
	UTF8String& Append(char unicodeChar) { return Append((unsigned)(byte)unicodeChar);}
    /// Append C string at the end as UTF8.
    UTF8String& Append(const char* str);
	/// Append wide string at the end as UTF8.
    UTF8String& Append(const wchar_t* str);
	/// Append wide string at the end as UTF8.
    UTF8String& Append(const WString str);
    /// Append string at the end as UTF8.
    UTF8String& Append(const String str);
	/// Append UTF8string at the end.
    UTF8String& Append(const UTF8String str);
    /// Return a UTF8 substring from position to end.
    UTF8String Substring(unsigned pos) const;
    /// Return a UTF8 substring with length from position.
    UTF8String Substring(unsigned pos, unsigned length) const;
	/// Return the C string.
    const char* CString() const { return buffer; }
	/// Return the pointer to the C string.
    char* CStringPtr() { return buffer; }
    /// Clear the string.
    void Clear();

    /// Return hash value for HashSet & HashMap.
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

    /// Return string in uppercase.
    UTF8String ToUpper() const;
    /// Return string in lowercase.
    UTF8String ToLower() const;

    /// Return buffer caspacity.
    unsigned Capacity() const { return capacity; }
    /// Return whether the string is empty.
    bool Empty() const { return length == 0; }

	/// Encode Unicode character to UTF8. Pointer will be incremented.
    static void EncodeUTF8(char*& dest, unsigned unicodeChar);
    /// Decode Unicode character from UTF8. Pointer will be incremented.
    static unsigned DecodeUTF8(const char*& src);

	/// Position for "not found."
    static const unsigned NPOS = 0xffffffff;

	#ifdef PLATFORM_WINDOWS
    /// Encode Unicode character to UTF16. Pointer will be incremented.
    static void EncodeUTF16(wchar_t*& dest, unsigned unicodeChar);
    /// Decode Unicode character from UTF16. Pointer will be incremented.
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

inline std::istream& operator>> (std::istream& is, UTF8String& rhs)
{
	return is >> rhs.CStringPtr();
}

inline std::ostream& operator<< (std::ostream& os, const UTF8String& rhs)
{
	return os << rhs.CString();
}

NAMESPACE_CORE_END
