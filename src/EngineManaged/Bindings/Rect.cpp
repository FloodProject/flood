/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Rect.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::RectSize::RectSize(::RectSize* native)
{
    Width = native->width;
    Height = native->height;
}

Flood::RectSize::RectSize(System::IntPtr native)
{
    auto __native = (::RectSize*)native.ToPointer();
    Width = __native->width;
    Height = __native->height;
}

Flood::Rect::Rect(::Rect* native)
{
    X = native->x;
    Y = native->y;
    Width = native->width;
    Height = native->height;
}

Flood::Rect::Rect(System::IntPtr native)
{
    auto __native = (::Rect*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Width = __native->width;
    Height = __native->height;
}

// Returns true if a is contained in b.
bool Flood::FloodRect::IsContainedIn(Flood::Rect a, Flood::Rect b)
{
    auto _marshal0 = ::Rect();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.width = a.Width;
    _marshal0.height = a.Height;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Rect();
    _marshal1.x = b.X;
    _marshal1.y = b.Y;
    _marshal1.width = b.Width;
    _marshal1.height = b.Height;
    auto arg1 = _marshal1;
    auto ret = ::IsContainedIn(arg0, arg1);
    return ret;
}

