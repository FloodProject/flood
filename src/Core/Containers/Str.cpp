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

#include "Core/API.h"
#include "Core/Containers/Str.h"
#include "Core/Containers/Swap.h"

#include <cstdio>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif
NAMESPACE_CORE_BEGIN

char String::endZero = 0;

const String String::EMPTY;

String::String(int value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%d", value);
    *this = tempBuffer;
}

String::String(short value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%d", value);
    *this = tempBuffer;
}

String::String(long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%ld", value);
    *this = tempBuffer;
}
String::String(const UTF8String& str):
    length(0),
    capacity(0),
    buffer(&endZero)
{
    *this = str.GetString();
}

String::String(long long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%lld", value);
    *this = tempBuffer;
}

String::String(unsigned value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%u", value);
    *this = tempBuffer;
}

String::String(unsigned short value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%u", value);
    *this = tempBuffer;
}

String::String(unsigned long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%lu", value);
    *this = tempBuffer;
}
    
String::String(unsigned long long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%llu", value);
    *this = tempBuffer;
}

String::String(float value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%g", value);
    *this = tempBuffer;
}

String::String(double value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%g", value);
    *this = tempBuffer;
}

String::String(bool value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    if (value)
        *this = "true";
    else
        *this = "false";
}

String::String(char value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    Resize(1);
    buffer[0] = value;
}

String::String(char value, unsigned length) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    Resize(length);
    for (unsigned i = 0; i < length; ++i)
        buffer[i] = value;
}

String& String::operator += (int rhs)
{
    return *this += String(rhs);
}

String& String::operator += (short rhs)
{
    return *this += String(rhs);
}

String& String::operator += (unsigned rhs)
{
    return *this += String(rhs);
}

String& String::operator += (unsigned short rhs)
{
    return *this += String(rhs);
}
String& String::operator += (long long rhs)
{
    return *this += String(rhs);
}

String& String::operator += (float rhs)
{
    return *this += String(rhs);
}

String& String::operator += (double rhs)
{
    return *this += String(rhs);
}

String& String::operator += (bool rhs)
{
    return *this += String(rhs);
}
String& String::operator += (const UTF8String& rhs)
{
    return *this += rhs.GetString();
}


std::istream& String::operator>> (std::istream& is)
{
    return is >> buffer;
}

std::ostream& String::operator<< (std::ostream& os)
{
    return os << buffer;
}

void String::Replace(char replaceThis, char replaceWith, bool caseSensitive)
{
    if (caseSensitive)
    {
        for (unsigned i = 0; i < length; ++i)
        {
            if (buffer[i] == replaceThis)
                buffer[i] = replaceWith;
        }
    }
    else
    {
        replaceThis = tolower(replaceThis);
        for (unsigned i = 0; i < length; ++i)
        {
            if (tolower(buffer[i]) == replaceThis)
                buffer[i] = replaceWith;
        }
    }
}

void String::Replace(const String& replaceThis, const String& replaceWith, bool caseSensitive)
{
    unsigned nextPos = 0;
    
    while (nextPos < length)
    {
        unsigned pos = Find(replaceThis, nextPos, caseSensitive);
        if (pos == NPOS)
            break;
        Replace(pos, replaceThis.length, replaceWith);
        nextPos = pos + replaceWith.length;
    }
}

void String::Replace(unsigned pos, unsigned length, const String& replaceWith)
{
    /// If substring is illegal, do nothing
    if (pos + length > this->length)
        return;
    
    Replace(pos, length, replaceWith.buffer, replaceWith.length);
}

void String::Replace(unsigned pos, unsigned length, const char* replaceWith)
{
    /// If substring is illegal, do nothing
    if (pos + length > this->length)
        return;
    
    Replace(pos, length, replaceWith, CStringLength(replaceWith));
}

String::Iterator String::Replace(const String::Iterator& start, const String::Iterator& end, const String& replaceWith)
{
    unsigned pos = start - Begin();
    if (pos >= this->length)
        return End();
    unsigned length = end - start;
    Replace(pos, length, replaceWith);
    
    return Begin() + pos;
}

String String::Replaced(char replaceThis, char replaceWith, bool caseSensitive) const
{
    String ret(*this);
    ret.Replace(replaceThis, replaceWith, caseSensitive);
    return ret;
}

String String::Replaced(const String& replaceThis, const String& replaceWith,  bool caseSensitive) const
{
    String ret(*this);
    ret.Replace(replaceThis, replaceWith, caseSensitive);
    return ret;
}

String& String::Append(const String& str)
{
    return *this += str;
}

String& String::Append(const char* str)
{
    return *this += str;
}

String& String::Append(char c)
{
    return *this += c;
}

String& String::Append(const char* str, unsigned length)
{
    if (str)
    {
        unsigned oldLength = this->length;
        Resize(oldLength + length);
        CopyChars(&buffer[oldLength], str, length);
    }
    return *this;
}

void String::Insert(unsigned pos, const String& str)
{
    if (pos > length)
        pos = length;
    
    if (pos == length)
        (*this) += str;
    else
        Replace(pos, 0, str);
}

void String::Insert(unsigned pos, char c)
{
    if (pos > length)
        pos = length;
    
    if (pos == length)
        (*this) += c;
    else
    {
        unsigned oldLength = length;
        Resize(length + 1);
        MoveRange(pos + 1, pos, oldLength - pos);
        buffer[pos] = c;
    }
}

String::Iterator String::Insert(const String::Iterator& dest, const String& str)
{
    unsigned pos = dest - Begin();
    if (pos > length)
        pos = length;
    Insert(pos, str);
    
    return Begin() + pos;
}

String::Iterator String::Insert(const String::Iterator& dest, const String::Iterator& start, const String::Iterator& end)
{
    unsigned pos = dest - Begin();
    if (pos > this->length)
        pos = this->length;
    unsigned length = end - start;
    Replace(pos, 0, &(*start), length);
    
    return Begin() + pos;
}

String::Iterator String::Insert(const String::Iterator& dest, char c)
{
    unsigned pos = dest - Begin();
    if (pos > length)
        pos = length;
    Insert(pos, c);
    
    return Begin() + pos;
}

void String::Erase(unsigned pos, unsigned length)
{
    Replace(pos, length, String::EMPTY);
}

String::Iterator String::Erase(const String::Iterator& it)
{
    unsigned pos = it - Begin();
    if (pos >= length)
        return End();
    Erase(pos);
    
    return Begin() + pos;
}

String::Iterator String::Erase(const String::Iterator& start, const String::Iterator& end)
{
    unsigned pos = start - Begin();
    if (pos >= this->length)
        return End();
    unsigned length = end - start;
    Erase(pos, length);
    
    return Begin() + pos;
}

void String::Resize(unsigned newLength)
{
    if (!capacity)
    {
        // Calculate initial capacity
        capacity = newLength + 1;
        if (capacity < MIN_CAPACITY)
            capacity = MIN_CAPACITY;
        
        buffer = static_cast<char*>(AllocatorAllocate(AllocatorGetHeap(),capacity, 0));
    }
    else
    {
        if (newLength && capacity < newLength + 1)
        {
            // Increase the capacity with half each time it is exceeded
            while (capacity < newLength + 1)
                capacity += (capacity + 1) >> 1;
            
            char* newBuffer = static_cast<char*>(AllocatorAllocate(AllocatorGetHeap(),capacity, 0));
            // Move the existing data to the new buffer, then delete the old buffer
            if (length)
                CopyChars(newBuffer, buffer, length);
            Deallocate(buffer);
            
            buffer = newBuffer;
        }
    }
    buffer[newLength] = 0;
    length = newLength;
}

void String::Reserve(unsigned newCapacity)
{
    if (newCapacity < length + 1)
        newCapacity = length + 1;
    if (newCapacity == capacity)
        return;
    
    char* newBuffer = static_cast<char*>(AllocatorAllocate(AllocatorGetHeap(),newCapacity, 0));
    // Move the existing data to the new buffer, then delete the old buffer
    CopyChars(newBuffer, buffer, length + 1);
    if (capacity)
        Deallocate(buffer);
    
    capacity = newCapacity;
    buffer = newBuffer;
}

void String::Compact()
{
    if (capacity)
        Reserve(length + 1);
}

void String::Clear()
{
    Resize(0);
}

void String::Swap(String& str)
{
    ::Swap(length, str.length);
    ::Swap(capacity, str.capacity);
    ::Swap(buffer, str.buffer);
}

String String::Substring(unsigned pos) const
{
    if (pos >= length)
        return String();

    String ret;
    ret.Resize(length - pos);
    CopyChars(ret.buffer, buffer + pos, ret.length);
        
    return ret;



}

String String::Substring(unsigned pos, unsigned length) const
{
    if (pos >= this->length)
        return String();

    String ret;
    if (pos + length > this->length)
        length = this->length - pos;
    ret.Resize(length);
    CopyChars(ret.buffer, buffer + pos, ret.length);

    return ret;
}

String String::Trimmed() const
{
    unsigned trimStart = 0;
    unsigned trimEnd = length;
    
    while (trimStart < trimEnd)
    {
        char c = buffer[trimStart];
        if (c != ' ' && c != 9)
            break;
        ++trimStart;
    }
    while (trimEnd > trimStart)
    {
        char c = buffer[trimEnd - 1];
        if (c != ' ' && c != 9)
            break;
        --trimEnd;
    }
    
    return Substring(trimStart, trimEnd - trimStart);
}

String String::ToLower() const
{
    String ret(*this);
    for (unsigned i = 0; i < ret.length; ++i)
        ret[i] = tolower(buffer[i]);
    
    return ret;
}

