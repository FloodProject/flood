/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "String.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::StringHash::StringHash(::StringHash* native)
{
    __Hash = native->hash;
}

Flood::StringHash::StringHash(System::IntPtr native)
{
    auto __native = (::StringHash*)native.ToPointer();
    __Hash = __native->hash;
}

Flood::StringHash::StringHash(System::String^ str, unsigned int size)
{
    auto _str = clix::marshalString<clix::E_UTF8>(str);
    ::StringHash _native(_str.c_str(), (::size_t)size);
    this->Hash = _native.hash;
}

unsigned int Flood::StringHash::Hash::get()
{
    return __Hash;
}

void Flood::StringHash::Hash::set(unsigned int value)
{
    __Hash = value;
}

Flood::StringHash Flood::FloodString::HashString(System::String^ _0)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(_0);
    auto __ret = ::HashString(arg0);
    return Flood::StringHash((::StringHash*)&__ret);
}

int Flood::FloodString::StringCompareInsensitive(System::String^ s1, System::String^ s2)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s1);
    auto arg1 = clix::marshalString<clix::E_UTF8>(s2);
    auto __ret = ::StringCompareInsensitive(arg0, arg1);
    return __ret;
}

System::String^ Flood::FloodString::StringFormat(System::String^ s)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto arg0 = _arg0.c_str();
    auto __ret = ::StringFormat(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

void Flood::FloodString::StringSplit(System::String^ s, char delim, System::Collections::Generic::List<System::String^>^ elems)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto _tmpelems = std::vector<::String>();
    for each(System::String^ _element in elems)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmpelems.push_back(_marshalElement);
    }
    auto arg2 = _tmpelems;
    ::StringSplit(arg0, delim, arg2);
}

System::String^ Flood::FloodString::StringFromWideString(System::String^ ws)
{
    auto arg0 = clix::marshalString<clix::E_UTF16>(ws);
    auto __ret = ::StringFromWideString(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::StringToWideString(System::String^ s)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto __ret = ::StringToWideString(arg0);
    return clix::marshalString<clix::E_UTF16>(__ret);
}

System::String^ Flood::FloodString::StringToLowerCase(System::String^ s)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto __ret = ::StringToLowerCase(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::StringToUpperCase(System::String^ s)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto __ret = ::StringToUpperCase(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

void Flood::FloodString::StringReplace(System::String^ s, System::String^ from, System::String^ to)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto arg1 = clix::marshalString<clix::E_UTF8>(from);
    auto arg2 = clix::marshalString<clix::E_UTF8>(to);
    ::StringReplace(arg0, arg1, arg2);
}

System::String^ Flood::FloodString::StringTrim(System::String^ s, System::String^ trim)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto _arg1 = clix::marshalString<clix::E_UTF8>(trim);
    auto arg1 = _arg1.c_str();
    auto __ret = ::StringTrim(arg0, arg1);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::StringFromFloat(float n, unsigned char precision)
{
    auto arg1 = (::uint8)(::uint8_t)precision;
    auto __ret = ::StringFromFloat(n, arg1);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::StringMatch(System::String^ s, System::String^ pattern)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto arg1 = clix::marshalString<clix::E_UTF8>(pattern);
    auto __ret = ::StringMatch(arg0, arg1);
    if (__ret == nullptr) return nullptr;
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::RawStringMatch(System::String^ s, unsigned int len, System::String^ p)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto arg0 = _arg0.c_str();
    auto arg1 = (::size_t)len;
    auto _arg2 = clix::marshalString<clix::E_UTF8>(p);
    auto arg2 = _arg2.c_str();
    auto __ret = ::RawStringMatch(arg0, arg1, arg2);
    if (__ret == nullptr) return nullptr;
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathGetFileBase(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ::PathGetFileBase(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathGetFileExtension(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ::PathGetFileExtension(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathGetBase(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ::PathGetBase(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathGetFile(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ::PathGetFile(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathNormalize(System::String^ path)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(path);
    auto __ret = ::PathNormalize(arg0);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathGetCurrentDir()
{
    auto __ret = ::PathGetCurrentDir();
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathGetSeparator()
{
    auto __ret = ::PathGetSeparator();
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::FloodString::PathCombine(System::String^ base, System::String^ extra)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(base);
    auto arg1 = clix::marshalString<clix::E_UTF8>(extra);
    auto __ret = ::PathCombine(arg0, arg1);
    return clix::marshalString<clix::E_UTF8>(__ret);
}

