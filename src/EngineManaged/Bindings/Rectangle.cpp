/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Rectangle.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Rectangle::Rectangle(::Rectangle* native)
{
    X = native->x;
    Y = native->y;
    Width = native->width;
    Height = native->height;
}

Flood::Rectangle::Rectangle(System::IntPtr native)
{
    auto __native = (::Rectangle*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Width = __native->width;
    Height = __native->height;
}

Flood::Rectangle::Rectangle(int x, int y, int width, int height)
{
    auto _native = ::Rectangle(x, y, width, height);
    this->X = _native.x;
    this->Y = _native.y;
    this->Width = _native.width;
    this->Height = _native.height;
}

// Returns true if this rectangle is contained in rect.
// Returns true if this rectangle is contained in rect.
bool Flood::Rectangle::IsContainedIn(Flood::Rectangle rect)
{
    auto _this0 = ::Rectangle();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.width = (*this).Width;
    _this0.height = (*this).Height;
    auto _marshal0 = ::Rectangle();
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

Flood::RectangleF::RectangleF(::RectangleF* native)
{
    X = native->x;
    Y = native->y;
    Width = native->width;
    Height = native->height;
}

Flood::RectangleF::RectangleF(System::IntPtr native)
{
    auto __native = (::RectangleF*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Width = __native->width;
    Height = __native->height;
}

Flood::RectangleF::RectangleF(float x, float y, float width, float height)
{
    auto _native = ::RectangleF(x, y, width, height);
    this->X = _native.x;
    this->Y = _native.y;
    this->Width = _native.width;
    this->Height = _native.height;
}

// Returns true if this rectangle is contained in rect.
// Returns true if this rectangle is contained in rect.
bool Flood::RectangleF::IsContainedIn(Flood::RectangleF rect)
{
    auto _this0 = ::RectangleF();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.width = (*this).Width;
    _this0.height = (*this).Height;
    auto _marshal0 = ::RectangleF();
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