String String::ToUpper() const
{
    String ret(*this);
    for (unsigned i = 0; i < ret.length; ++i)
        ret[i] = toupper(buffer[i]);
    
    return ret;
}

Vector<String> String::Split(char separator) const
{
    return Split(CString(), separator);
}
    
void String::Join(const Vector<String>& subStrings, String glue)
{
    *this = Joined(subStrings, glue);
}

unsigned String::Find(char c, unsigned startPos, bool caseSensitive) const
{
    if (caseSensitive)
    {
        for (unsigned i = startPos; i < length; ++i)
        {
            if (buffer[i] == c)
                return i;
        }
    }
    else
    {
        c = tolower(c);
        for (unsigned i = startPos; i < length; ++i)
        {
            if (tolower(buffer[i]) == c)
                return i;
        }
    }
    
    return NPOS;
}

unsigned String::Find(const String& str, unsigned startPos, bool caseSensitive) const
{
    if (!str.length || str.length > length)
        return NPOS;
    
    char first = str.buffer[0];
    if (!caseSensitive)
        first = tolower(first);

    for (unsigned i = startPos; i <= length - str.length; ++i)
    {
        char c = buffer[i];
        if (!caseSensitive)
            c = tolower(c);

        if (c == first)
        {
            unsigned skip = NPOS;
            bool found = true;
            for (unsigned j = 1; j < str.length; ++j)
            {
                c = buffer[i + j];
                char d = str.buffer[j];
                if (!caseSensitive)
                {
                    c = tolower(c);
                    d = tolower(d);
                }

                if (skip == NPOS && c == first)
                    skip = i + j - 1;

                if (c != d)
                {
                    found = false;
                    if (skip != NPOS)
                        i = skip;
                    break;
                }
            }
            if (found)
                return i;
        }
    }
    
    return NPOS;
}

unsigned String::FindLast(char c, unsigned startPos, bool caseSensitive) const
{
    if (startPos >= length)
        startPos = length - 1;
    
    if (caseSensitive)
    {
        for (int64 i = startPos; i >= 0; --i)
        {
            if (buffer[i] == c)
                return (unsigned)i;
        }
    }
    else
    {
        c = tolower(c);
        for (long long i = startPos; i >= 0; --i)
        {
            if (tolower(buffer[i]) == c)
                return i;
        }
    }
    
    return NPOS;
}

