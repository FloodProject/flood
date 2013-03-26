/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/RectangleBinPack/Rect.h>
#include "ResourceHandle.h"

namespace Flood
{
    value struct Rect;
    value struct RectSize;

    public value struct RectSize
    {
    public:
        RectSize(::RectSize* native);
        RectSize(System::IntPtr native);
        int Width;
        int Height;
    };

    public value struct Rect
    {
    public:
        Rect(::Rect* native);
        Rect(System::IntPtr native);
        int X;
        int Y;
        int Width;
        int Height;
    };

    public ref class FloodRect
    {
    public:
        /// <summary>
        /// Returns true if a is contained in b.
        /// </summary>
        static bool IsContainedIn(Flood::Rect a, Flood::Rect b);
    };
}
