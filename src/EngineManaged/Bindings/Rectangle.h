/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Rectangle.h>

namespace Flood
{
    value struct Rectangle;
    value struct RectangleF;

    public value struct Rectangle
    {
    public:
        Rectangle(::Rectangle* native);
        Rectangle(System::IntPtr native);
        Rectangle(int x, int y, int width, int height);
        int X;
        int Y;
        int Width;
        int Height;
        bool IsContainedIn(Flood::Rectangle rect);
    };

    public value struct RectangleF
    {
    public:
        RectangleF(::RectangleF* native);
        RectangleF(System::IntPtr native);
        RectangleF(float x, float y, float width, float height);
        float X;
        float Y;
        float Width;
        float Height;
        bool IsContainedIn(Flood::RectangleF rect);
    };
}