unsigned String::FindLast(const String& str, unsigned startPos, bool caseSensitive) const
{
    if (!str.length || str.length > length)
        return NPOS;
    if (startPos > length - str.length)
        startPos = length - str.length;
    
    char first = str.buffer[0];
    if (!caseSensitive)
        first = tolower(first);

    for (long long i = startPos; i >= 0; --i)
    {
        char c = buffer[i];
        if (!caseSensitive)
            c = tolower(c);

        if (c == first)
        {
            bool found = true;
            for (unsigned j = 1; j < str.length; ++j)
            {
                c = buffer[i + j];
                char d = str.buffer[j];
                if (!caseSensitive)
                {
                    c = tolower(c);
                    d = tolower(d);
                }

                if (c != d)
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
    }
    
    return NPOS;
}

bool String::StartsWith(const String& str, bool caseSensitive) const
{
    return Find(str, 0, caseSensitive) == 0;
}

bool String::EndsWith(const String& str, bool caseSensitive) const
{
    return FindLast(str, Length() - 1, caseSensitive) == Length() - str.Length();
}

int String::Compare(const String& str, bool caseSensitive) const
{
    return Compare(CString(), str.CString(), caseSensitive);
}

int String::Compare(const char* str, bool caseSensitive) const
{
    return Compare(CString(), str, caseSensitive);
}

Vector<String> String::Split(const char* str, char separator)
{
    Vector<String> ret;
    unsigned pos = 0;
    unsigned length = CStringLength(str);
    
    while (pos < length)
    {
        if (str[pos] != separator)
            break;
        ++pos;
    }
    
    while (pos < length)
    {
        unsigned start = pos;
        
        while (start < length)
        {
            if (str[start] == separator)
                break;
            
            ++start;
        }
        
        if (start == length)
        {
            ret.Push(String(&str[pos]));
            break;
        }
        
        unsigned end = start;
        
        while (end < length)
        {
            if (str[end] != separator)
                break;
            
            ++end;
        }
        
        ret.Push(String(&str[pos], start - pos));
        pos = end;
    }
    
    return ret;
}

String String::Joined(const Vector<String>& subStrings, String glue)
{
    if (subStrings.Empty())
        return String();
    
    String joinedString(subStrings[0]);
    for (unsigned i = 1; i < subStrings.Size(); ++i)
        joinedString.Append(glue).Append(subStrings[i]);
    
    return joinedString;
}

String& String::AppendWithFormat(const char* formatString, ... )
{
    va_list args;
    va_start(args, formatString);
    AppendWithFormatArgs(formatString, args);
    va_end(args);
    return *this;
}

String& String::AppendWithFormatArgs(const char* formatString, va_list args)
{
    int pos = 0, lastPos = 0;
    int length = strlen(formatString);

    while (true)
    {
        // Scan the format string and find %a argument where a is one of d, f, s ...
        while (pos < length && formatString[pos] != '%') pos++;
        Append(formatString + lastPos, pos - lastPos);
        if (pos >= length)
            return *this;
        
        char arg = formatString[pos + 1];
        pos += 2;
        lastPos = pos;
        
        switch (arg)
        {
        // Integer
        case 'd':
        case 'i':
            {
                int arg = va_arg(args, int);
                Append(String(arg));
                break;
            }
            
        /// Unsigned
        case 'u':
            {
                unsigned arg = va_arg(args, unsigned);
                Append(String(arg));
                break;
            }
            
        /// Real
        case 'f':
            {
                double arg = va_arg(args, double);
                Append(String(arg));
                break;
            }
            
        /// Character
        case 'c':
            {
                int arg = va_arg(args, int);
                Append(arg);
                break;
            }
            
        /// C string
        case 's':
            {
                char* arg = va_arg(args, char*);
                Append(arg);
                break;
            }
            
        /// Hex
        case 'x':
            {
                char buf[CONVERSION_bufferLENGTH];
                int arg = va_arg(args, int);
                int arglen = ::sprintf(buf, "%x", arg);
                Append(buf, arglen);
                break;
            }
            
        /// Pointer
        case 'p':
            {
                char buf[CONVERSION_bufferLENGTH];
                int arg = va_arg(args, int);
                int arglen = ::sprintf(buf, "%p", reinterpret_cast<void*>(arg));
                Append(buf, arglen);
                break;
            }
            
        case '%':
            {
                Append("%", 1);
                break;
            }
        }
    }

    return *this;
}

int String::Compare(const char* lhs, const char* rhs, bool caseSensitive)
{
    if (!lhs || !rhs)
        return lhs ? 1 : (rhs ? -1 : 0);
    
    if (caseSensitive)
        return strcmp(lhs, rhs);
    else
    {
        for (;;)
        {
            char l = tolower(*lhs);
            char r = tolower(*rhs);
            if (!l || !r)
                return l ? 1 : (r ? -1 : 0);
            if (l < r)
                return -1;
            if (l > r)
                return 1;
            
            ++lhs;
            ++rhs;
        }
    }
}

void String::Replace(unsigned pos, unsigned length, const char* srcStart, unsigned srcLength)
{
    int delta = (int)srcLength - (int)length;
    
    if (pos + length < this->length)
    {
        if (delta < 0)
        {
            MoveRange(pos + srcLength, pos + length, this->length - pos - length);
            Resize(this->length + delta);
        }
        if (delta > 0)
        {
            Resize(this->length + delta);
            MoveRange(pos + srcLength, pos + length, this->length - pos - length - delta);
        }
    }
    else
        Resize(this->length + delta);
    
    CopyChars(buffer + pos, srcStart, srcLength);
}

WString::WString() 
    : length(0)
    , buffer(0)
    , bytes(0)
{
}

WString::WString(const wchar_t* str) 
    : length(0)
    , buffer(0)
    , bytes(0)
{
    Resize(wcslen(str));
    wcscpy(buffer, str);
}

WString::WString(const UTF8String& str) 
    : length(0)
    , buffer(0)
    , bytes(0)

{
    #ifdef PLATFORM_WINDOWS
    Resize(str.Length());
    MultiByteToWideChar(CP_UTF8, 0, str.CString(), -1, buffer, bytes);
    #else
    Resize(str.Length());

    unsigned byteOffset = 0;
    wchar_t* dest = buffer;
    while (byteOffset < str.ByteLength())
        *dest++ = str.NextChar(byteOffset);
    #endif
}

WString::~WString()
{
    Deallocate(buffer);
}

void WString::Resize(unsigned newSize)
{
    if (!newSize)
    {
        Deallocate(buffer);
        buffer = 0;
        length = 0;
        bytes = 0;
    }
    else
    {
        bytes = sizeof(wchar_t)*(newSize + 1);
        wchar_t* newBuffer = static_cast<wchar_t*>(AllocatorAllocate(AllocatorGetHeap(), bytes, 0));
        memset(newBuffer, 0, sizeof(wchar_t)*(newSize + 1));
        if (buffer)
        {           
            memcpy(newBuffer, buffer, length * sizeof(wchar_t));
            Deallocate(buffer);
        }
        buffer = newBuffer;
        length = newSize;
    }
}


/*********************************************/
/***************  UTF8String *****************/

    static short lowers[] = {
    (short)0x0061, (short)0x0062, (short)0x0063, (short)0x0064, (short)0x0065, (short)0x0066, (short)0x0067, (short)0x0068, (short)0x0069, 
    (short)0x006A, (short)0x006B, (short)0x006C, (short)0x006D, (short)0x006E, (short)0x006F, (short)0x0070, (short)0x0071, (short)0x0072, 
    (short)0x0073, (short)0x0074, (short)0x0075, (short)0x0076, (short)0x0077, (short)0x0078, (short)0x0079, (short)0x007A, (short)0x00E0, 
    (short)0x00E1, (short)0x00E2, (short)0x00E3, (short)0x00E4, (short)0x00E5, (short)0x00E6, (short)0x00E7, (short)0x00E8, (short)0x00E9, 
    (short)0x00EA, (short)0x00EB, (short)0x00EC, (short)0x00ED, (short)0x00EE, (short)0x00EF, (short)0x00F0, (short)0x00F1, (short)0x00F2, 
    (short)0x00F3, (short)0x00F4, (short)0x00F5, (short)0x00F6, (short)0x00F8, (short)0x00F9, (short)0x00FA, (short)0x00FB, (short)0x00FC, 
    (short)0x00FD, (short)0x00FE, (short)0x00FF, (short)0x0101, (short)0x0103, (short)0x0105, (short)0x0107, (short)0x0109, (short)0x010B, 
    (short)0x010D, (short)0x010F, (short)0x0111, (short)0x0113, (short)0x0115, (short)0x0117, (short)0x0119, (short)0x011B, (short)0x011D, 
    (short)0x011F, (short)0x0121, (short)0x0123, (short)0x0125, (short)0x0127, (short)0x0129, (short)0x012B, (short)0x012D, (short)0x012F, 
    (short)0x0131, (short)0x0133, (short)0x0135, (short)0x0137, (short)0x013A, (short)0x013C, (short)0x013E, (short)0x0140, (short)0x0142, 
    (short)0x0144, (short)0x0146, (short)0x0148, (short)0x014B, (short)0x014D, (short)0x014F, (short)0x0151, (short)0x0153, (short)0x0155, 
    (short)0x0157, (short)0x0159, (short)0x015B, (short)0x015D, (short)0x015F, (short)0x0161, (short)0x0163, (short)0x0165, (short)0x0167, 
    (short)0x0169, (short)0x016B, (short)0x016D, (short)0x016F, (short)0x0171, (short)0x0173, (short)0x0175, (short)0x0177, (short)0x017A, 
    (short)0x017C, (short)0x017E, (short)0x0183, (short)0x0185, (short)0x0188, (short)0x018C, (short)0x0192, (short)0x0199, (short)0x01A1, 
    (short)0x01A3, (short)0x01A5, (short)0x01A8, (short)0x01AD, (short)0x01B0, (short)0x01B4, (short)0x01B6, (short)0x01B9, (short)0x01BD, 
    (short)0x01C6, (short)0x01C9, (short)0x01CC, (short)0x01CE, (short)0x01D0, (short)0x01D2, (short)0x01D4, (short)0x01D6, (short)0x01D8, 
    (short)0x01DA, (short)0x01DC, (short)0x01DF, (short)0x01E1, (short)0x01E3, (short)0x01E5, (short)0x01E7, (short)0x01E9, (short)0x01EB, 
    (short)0x01ED, (short)0x01EF, (short)0x01F3, (short)0x01F5, (short)0x01FB, (short)0x01FD, (short)0x01FF, (short)0x0201, (short)0x0203, 
    (short)0x0205, (short)0x0207, (short)0x0209, (short)0x020B, (short)0x020D, (short)0x020F, (short)0x0211, (short)0x0213, (short)0x0215, 
    (short)0x0217, (short)0x0253, (short)0x0254, (short)0x0257, (short)0x0258, (short)0x0259, (short)0x025B, (short)0x0260, (short)0x0263, 
    (short)0x0268, (short)0x0269, (short)0x026F, (short)0x0272, (short)0x0275, (short)0x0283, (short)0x0288, (short)0x028A, (short)0x028B, 
    (short)0x0292, (short)0x03AC, (short)0x03AD, (short)0x03AE, (short)0x03AF, (short)0x03B1, (short)0x03B2, (short)0x03B3, (short)0x03B4, 
    (short)0x03B5, (short)0x03B6, (short)0x03B7, (short)0x03B8, (short)0x03B9, (short)0x03BA, (short)0x03BB, (short)0x03BC, (short)0x03BD, 
    (short)0x03BE, (short)0x03BF, (short)0x03C0, (short)0x03C1, (short)0x03C3, (short)0x03C4, (short)0x03C5, (short)0x03C6, (short)0x03C7, 
    (short)0x03C8, (short)0x03C9, (short)0x03CA, (short)0x03CB, (short)0x03CC, (short)0x03CD, (short)0x03CE, (short)0x03E3, (short)0x03E5, 
    (short)0x03E7, (short)0x03E9, (short)0x03EB, (short)0x03ED, (short)0x03EF, (short)0x0430, (short)0x0431, (short)0x0432, (short)0x0433, 
    (short)0x0434, (short)0x0435, (short)0x0436, (short)0x0437, (short)0x0438, (short)0x0439, (short)0x043A, (short)0x043B, (short)0x043C, 
    (short)0x043D, (short)0x043E, (short)0x043F, (short)0x0440, (short)0x0441, (short)0x0442, (short)0x0443, (short)0x0444, (short)0x0445, 
    (short)0x0446, (short)0x0447, (short)0x0448, (short)0x0449, (short)0x044A, (short)0x044B, (short)0x044C, (short)0x044D, (short)0x044E, 
    (short)0x044F, (short)0x0451, (short)0x0452, (short)0x0453, (short)0x0454, (short)0x0455, (short)0x0456, (short)0x0457, (short)0x0458, 
    (short)0x0459, (short)0x045A, (short)0x045B, (short)0x045C, (short)0x045E, (short)0x045F, (short)0x0461, (short)0x0463, (short)0x0465, 
    (short)0x0467, (short)0x0469, (short)0x046B, (short)0x046D, (short)0x046F, (short)0x0471, (short)0x0473, (short)0x0475, (short)0x0477, 
    (short)0x0479, (short)0x047B, (short)0x047D, (short)0x047F, (short)0x0481, (short)0x0491, (short)0x0493, (short)0x0495, (short)0x0497, 
    (short)0x0499, (short)0x049B, (short)0x049D, (short)0x049F, (short)0x04A1, (short)0x04A3, (short)0x04A5, (short)0x04A7, (short)0x04A9, 
    (short)0x04AB, (short)0x04AD, (short)0x04AF, (short)0x04B1, (short)0x04B3, (short)0x04B5, (short)0x04B7, (short)0x04B9, (short)0x04BB, 
    (short)0x04BD, (short)0x04BF, (short)0x04C2, (short)0x04C4, (short)0x04C8, (short)0x04CC, (short)0x04D1, (short)0x04D3, (short)0x04D5, 
    (short)0x04D7, (short)0x04D9, (short)0x04DB, (short)0x04DD, (short)0x04DF, (short)0x04E1, (short)0x04E3, (short)0x04E5, (short)0x04E7, 
    (short)0x04E9, (short)0x04EB, (short)0x04EF, (short)0x04F1, (short)0x04F3, (short)0x04F5, (short)0x04F9, (short)0x0561, (short)0x0562, 
    (short)0x0563, (short)0x0564, (short)0x0565, (short)0x0566, (short)0x0567, (short)0x0568, (short)0x0569, (short)0x056A, (short)0x056B, 
    (short)0x056C, (short)0x056D, (short)0x056E, (short)0x056F, (short)0x0570, (short)0x0571, (short)0x0572, (short)0x0573, (short)0x0574, 
    (short)0x0575, (short)0x0576, (short)0x0577, (short)0x0578, (short)0x0579, (short)0x057A, (short)0x057B, (short)0x057C, (short)0x057D, 
    (short)0x057E, (short)0x057F, (short)0x0580, (short)0x0581, (short)0x0582, (short)0x0583, (short)0x0584, (short)0x0585, (short)0x0586, 
    (short)0x10D0, (short)0x10D1, (short)0x10D2, (short)0x10D3, (short)0x10D4, (short)0x10D5, (short)0x10D6, (short)0x10D7, (short)0x10D8, 
    (short)0x10D9, (short)0x10DA, (short)0x10DB, (short)0x10DC, (short)0x10DD, (short)0x10DE, (short)0x10DF, (short)0x10E0, (short)0x10E1, 
    (short)0x10E2, (short)0x10E3, (short)0x10E4, (short)0x10E5, (short)0x10E6, (short)0x10E7, (short)0x10E8, (short)0x10E9, (short)0x10EA, 
    (short)0x10EB, (short)0x10EC, (short)0x10ED, (short)0x10EE, (short)0x10EF, (short)0x10F0, (short)0x10F1, (short)0x10F2, (short)0x10F3, 
    (short)0x10F4, (short)0x10F5, (short)0x1E01, (short)0x1E03, (short)0x1E05, (short)0x1E07, (short)0x1E09, (short)0x1E0B, (short)0x1E0D, 
    (short)0x1E0F, (short)0x1E11, (short)0x1E13, (short)0x1E15, (short)0x1E17, (short)0x1E19, (short)0x1E1B, (short)0x1E1D, (short)0x1E1F, 
    (short)0x1E21, (short)0x1E23, (short)0x1E25, (short)0x1E27, (short)0x1E29, (short)0x1E2B, (short)0x1E2D, (short)0x1E2F, (short)0x1E31, 
    (short)0x1E33, (short)0x1E35, (short)0x1E37, (short)0x1E39, (short)0x1E3B, (short)0x1E3D, (short)0x1E3F, (short)0x1E41, (short)0x1E43, 
    (short)0x1E45, (short)0x1E47, (short)0x1E49, (short)0x1E4B, (short)0x1E4D, (short)0x1E4F, (short)0x1E51, (short)0x1E53, (short)0x1E55, 
    (short)0x1E57, (short)0x1E59, (short)0x1E5B, (short)0x1E5D, (short)0x1E5F, (short)0x1E61, (short)0x1E63, (short)0x1E65, (short)0x1E67, 
    (short)0x1E69, (short)0x1E6B, (short)0x1E6D, (short)0x1E6F, (short)0x1E71, (short)0x1E73, (short)0x1E75, (short)0x1E77, (short)0x1E79, 
    (short)0x1E7B, (short)0x1E7D, (short)0x1E7F, (short)0x1E81, (short)0x1E83, (short)0x1E85, (short)0x1E87, (short)0x1E89, (short)0x1E8B, 
    (short)0x1E8D, (short)0x1E8F, (short)0x1E91, (short)0x1E93, (short)0x1E95, (short)0x1EA1, (short)0x1EA3, (short)0x1EA5, (short)0x1EA7, 
    (short)0x1EA9, (short)0x1EAB, (short)0x1EAD, (short)0x1EAF, (short)0x1EB1, (short)0x1EB3, (short)0x1EB5, (short)0x1EB7, (short)0x1EB9, 
    (short)0x1EBB, (short)0x1EBD, (short)0x1EBF, (short)0x1EC1, (short)0x1EC3, (short)0x1EC5, (short)0x1EC7, (short)0x1EC9, (short)0x1ECB, 
    (short)0x1ECD, (short)0x1ECF, (short)0x1ED1, (short)0x1ED3, (short)0x1ED5, (short)0x1ED7, (short)0x1ED9, (short)0x1EDB, (short)0x1EDD, 
    (short)0x1EDF, (short)0x1EE1, (short)0x1EE3, (short)0x1EE5, (short)0x1EE7, (short)0x1EE9, (short)0x1EEB, (short)0x1EED, (short)0x1EEF, 
    (short)0x1EF1, (short)0x1EF3, (short)0x1EF5, (short)0x1EF7, (short)0x1EF9, (short)0x1F00, (short)0x1F01, (short)0x1F02, (short)0x1F03, 
    (short)0x1F04, (short)0x1F05, (short)0x1F06, (short)0x1F07, (short)0x1F10, (short)0x1F11, (short)0x1F12, (short)0x1F13, (short)0x1F14, 
    (short)0x1F15, (short)0x1F20, (short)0x1F21, (short)0x1F22, (short)0x1F23, (short)0x1F24, (short)0x1F25, (short)0x1F26, (short)0x1F27, 
    (short)0x1F30, (short)0x1F31, (short)0x1F32, (short)0x1F33, (short)0x1F34, (short)0x1F35, (short)0x1F36, (short)0x1F37, (short)0x1F40, 
    (short)0x1F41, (short)0x1F42, (short)0x1F43, (short)0x1F44, (short)0x1F45, (short)0x1F51, (short)0x1F53, (short)0x1F55, (short)0x1F57, 
    (short)0x1F60, (short)0x1F61, (short)0x1F62, (short)0x1F63, (short)0x1F64, (short)0x1F65, (short)0x1F66, (short)0x1F67, (short)0x1F80, 
    (short)0x1F81, (short)0x1F82, (short)0x1F83, (short)0x1F84, (short)0x1F85, (short)0x1F86, (short)0x1F87, (short)0x1F90, (short)0x1F91, 
    (short)0x1F92, (short)0x1F93, (short)0x1F94, (short)0x1F95, (short)0x1F96, (short)0x1F97, (short)0x1FA0, (short)0x1FA1, (short)0x1FA2, 
    (short)0x1FA3, (short)0x1FA4, (short)0x1FA5, (short)0x1FA6, (short)0x1FA7, (short)0x1FB0, (short)0x1FB1, (short)0x1FD0, (short)0x1FD1, 
    (short)0x1FE0, (short)0x1FE1, (short)0x24D0, (short)0x24D1, (short)0x24D2, (short)0x24D3, (short)0x24D4, (short)0x24D5, (short)0x24D6, 
    (short)0x24D7, (short)0x24D8, (short)0x24D9, (short)0x24DA, (short)0x24DB, (short)0x24DC, (short)0x24DD, (short)0x24DE, (short)0x24DF, 
    (short)0x24E0, (short)0x24E1, (short)0x24E2, (short)0x24E3, (short)0x24E4, (short)0x24E5, (short)0x24E6, (short)0x24E7, (short)0x24E8, 
    (short)0x24E9, (short)0xFF41, (short)0xFF42, (short)0xFF43, (short)0xFF44, (short)0xFF45, (short)0xFF46, (short)0xFF47, (short)0xFF48, 
    (short)0xFF49, (short)0xFF4A, (short)0xFF4B, (short)0xFF4C, (short)0xFF4D, (short)0xFF4E, (short)0xFF4F, (short)0xFF50, (short)0xFF51, 
    (short)0xFF52, (short)0xFF53, (short)0xFF54, (short)0xFF55, (short)0xFF56, (short)0xFF57, (short)0xFF58, (short)0xFF59, (short)0xFF5A, 
    (short)0x0000 };

    static short uppers[] = {
    (short)0x0041, (short)0x0042, (short)0x0043, (short)0x0044, (short)0x0045, (short)0x0046, (short)0x0047, (short)0x0048, (short)0x0049, 
    (short)0x004A, (short)0x004B, (short)0x004C, (short)0x004D, (short)0x004E, (short)0x004F, (short)0x0050, (short)0x0051, (short)0x0052, 
    (short)0x0053, (short)0x0054, (short)0x0055, (short)0x0056, (short)0x0057, (short)0x0058, (short)0x0059, (short)0x005A, (short)0x00C0, 
    (short)0x00C1, (short)0x00C2, (short)0x00C3, (short)0x00C4, (short)0x00C5, (short)0x00C6, (short)0x00C7, (short)0x00C8, (short)0x00C9, 
    (short)0x00CA, (short)0x00CB, (short)0x00CC, (short)0x00CD, (short)0x00CE, (short)0x00CF, (short)0x00D0, (short)0x00D1, (short)0x00D2, 
    (short)0x00D3, (short)0x00D4, (short)0x00D5, (short)0x00D6, (short)0x00D8, (short)0x00D9, (short)0x00DA, (short)0x00DB, (short)0x00DC, 
    (short)0x00DD, (short)0x00DE, (short)0x0178, (short)0x0100, (short)0x0102, (short)0x0104, (short)0x0106, (short)0x0108, (short)0x010A, 
    (short)0x010C, (short)0x010E, (short)0x0110, (short)0x0112, (short)0x0114, (short)0x0116, (short)0x0118, (short)0x011A, (short)0x011C, 
    (short)0x011E, (short)0x0120, (short)0x0122, (short)0x0124, (short)0x0126, (short)0x0128, (short)0x012A, (short)0x012C, (short)0x012E, 
    (short)0x0049, (short)0x0132, (short)0x0134, (short)0x0136, (short)0x0139, (short)0x013B, (short)0x013D, (short)0x013F, (short)0x0141, 
    (short)0x0143, (short)0x0145, (short)0x0147, (short)0x014A, (short)0x014C, (short)0x014E, (short)0x0150, (short)0x0152, (short)0x0154, 
    (short)0x0156, (short)0x0158, (short)0x015A, (short)0x015C, (short)0x015E, (short)0x0160, (short)0x0162, (short)0x0164, (short)0x0166, 
    (short)0x0168, (short)0x016A, (short)0x016C, (short)0x016E, (short)0x0170, (short)0x0172, (short)0x0174, (short)0x0176, (short)0x0179, 
    (short)0x017B, (short)0x017D, (short)0x0182, (short)0x0184, (short)0x0187, (short)0x018B, (short)0x0191, (short)0x0198, (short)0x01A0, 
    (short)0x01A2, (short)0x01A4, (short)0x01A7, (short)0x01AC, (short)0x01AF, (short)0x01B3, (short)0x01B5, (short)0x01B8, (short)0x01BC, 
    (short)0x01C4, (short)0x01C7, (short)0x01CA, (short)0x01CD, (short)0x01CF, (short)0x01D1, (short)0x01D3, (short)0x01D5, (short)0x01D7, 
    (short)0x01D9, (short)0x01DB, (short)0x01DE, (short)0x01E0, (short)0x01E2, (short)0x01E4, (short)0x01E6, (short)0x01E8, (short)0x01EA, 
    (short)0x01EC, (short)0x01EE, (short)0x01F1, (short)0x01F4, (short)0x01FA, (short)0x01FC, (short)0x01FE, (short)0x0200, (short)0x0202, 
    (short)0x0204, (short)0x0206, (short)0x0208, (short)0x020A, (short)0x020C, (short)0x020E, (short)0x0210, (short)0x0212, (short)0x0214, 
    (short)0x0216, (short)0x0181, (short)0x0186, (short)0x018A, (short)0x018E, (short)0x018F, (short)0x0190, (short)0x0193, (short)0x0194, 
    (short)0x0197, (short)0x0196, (short)0x019C, (short)0x019D, (short)0x019F, (short)0x01A9, (short)0x01AE, (short)0x01B1, (short)0x01B2, 
    (short)0x01B7, (short)0x0386, (short)0x0388, (short)0x0389, (short)0x038A, (short)0x0391, (short)0x0392, (short)0x0393, (short)0x0394, 
    (short)0x0395, (short)0x0396, (short)0x0397, (short)0x0398, (short)0x0399, (short)0x039A, (short)0x039B, (short)0x039C, (short)0x039D, 
    (short)0x039E, (short)0x039F, (short)0x03A0, (short)0x03A1, (short)0x03A3, (short)0x03A4, (short)0x03A5, (short)0x03A6, (short)0x03A7, 
    (short)0x03A8, (short)0x03A9, (short)0x03AA, (short)0x03AB, (short)0x038C, (short)0x038E, (short)0x038F, (short)0x03E2, (short)0x03E4, 
    (short)0x03E6, (short)0x03E8, (short)0x03EA, (short)0x03EC, (short)0x03EE, (short)0x0410, (short)0x0411, (short)0x0412, (short)0x0413, 
    (short)0x0414, (short)0x0415, (short)0x0416, (short)0x0417, (short)0x0418, (short)0x0419, (short)0x041A, (short)0x041B, (short)0x041C, 
    (short)0x041D, (short)0x041E, (short)0x041F, (short)0x0420, (short)0x0421, (short)0x0422, (short)0x0423, (short)0x0424, (short)0x0425, 
    (short)0x0426, (short)0x0427, (short)0x0428, (short)0x0429, (short)0x042A, (short)0x042B, (short)0x042C, (short)0x042D, (short)0x042E, 
    (short)0x042F, (short)0x0401, (short)0x0402, (short)0x0403, (short)0x0404, (short)0x0405, (short)0x0406, (short)0x0407, (short)0x0408, 
    (short)0x0409, (short)0x040A, (short)0x040B, (short)0x040C, (short)0x040E, (short)0x040F, (short)0x0460, (short)0x0462, (short)0x0464, 
    (short)0x0466, (short)0x0468, (short)0x046A, (short)0x046C, (short)0x046E, (short)0x0470, (short)0x0472, (short)0x0474, (short)0x0476, 
    (short)0x0478, (short)0x047A, (short)0x047C, (short)0x047E, (short)0x0480, (short)0x0490, (short)0x0492, (short)0x0494, (short)0x0496, 
    (short)0x0498, (short)0x049A, (short)0x049C, (short)0x049E, (short)0x04A0, (short)0x04A2, (short)0x04A4, (short)0x04A6, (short)0x04A8, 
    (short)0x04AA, (short)0x04AC, (short)0x04AE, (short)0x04B0, (short)0x04B2, (short)0x04B4, (short)0x04B6, (short)0x04B8, (short)0x04BA, 
    (short)0x04BC, (short)0x04BE, (short)0x04C1, (short)0x04C3, (short)0x04C7, (short)0x04CB, (short)0x04D0, (short)0x04D2, (short)0x04D4, 
    (short)0x04D6, (short)0x04D8, (short)0x04DA, (short)0x04DC, (short)0x04DE, (short)0x04E0, (short)0x04E2, (short)0x04E4, (short)0x04E6, 
    (short)0x04E8, (short)0x04EA, (short)0x04EE, (short)0x04F0, (short)0x04F2, (short)0x04F4, (short)0x04F8, (short)0x0531, (short)0x0532, 
    (short)0x0533, (short)0x0534, (short)0x0535, (short)0x0536, (short)0x0537, (short)0x0538, (short)0x0539, (short)0x053A, (short)0x053B, 
    (short)0x053C, (short)0x053D, (short)0x053E, (short)0x053F, (short)0x0540, (short)0x0541, (short)0x0542, (short)0x0543, (short)0x0544, 
    (short)0x0545, (short)0x0546, (short)0x0547, (short)0x0548, (short)0x0549, (short)0x054A, (short)0x054B, (short)0x054C, (short)0x054D, 
    (short)0x054E, (short)0x054F, (short)0x0550, (short)0x0551, (short)0x0552, (short)0x0553, (short)0x0554, (short)0x0555, (short)0x0556, 
    (short)0x10A0, (short)0x10A1, (short)0x10A2, (short)0x10A3, (short)0x10A4, (short)0x10A5, (short)0x10A6, (short)0x10A7, (short)0x10A8, 
    (short)0x10A9, (short)0x10AA, (short)0x10AB, (short)0x10AC, (short)0x10AD, (short)0x10AE, (short)0x10AF, (short)0x10B0, (short)0x10B1, 
    (short)0x10B2, (short)0x10B3, (short)0x10B4, (short)0x10B5, (short)0x10B6, (short)0x10B7, (short)0x10B8, (short)0x10B9, (short)0x10BA, 
    (short)0x10BB, (short)0x10BC, (short)0x10BD, (short)0x10BE, (short)0x10BF, (short)0x10C0, (short)0x10C1, (short)0x10C2, (short)0x10C3, 
    (short)0x10C4, (short)0x10C5, (short)0x1E00, (short)0x1E02, (short)0x1E04, (short)0x1E06, (short)0x1E08, (short)0x1E0A, (short)0x1E0C, 
    (short)0x1E0E, (short)0x1E10, (short)0x1E12, (short)0x1E14, (short)0x1E16, (short)0x1E18, (short)0x1E1A, (short)0x1E1C, (short)0x1E1E, 
    (short)0x1E20, (short)0x1E22, (short)0x1E24, (short)0x1E26, (short)0x1E28, (short)0x1E2A, (short)0x1E2C, (short)0x1E2E, (short)0x1E30, 
    (short)0x1E32, (short)0x1E34, (short)0x1E36, (short)0x1E38, (short)0x1E3A, (short)0x1E3C, (short)0x1E3E, (short)0x1E40, (short)0x1E42, 
    (short)0x1E44, (short)0x1E46, (short)0x1E48, (short)0x1E4A, (short)0x1E4C, (short)0x1E4E, (short)0x1E50, (short)0x1E52, (short)0x1E54, 
    (short)0x1E56, (short)0x1E58, (short)0x1E5A, (short)0x1E5C, (short)0x1E5E, (short)0x1E60, (short)0x1E62, (short)0x1E64, (short)0x1E66, 
    (short)0x1E68, (short)0x1E6A, (short)0x1E6C, (short)0x1E6E, (short)0x1E70, (short)0x1E72, (short)0x1E74, (short)0x1E76, (short)0x1E78, 
    (short)0x1E7A, (short)0x1E7C, (short)0x1E7E, (short)0x1E80, (short)0x1E82, (short)0x1E84, (short)0x1E86, (short)0x1E88, (short)0x1E8A, 
    (short)0x1E8C, (short)0x1E8E, (short)0x1E90, (short)0x1E92, (short)0x1E94, (short)0x1EA0, (short)0x1EA2, (short)0x1EA4, (short)0x1EA6, 
    (short)0x1EA8, (short)0x1EAA, (short)0x1EAC, (short)0x1EAE, (short)0x1EB0, (short)0x1EB2, (short)0x1EB4, (short)0x1EB6, (short)0x1EB8, 
    (short)0x1EBA, (short)0x1EBC, (short)0x1EBE, (short)0x1EC0, (short)0x1EC2, (short)0x1EC4, (short)0x1EC6, (short)0x1EC8, (short)0x1ECA, 
    (short)0x1ECC, (short)0x1ECE, (short)0x1ED0, (short)0x1ED2, (short)0x1ED4, (short)0x1ED6, (short)0x1ED8, (short)0x1EDA, (short)0x1EDC, 
    (short)0x1EDE, (short)0x1EE0, (short)0x1EE2, (short)0x1EE4, (short)0x1EE6, (short)0x1EE8, (short)0x1EEA, (short)0x1EEC, (short)0x1EEE, 
    (short)0x1EF0, (short)0x1EF2, (short)0x1EF4, (short)0x1EF6, (short)0x1EF8, (short)0x1F08, (short)0x1F09, (short)0x1F0A, (short)0x1F0B, 
    (short)0x1F0C, (short)0x1F0D, (short)0x1F0E, (short)0x1F0F, (short)0x1F18, (short)0x1F19, (short)0x1F1A, (short)0x1F1B, (short)0x1F1C, 
    (short)0x1F1D, (short)0x1F28, (short)0x1F29, (short)0x1F2A, (short)0x1F2B, (short)0x1F2C, (short)0x1F2D, (short)0x1F2E, (short)0x1F2F, 
    (short)0x1F38, (short)0x1F39, (short)0x1F3A, (short)0x1F3B, (short)0x1F3C, (short)0x1F3D, (short)0x1F3E, (short)0x1F3F, (short)0x1F48, 
    (short)0x1F49, (short)0x1F4A, (short)0x1F4B, (short)0x1F4C, (short)0x1F4D, (short)0x1F59, (short)0x1F5B, (short)0x1F5D, (short)0x1F5F, 
    (short)0x1F68, (short)0x1F69, (short)0x1F6A, (short)0x1F6B, (short)0x1F6C, (short)0x1F6D, (short)0x1F6E, (short)0x1F6F, (short)0x1F88, 
    (short)0x1F89, (short)0x1F8A, (short)0x1F8B, (short)0x1F8C, (short)0x1F8D, (short)0x1F8E, (short)0x1F8F, (short)0x1F98, (short)0x1F99, 
    (short)0x1F9A, (short)0x1F9B, (short)0x1F9C, (short)0x1F9D, (short)0x1F9E, (short)0x1F9F, (short)0x1FA8, (short)0x1FA9, (short)0x1FAA, 
    (short)0x1FAB, (short)0x1FAC, (short)0x1FAD, (short)0x1FAE, (short)0x1FAF, (short)0x1FB8, (short)0x1FB9, (short)0x1FD8, (short)0x1FD9, 
    (short)0x1FE8, (short)0x1FE9, (short)0x24B6, (short)0x24B7, (short)0x24B8, (short)0x24B9, (short)0x24BA, (short)0x24BB, (short)0x24BC, 
    (short)0x24BD, (short)0x24BE, (short)0x24BF, (short)0x24C0, (short)0x24C1, (short)0x24C2, (short)0x24C3, (short)0x24C4, (short)0x24C5, 
    (short)0x24C6, (short)0x24C7, (short)0x24C8, (short)0x24C9, (short)0x24CA, (short)0x24CB, (short)0x24CC, (short)0x24CD, (short)0x24CE, 
    (short)0x24CF, (short)0xFF21, (short)0xFF22, (short)0xFF23, (short)0xFF24, (short)0xFF25, (short)0xFF26, (short)0xFF27, (short)0xFF28, 
    (short)0xFF29, (short)0xFF2A, (short)0xFF2B, (short)0xFF2C, (short)0xFF2D, (short)0xFF2E, (short)0xFF2F, (short)0xFF30, (short)0xFF31, 
    (short)0xFF32, (short)0xFF33, (short)0xFF34, (short)0xFF35, (short)0xFF36, (short)0xFF37, (short)0xFF38, (short)0xFF39, (short)0xFF3A, 
    (short)0x0000 };

unsigned tolowerUnic(unsigned c)
{
    short *p;


    if(c > 0xffff)
        return c;
    if ((p = std::find(uppers, uppers + 667, (short)c)) != uppers + 667)
        return (unsigned)*(lowers + (p - uppers));
    return c;
}

unsigned toupperUnic(unsigned c)
{
    short *p;


    if(c > 0xffff)
        return c;

    if ((p = std::find(lowers, lowers + 667, (short)c)) != lowers + 667)
        return (unsigned)*(uppers + (p - lowers));
    return c;
}

void tolowerUnicStr(unsigned *p)
{
    while (*p)
        *p++ = tolowerUnic(*p);
}

void toupperUnicStr(unsigned *p)
{
    while (*p)
        *p++ = toupperUnic(*p);
}

char UTF8String::endZero = 0;
const UTF8String UTF8String::EMPTY;

UTF8String::UTF8String(int value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%d", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(short value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%d", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%ld", value);
    *this = tempBuffer;
}
    
UTF8String::UTF8String(long long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%lld", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(unsigned value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%u", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(unsigned short value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%u", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(unsigned long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%lu", value);
    *this = tempBuffer;
}
    
UTF8String::UTF8String(unsigned long long value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%llu", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(float value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%g", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(double value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    char tempBuffer[CONVERSION_bufferLENGTH];
    sprintf(tempBuffer, "%g", value);
    *this = tempBuffer;
}

UTF8String::UTF8String(bool value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    if (value)
        *this = "true";
    else
        *this = "false";
}

UTF8String::UTF8String(char value) :
    length(0),
    capacity(0),
    buffer(&endZero)
{
    *this = value;
}

UTF8String::UTF8String(char value, unsigned length) :
    length(0),
    capacity(0),
    buffer(&endZero)
{

    auto str = UTF8String(value);
    int byteLength = str.ByteLength();
    const char * val = str.CString();
    Resize(length * byteLength);
    for (unsigned i = 0; i < length; ++i)
    {
        for(int j = 0; j < byteLength; ++j )
            buffer[i*byteLength +j] = val[j];
    }
}

UTF8String::UTF8String(const WString& str) 
    : length(0)
    , capacity(0)
    , buffer(&endZero)
{
    SetUTF8FromWChar(str.CString());
}

UTF8String::UTF8String() 
    : length(0)
    , capacity(0)
    , buffer(&endZero)
{
}

UTF8String::UTF8String(const String& str) 
    : length(0)
    , capacity(0)
    , buffer(&endZero)
{
    SetUTF8FromLatin1(str.CString());
}

UTF8String::UTF8String(const UTF8String& str) 
    : length(0)
    , capacity(0)
    , buffer(&endZero)
{
    *this = str;
}
UTF8String::UTF8String(UTF8String&& str) 
    : length(str.length)
    , capacity(str.capacity)
    , buffer(str.buffer)
{
        str.buffer = nullptr;
        str.length = 0;
        str.capacity = 0;
}


UTF8String::UTF8String(const char* str, bool isUTF8)
    : length(0)
    , capacity(0)
    , buffer(&endZero)

{
    if(isUTF8)
        AuxAppend(str);
    else
        SetUTF8FromLatin1(str);
}

UTF8String::UTF8String(const wchar_t* str)
    : length(0)
    , capacity(0)
    , buffer(&endZero)

{
    SetUTF8FromWChar(str);
}

UTF8String::UTF8String(wchar_t* str) 
    : length(0)
    , capacity(0)
    , buffer(&endZero)
    {
        SetUTF8FromWChar(str);
    }

void UTF8String::SetUTF8FromLatin1(const char* str)
{
    char temp[7];
    
    Clear();
    
    if (!str)
        return;
    
    while (*str)
    {
        char* dest = temp;
        EncodeUTF8(dest, (byte)*str++);
        *dest = 0;
        AuxAppend(temp);
    }
}

UTF8String UTF8String::GetUTF8FromLatin1(const char* str) const
{
    UTF8String ret;
    ret.SetUTF8FromLatin1(str);
    return ret;
}

void UTF8String::SetUTF8FromWChar(const wchar_t* str)
{
    char temp[7];
    
    Clear();
    
    if (!str)
        return;
    
    #ifdef PLATFORM_WINDOWS
    while (*str)
    {
        unsigned unicodeChar = DecodeUTF16(str);
        char* dest = temp;
        EncodeUTF8(dest, unicodeChar);
        *dest = 0;
        AuxAppend(temp);
    }
    #else
    while (*str)
    {
        char* dest = temp;
        EncodeUTF8(dest, *str++);
        *dest = 0;
        AuxAppend(temp);
    }
    #endif
}

unsigned UTF8String::Length() const
{
    unsigned ret = 0;
    
    const char* src = buffer;
    if (!src)
        return ret;
    const char* end = buffer + length;
    
    while (src < end)
    {
        DecodeUTF8(src);
        ++ret;
    }
    
    return ret;
}

String UTF8String::GetString() const
{
    auto l = Length();
    String ret;
    
    const char* src = buffer;
    if (!src)
        return "";
    
    for(unsigned i = 0; i < l; ++i)
        ret += (char)DecodeUTF8(src);
    
    return ret;
}

void UTF8String::Erase(unsigned pos, unsigned length)
{
    Replace(pos, length, UTF8String::EMPTY);
}

unsigned UTF8String::ByteOffset(unsigned index) const
{
    unsigned byteOffset = 0;
    unsigned utfPos = 0;
    
    while (utfPos < index && byteOffset < length)
    {
        NextChar(byteOffset);
        ++utfPos;
    }
    
    return byteOffset;
}

unsigned UTF8String::NextChar(unsigned& byteOffset) const
{
    if (!buffer)
        return 0;
    
    const char* src = buffer + byteOffset;
    unsigned ret = DecodeUTF8(src);
    byteOffset = src - buffer;
    
    return ret;
}

unsigned UTF8String::CharOffset(char utf8Char)
{
    unsigned ret = 0;
    for(int i = 0; i < 8; i++ )
    {
        if((utf8Char & 1) == 1)
            ret++;
        else 
            ret = 0;
        utf8Char >>= 1;
    }
    return ret ? ret : 1;
}

unsigned UTF8String::At(unsigned index) const
{
    unsigned byteOffset = ByteOffset(index);
    return NextChar(byteOffset);
}

void UTF8String::Replace(unsigned replaceThis, unsigned replaceWith, bool caseSensitive)
{
    int length = Length();
    if (caseSensitive)
    {
        for (int i = 0; i < length; ++i)
        {
            if (At(i) == replaceThis)
                Replace(i, replaceWith);
        }
    }
    else
    {
        replaceThis = tolowerUnic(replaceThis);
        for (int i = 0; i < length; ++i)
        {
            if (tolowerUnic(At(i)) == replaceThis)
                Replace(i, replaceWith);
        }
    }
}

void UTF8String::Replace(const UTF8String& replaceThis, const UTF8String& replaceWith, bool caseSensitive)
{
    int length = Length();
    int nextPos = 0;
    
    while (nextPos < length)
    {
        int pos = Find(replaceThis, caseSensitive, nextPos);
        if (pos == NPOS)
            break;
        Replace(pos, replaceThis.Length(), replaceWith);
        nextPos = pos + replaceWith.Length();
    }
}

void UTF8String::Replace(unsigned pos, unsigned length, const UTF8String& replaceWith)
{
    /// If substring is illegal, do nothing
    if (pos + length > this->Length())
        return;
    unsigned startPos = ByteOffset(pos); 
    unsigned byteOffset = startPos;
    for(unsigned i = 0; byteOffset < this->length && i < length; ++i)
        NextChar(byteOffset);
    
    AuxReplace(startPos, byteOffset-startPos, replaceWith.buffer, replaceWith.length);
}

void UTF8String::Replace(int index, unsigned unicodeChar)
{
    int utfPos = 0;
    unsigned byteOffset = 0;
    
    while (utfPos < index && byteOffset < length)
    {
        NextChar(byteOffset);
        ++utfPos;
    }
    
    if (utfPos < index)
        return;
    
    unsigned beginCharPos = byteOffset;
    NextChar(byteOffset);
    
    char temp[7];
    char* dest = temp;
    EncodeUTF8(dest, unicodeChar);
    *dest = 0;
    
    AuxReplace(beginCharPos, byteOffset - beginCharPos, temp, dest - temp);
}

UTF8String& UTF8String::operator += (int rhs)
{
    return *this += UTF8String(rhs);
}

UTF8String& UTF8String::operator += (short rhs)
{
    return *this += UTF8String(rhs);
}

UTF8String& UTF8String::operator += (unsigned rhs)
{
    return *this += UTF8String(rhs);
}

UTF8String& UTF8String::operator += (unsigned short rhs)
{
    return *this += UTF8String(rhs);
}
UTF8String& UTF8String::operator += (long long rhs)
{
    return *this += UTF8String(rhs);
}

UTF8String& UTF8String::operator += (float rhs)
{
    return *this += UTF8String(rhs);
}

UTF8String& UTF8String::operator += (double rhs)
{
    return *this += UTF8String(rhs);
}

UTF8String& UTF8String::operator += (bool rhs)
{
    return *this += UTF8String(rhs);
}

std::istream& UTF8String::operator>> (std::istream& is)
{
    return is >> buffer;
}

std::ostream& UTF8String::operator<< (std::ostream& os)
{
    return os << buffer;
}

UTF8String& UTF8String::Append(unsigned unicodeChar)
{
    char temp[7];
    char* dest = temp;
    EncodeUTF8(dest, unicodeChar);
    *dest = 0;
    return AuxAppend(temp);
}

UTF8String& UTF8String::Append(const char* str)
{
    char temp[7];
    
    if (!str)
        return *this;

    while (*str)
    {
        char* dest = temp;
        EncodeUTF8(dest, (byte)*str++);
        *dest = 0;
        AuxAppend(temp);
    }
    return *this;
}
UTF8String& UTF8String::Append(const wchar_t* str)
{
    char temp[7];
    if (!str)
        return *this;
    
    #ifdef PLATFORM_WINDOWS
    while (*str)
    {
        unsigned unicodeChar = DecodeUTF16(str);
        char* dest = temp;
        EncodeUTF8(dest, unicodeChar);
        *dest = 0;
        AuxAppend(temp);
    }
    #else
    while (*str)
    {
        char* dest = temp;
        EncodeUTF8(dest, (byte)*str++);
        *dest = 0;
        AuxAppend(temp);
    }
    #endif
    return *this;
}

UTF8String& UTF8String::Append(const String str)
{
    return Append(str.CString());
}

UTF8String& UTF8String::Append(const WString str)
{
    return Append(str.CString());
}

UTF8String& UTF8String::Append(const UTF8String str)
{
    return AuxAppend(str.CString());
}

UTF8String& UTF8String::AuxAppend(const char* rhs)
{

        unsigned rhsLength = CStringLength(rhs);
        unsigned oldLength = length;
        Resize(length + rhsLength);
        CopyChars(buffer + oldLength, rhs, rhsLength);
        
        return *this;
}

bool isDelimiter(int pos, int length, const char* str, char* separator)
{
    for(int i = 0; separator[i] != 0 ; ++i) 
    {
        if (pos + i >= length || str[pos +i] != separator[i])
            return false;
    }
    
    return true;
}

int IgnoreDelimiter(int pos, int length, const char* str, char* separator)
{
    while (pos < length)
    {
        int i;
        for(i = 0; separator[i] != 0 && pos + i < length; ++i) 
        {
            if (str[pos +i] != separator[i])
                return pos;
        }
        pos+=i;
    }
    return (pos > length)? length : pos;
}

int FindDelimiter(int pos, int length, const char* str, char* separator)
{
    while (pos < length)
    {
        if(isDelimiter(pos, length, str, separator))
            return pos;
        ++pos;
    }
    return pos;
}

Vector<UTF8String> UTF8String::Split(unsigned separator) const
{
    return Split(CString(), separator);
}

Vector<UTF8String> UTF8String::Split(const char* str, unsigned sep)
{
    Vector<UTF8String> ret;
    unsigned pos = 0;
    unsigned length = CStringLength(str);
    
    char separator[7];
    char* dest = separator;  
    EncodeUTF8(dest, sep);
    *dest = 0;
    pos = IgnoreDelimiter(pos, length, str, separator);
    
    while (pos < length)
    {
        unsigned start = FindDelimiter(pos, length, str, separator);

        if (start == length)
        {
            ret.Push(UTF8String(&str[pos], true));
            break;
        }

        unsigned end = IgnoreDelimiter(start, length, str, separator);

        ret.Push(UTF8String(&str[pos], start - pos, true));
        pos = end;
    }

    return ret;
}

void UTF8String::Split(unsigned separator, Vector<UTF8String>& ret) const
{
    Split(CString(), separator, ret);
}

void UTF8String::Split(const char* str, unsigned sep, Vector<UTF8String>& ret)
{
    unsigned pos = 0;
    unsigned length = CStringLength(str);

    char separator[7];
    char* dest = separator;
    EncodeUTF8(dest, sep);
    *dest = 0;
    pos = IgnoreDelimiter(pos, length, str, separator);

    while (pos < length)
    {
        unsigned start = FindDelimiter(pos, length, str, separator);

        if (start == length)
        {
            ret.Push(UTF8String(&str[pos], true));
            break;
        }
        
        unsigned end = IgnoreDelimiter(start, length, str, separator);

        ret.Push(UTF8String(&str[pos], start - pos, true));
        pos = end;
    }
}

UTF8String UTF8String::Substring(unsigned pos) const
{
    unsigned utf8Length = Length();
    unsigned byteOffset = ByteOffset(pos);
    UTF8String ret;

    while (pos < utf8Length)
    {
        ret.Append(NextChar(byteOffset));
        ++pos;
    }

    return ret;
}

UTF8String UTF8String::Substring(unsigned pos, unsigned length) const
{
    unsigned utf8Length = Length();
    unsigned byteOffset = ByteOffset(pos);
    unsigned endPos = pos + length;
    UTF8String ret;

    while (pos < endPos && pos < utf8Length)
    {
        ret.Append(NextChar(byteOffset));
        ++pos;
    }

    return ret;
}

void UTF8String::EncodeUTF8(char*& dest, unsigned unicodeChar)
{
    if (unicodeChar < 0x80)
        *dest++ = unicodeChar;
    else if (unicodeChar < 0x800)
    {
        *dest++ = 0xc0 | ((unicodeChar >> 6) & 0x1f);
        *dest++ = 0x80 | (unicodeChar & 0x3f);
    }
    else if (unicodeChar < 0x10000)
    {
        *dest++ = 0xe0 | ((unicodeChar >> 12) & 0xf);
        *dest++ = 0x80 | ((unicodeChar >> 6) & 0x3f);
        *dest++ = 0x80 | (unicodeChar & 0x3f);
    }
    else if (unicodeChar < 0x200000)
    {
        *dest++ = 0xf0 | ((unicodeChar >> 18) & 0x7);
        *dest++ = 0x80 | ((unicodeChar >> 12) & 0x3f);
        *dest++ = 0x80 | ((unicodeChar >> 6) & 0x3f);
        *dest++ = 0x80 | (unicodeChar & 0x3f);
    }
    else if (unicodeChar < 0x4000000)
    {
        *dest++ = 0xf8 | ((unicodeChar >> 24) & 0x3);
        *dest++ = 0x80 | ((unicodeChar >> 18) & 0x3f);
        *dest++ = 0x80 | ((unicodeChar >> 12) & 0x3f);
        *dest++ = 0x80 | ((unicodeChar >> 6) & 0x3f);
        *dest++ = 0x80 | (unicodeChar & 0x3f);
    }
    else
    {
        *dest++ = 0xfc | ((unicodeChar >> 30) & 0x1);
        *dest++ = 0x80 | ((unicodeChar >> 24) & 0x3f);
        *dest++ = 0x80 | ((unicodeChar >> 18) & 0x3f);
        *dest++ = 0x80 | ((unicodeChar >> 12) & 0x3f);
        *dest++ = 0x80 | ((unicodeChar >> 6) & 0x3f);
        *dest++ = 0x80 | (unicodeChar & 0x3f);
    }
}

#define GET_NEXT_CONTINUATION_BYTE(ptr) *ptr; if ((unsigned char)*ptr < 0x80 || (unsigned char)*ptr >= 0xc0) return '?'; else ++ptr;

unsigned UTF8String::DecodeUTF8(const char*& src)
{
    if (src == 0)
        return 0;

    unsigned char char1 = *src++;

    /// Check if we are in the middle of a UTF8 character
    if (char1 >= 0x80 && char1 < 0xc0)
    {
        while ((unsigned char)*src >= 0x80 && (unsigned char)*src < 0xc0)
            ++src;
        return '?';
    }

    if (char1 < 0x80)
        return char1;
    else if (char1 < 0xe0)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char2 & 0x3f) | ((char1 & 0x1f) << 6);
    }
    else if (char1 < 0xf0)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char3 & 0x3f) | ((char2 & 0x3f) << 6) | ((char1 & 0xf) << 12);
    }
    else if (char1 < 0xf8)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char4 & 0x3f) | ((char3 & 0x3f) << 6) | ((char2 & 0x3f) << 12) | ((char1 & 0x7) << 18);
    }
    else if (char1 < 0xfc)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char5 & 0x3f) | ((char4 & 0x3f) << 6) | ((char3 & 0x3f) << 12) | ((char2 & 0x3f) << 18) | ((char1 & 0x3) << 24);
    }
    else
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char6 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char6 & 0x3f) | ((char5 & 0x3f) << 6) | ((char4 & 0x3f) << 12) | ((char3 & 0x3f) << 18) | ((char2 & 0x3f) << 24) |
            ((char1 & 0x1) << 30);
    }
}

