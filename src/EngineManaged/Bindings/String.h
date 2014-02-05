/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/String.h>
#include "StringConverter.h"

namespace Flood
{
    value struct StringHash;
}

namespace Flood
{
    public value struct StringHash
    {
    public:

        StringHash(::StringHash* native);
        StringHash(System::IntPtr native);
        StringHash(System::String^ str, unsigned int size);
        unsigned int Hash;
    };
}
