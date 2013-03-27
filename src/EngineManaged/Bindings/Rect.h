/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Rect.h>
#include "ResourceHandle.h"

namespace Flood
{
    value struct Rect;

    public value struct Rect
    {
    public:
        Rect(::Rect* native);
        Rect(System::IntPtr native);
        Rect(int x, int y, int width, int height);
        int X;
        int Y;
        int Width;
        int Height;
        bool IsContainedIn(Flood::Rect rect);
    };
}
