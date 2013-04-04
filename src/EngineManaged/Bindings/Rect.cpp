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

Flood::Rect::Rect(int x, int y, int width, int height)
{
    auto _native = ::Rect(x, y, width, height);
    this->X = _native.x;
    this->Y = _native.y;
    this->Width = _native.width;
    this->Height = _native.height;
}

bool Flood::Rect::IsContainedIn(Flood::Rect rect)
{
    auto _this0 = ::Rect();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.width = (*this).Width;
    _this0.height = (*this).Height;
    auto _marshal0 = ::Rect();
    _marshal0.x = rect.X;
    _marshal0.y = rect.Y;
    _marshal0.width = rect.Width;
    _marshal0.height = rect.Height;
    auto arg0 = _marshal0;
    auto ret = _this0.isContainedIn(arg0);
    X = _this0.x;
    Y = _this0.y;
    Width = _this0.width;
    Height = _this0.height;
    return ret;
}

