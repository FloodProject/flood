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
    auto ____temp = clix::marshalString<clix::E_UTF8>(str);
    ::StringHash _native(____temp.c_str(), (::size_t)size);
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