void UTF8String::AuxReplace(unsigned pos, unsigned length, const char* srcStart, unsigned srcLength)
{
    int delta = (int)srcLength - (int)length;

    if (pos + length < this->length)
    {
        if (delta < 0)
        {
            MoveRange(pos + srcLength, pos + length, this->length - pos - length);
            Resize(this->length + delta);
        }
        if (delta > 0)
        {
            Resize(this->length + delta);
            MoveRange(pos + srcLength, pos + length, this->length - pos - length - delta);
        }
    }
    else
        Resize(this->length + delta);

    CopyChars(buffer + pos, srcStart, srcLength);
}



void UTF8String::Resize(unsigned newLength)
{
    if (!capacity)
    {
        // Calculate initial capacity
        capacity = newLength + 1;
        if (capacity < MIN_CAPACITY)
            capacity = MIN_CAPACITY;

        buffer = static_cast<char*>(AllocatorAllocate(AllocatorGetHeap(),capacity, 0));
    }
    else
    {
        if (newLength && capacity < newLength + 1)
        {
            // Increase the capacity with half each time it is exceeded
            while (capacity < newLength + 1)
                capacity += (capacity + 1) >> 1;

            char* newBuffer = static_cast<char*>(AllocatorAllocate(AllocatorGetHeap(),capacity, 0));
            // Move the existing data to the new buffer, then delete the old buffer
            if (length)
                CopyChars(newBuffer, buffer, length);
            Deallocate(buffer);

            buffer = newBuffer;
        }
    }

    buffer[newLength] = 0;
    length = newLength;
}

