/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/String.h>

namespace Flood
{
    value struct StringHash;
    ref class RawStringCompare;

    public value struct StringHash
    {
    public:
        StringHash(::StringHash* native);
        StringHash(System::IntPtr native);
        unsigned int Hash;
        StringHash(System::String^ str, unsigned int size);
    };


    public ref class FloodString
    {
    public:
        static Flood::StringHash HashString(System::String^ _0);
        static int StringCompareInsensitive(System::String^ s1, System::String^ s2);
        static System::String^ StringFormat(System::String^ s);
        static System::String^ StringFromWideString(System::String^ ws);
        static System::String^ StringToWideString(System::String^ s);
        static System::String^ StringToLowerCase(System::String^ s);
        static System::String^ StringToUpperCase(System::String^ s);
        static void StringReplace(System::String^ s, System::String^ from, System::String^ to);
        static System::String^ StringTrim(System::String^ s, System::String^ trim);
        static System::String^ StringFromFloat(float n, unsigned char precision);
        static System::String^ StringMatch(System::String^ s, System::String^ pattern);
        static System::String^ RawStringMatch(System::String^ s, unsigned int len, System::String^ p);
        static System::String^ PathGetFileBase(System::String^ path);
        static System::String^ PathGetFileExtension(System::String^ path);
        static System::String^ PathGetBase(System::String^ path);
        static System::String^ PathGetFile(System::String^ path);
        static System::String^ PathNormalize(System::String^ path);
        static System::String^ PathGetCurrentDir();
        static System::String^ PathGetSeparator();
        static System::String^ PathCombine(System::String^ base, System::String^ extra);
    };
}
