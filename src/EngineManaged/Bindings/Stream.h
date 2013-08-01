/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Stream.h>

namespace Flood
{
    enum struct StreamOpenMode;
    enum struct StreamSeekMode;
    ref class Allocator;

    public enum struct StreamOpenMode
    {
        Read = 0,
        Write = 1,
        Append = 2
    };

    public enum struct StreamSeekMode
    {
        Absolute = 0,
        Relative = 1,
        RelativeEnd = 2
    };
}