void UTF8String::Clear()
{
    Resize(0);
}

UTF8String UTF8String::Joined(const Vector<UTF8String>& subStrings, UTF8String glue)
{
    if (subStrings.Empty())
        return UTF8String();

    UTF8String joinedString(subStrings[0]);
    for (unsigned i = 1; i < subStrings.Size(); ++i)
        joinedString.Append(glue).Append(subStrings[i]);

    return joinedString;
}

void UTF8String::Join(const Vector<UTF8String>& subStrings, UTF8String glue)
{
    *this = Joined(subStrings, glue);
}

UTF8String UTF8String::Trimmed() const
{
    unsigned trimStart = 0;
    unsigned trimEnd = length;
    while (trimStart < trimEnd)
    {
        char c = buffer[trimStart];
        if (c != ' ' && c != 9)
            break;
        ++trimStart;
    }
    while (trimEnd > trimStart)
    {
        char c = buffer[trimEnd-1];
        if (c != ' ' && c != 9)
            break;
        --trimEnd;
    }

    return Substring(trimStart, trimEnd - trimStart);
}

unsigned UTF8String::Find(unsigned c, bool caseSensitive, int startPos) const
{
    int length = Length();
    unsigned byteOffset = 0;
    if (caseSensitive)
    {
        for (int i = startPos; i < length; ++i)
        {
            if (At(i) == c)
                return i;
        }
    }
    else
    {
        c = tolowerUnic(c);
        for (int i = startPos; i < length; ++i)
        {
            if (tolowerUnic(At(i)) == c)
                return i;
        }
    }

    return NPOS;
}

