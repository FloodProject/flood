/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Rectangle.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Rectangle::Rectangle(::Rectangle* native)
{
    __X = native->x;
    __Y = native->y;
    __Width = native->width;
    __Height = native->height;
}

Flood::Rectangle::Rectangle(System::IntPtr native)
{
    auto __native = (::Rectangle*)native.ToPointer();
    __X = __native->x;
    __Y = __native->y;
    __Width = __native->width;
    __Height = __native->height;
}

Flood::Rectangle::Rectangle(int x, int y, int width, int height)
{
    ::Rectangle _native(x, y, width, height);
    this->X = _native.x;
    this->Y = _native.y;
    this->Width = _native.width;
    this->Height = _native.height;
}

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
    auto __ret = _this0.isContainedIn(arg0);
    __X = _this0.x;
    __Y = _this0.y;
    __Width = _this0.width;
    __Height = _this0.height;
    return __ret;
}

int Flood::Rectangle::X::get()
{
    return __X;
}

void Flood::Rectangle::X::set(int value)
{
    __X = value;
}

int Flood::Rectangle::Y::get()
{
    return __Y;
}

void Flood::Rectangle::Y::set(int value)
{
    __Y = value;
}

int Flood::Rectangle::Width::get()
{
    return __Width;
}

void Flood::Rectangle::Width::set(int value)
{
    __Width = value;
}

int Flood::Rectangle::Height::get()
{
    return __Height;
}

void Flood::Rectangle::Height::set(int value)
{
    __Height = value;
}

Flood::RectangleF::RectangleF(::RectangleF* native)
{
    __X = native->x;
    __Y = native->y;
    __Width = native->width;
    __Height = native->height;
}

Flood::RectangleF::RectangleF(System::IntPtr native)
{
    auto __native = (::RectangleF*)native.ToPointer();
    __X = __native->x;
    __Y = __native->y;
    __Width = __native->width;
    __Height = __native->height;
}

Flood::RectangleF::RectangleF(float x, float y, float width, float height)
{
    ::RectangleF _native(x, y, width, height);
    this->X = _native.x;
    this->Y = _native.y;
    this->Width = _native.width;
    this->Height = _native.height;
}

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
    auto __ret = _this0.isContainedIn(arg0);
    __X = _this0.x;
    __Y = _this0.y;
    __Width = _this0.width;
    __Height = _this0.height;
    return __ret;
}

float Flood::RectangleF::X::get()
{
    return __X;
}

void Flood::RectangleF::X::set(float value)
{
    __X = value;
}

float Flood::RectangleF::Y::get()
{
    return __Y;
}

void Flood::RectangleF::Y::set(float value)
{
    __Y = value;
}

float Flood::RectangleF::Width::get()
{
    return __Width;
}

void Flood::RectangleF::Width::set(float value)
{
    __Width = value;
}

float Flood::RectangleF::Height::get()
{
    return __Height;
}

void Flood::RectangleF::Height::set(float value)
{
    __Height = value;
}