unsigned UTF8String::Find(const UTF8String& str, bool caseSensitive, int startPos) const
{
    int length = Length();
    if (str.Empty() || str.Length() > Length())
        return NPOS;

    unsigned first = str[0];
    if (!caseSensitive)
        first = tolowerUnic(first);

    for (int i = startPos; i <= length - (int)str.Length(); ++i)
    {
        unsigned c = At(i);
        if (!caseSensitive)
            c = tolowerUnic(c);

        if (c == first)
        {
            unsigned skip = NPOS;
            bool found = true;
            for (int j = 1; j < (int)str.Length(); ++j)
            {
                c = At(i + j);
                unsigned d = str[j];
                if (!caseSensitive)
                {
                    c = tolowerUnic(c);
                    d = tolowerUnic(d);
                }

                if (skip == NPOS && c == first)
                    skip = i + j - 1;

                if (c != d)
                {
                    found = false;
                    if (skip != NPOS)
                        i = skip;
                    break;
                }
            }
            if (found)
                return i;
        }
    }
    
    return NPOS;
}

unsigned UTF8String::FindLast(unsigned c,  bool caseSensitive, int startPos) const
{
    int length = Length();
    if (startPos >= length || startPos < 0)
        startPos = length - 1;

    if (caseSensitive)
    {
        for (int i = startPos; i >= 0; --i)
        {
            if (At(i) == c)
                return i;
        }
    }
    else
    {
        c = tolowerUnic(c);
        for (int i = startPos; i >= 0; --i)
        {
            if (tolowerUnic(At(i)) == c)
                return i;
        }
    }
    
    return NPOS;
}

unsigned UTF8String::FindLast(const UTF8String& str, bool caseSensitive, int startPos) const
{
    int length = Length();
    if (str.Empty() || str.Length() > Length())
        return NPOS;
    if (startPos > length - (int)str.Length() || startPos < 0)
        startPos = length - str.Length();

    unsigned first = str[0];
    if (!caseSensitive)
        first = tolowerUnic(first);

    for (int i = startPos; i >= 0; --i)
    {
        unsigned c = At(i);
        if (!caseSensitive)
            c = tolowerUnic(c);

        if (c == first)
        {
            bool found = true;
            for (int j = 1; j < (int)str.Length(); ++j)
            {
                c = At(i + j);
                unsigned d = str[j];
                if (!caseSensitive)
                {
                    c = tolowerUnic(c);
                    d = tolowerUnic(d);
                }

                if (c != d)
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
    }

    return NPOS;
}

unsigned UTF8String::FindFirstNotOf(const UTF8String& str, bool caseSensitive, int startPos) const
{
    int length = Length();
    int strLength = (int)str.Length();
    if (str.Empty() || strLength > length)
        return 0;

    unsigned first = str[0];
    if (!caseSensitive)
        first = tolowerUnic(first);

    for (int i = startPos; ;i += strLength)
    {
        if(i > length)
            return i - strLength;
        if(i == length)
            return NPOS;

        unsigned c = At(i);
        if (!caseSensitive)
            c = tolowerUnic(c);

        if (c != first)
            return i;
        else
        {
            for (int j = 1; j < strLength; ++j)
            {
                c = At(i + j);
                unsigned d = str[j];
                if (!caseSensitive)
                {
                    c = tolowerUnic(c);
                    d = tolowerUnic(d);
                }
                if (c != d)
                    return i;
            }
        }
    }
}

unsigned UTF8String::FindLastNotOf(const UTF8String& str, bool caseSensitive, int startPos) const
{
    int length = Length();
    int strLength = (int)str.Length();
    if (str.Empty() || strLength > length)
        return 0;

    if (startPos > length - strLength || startPos < 0)
        startPos = length - strLength;

    unsigned first = str[0];
    if (!caseSensitive)
        first = tolowerUnic(first);

    for (int i = startPos;; i-= strLength)
    {
        if(i < 0)
            return i + strLength;
        if(i == 0)
            return NPOS;

        unsigned c = At(i);
        if (!caseSensitive)
            c = tolowerUnic(c);

        if (c != first)
            return i;
        {
            for (int j = 1; j < strLength; ++j)
            {
                c = At(i + j);
                unsigned d = str[j];
                if (!caseSensitive)
                {
                    c = tolowerUnic(c);
                    d = tolowerUnic(d);
                }

                if (c != d)
                    return i;
            }
        }
    }
}

UTF8String UTF8String::ToLower() const
{

    UTF8String ret;
    int len = Length();

    for(int i= 0; i < len; ++i)
        ret.Append(tolowerUnic(At(i)));

    return ret;
}

UTF8String UTF8String::ToUpper() const
{
    UTF8String ret;
    int len = Length();

    for(int i= 0; i < len; ++i)
        ret.Append(toupperUnic(At(i)));

    return ret;
}

int UTF8String::UTF8Compare(const char* lhs, const char* rhs, bool caseSensitive)
{
    if (!lhs || !rhs)
        return lhs ? 1 : (rhs ? -1 : 0);

    if (caseSensitive)
        return strcmp(lhs, rhs);
    else
    {
        for (;;)
        {
            unsigned l = tolowerUnic(DecodeUTF8(lhs));
            unsigned r = tolowerUnic(DecodeUTF8(rhs));
            if (!l || !r)
                return l ? 1 : (r ? -1 : 0);
            if (l < r)
                return -1;
            if (l > r)
                return 1;
        }
    }
}

int UTF8String::Compare(const UTF8String& str, bool caseSensitive) const
{
    return UTF8Compare(CString(), str.CString(), caseSensitive);
}

int UTF8String::Compare(const char* str, bool caseSensitive) const
{
    return UTF8Compare(CString(), str, caseSensitive);
}

#ifdef PLATFORM_WINDOWS
void UTF8String::EncodeUTF16(wchar_t*& dest, unsigned unicodeChar)
{
    if (unicodeChar < 0x10000)
        *dest++ = unicodeChar;
    else
    {
        unicodeChar -= 0x10000;
        *dest++ = 0xd800 | ((unicodeChar >> 10) & 0x3ff);
        *dest++ = 0xdc00 | (unicodeChar & 0x3ff);
    }
}

unsigned UTF8String::DecodeUTF16(const wchar_t*& src)
{
    if (src == 0)
        return 0;

    unsigned short word1 = *src;

    /// Check if we are at a low surrogate
    word1 = *src++;
    if (word1 >= 0xdc00 && word1 < 0xe000)
    {
        while (*src >= 0xdc00 && *src < 0xe000)
            ++src;
        return '?';
    }

    if (word1 < 0xd800 || word1 >= 0xe00)
        return word1;
    else
    {
        unsigned short word2 = *src++;
        if (word2 < 0xdc00 || word2 >= 0xe000)
        {
            --src;
            return '?';
        }
        else
            return ((word1 & 0x3ff) << 10) | (word2 & 0x3ff) | 0x10000;
    }
}
#endif

NAMESPACE_CORE_END
