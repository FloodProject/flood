/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Color.h"
#include "RenderContext.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Color::Color(::Color* native)
{
    R = native->r;
    G = native->g;
    B = native->b;
    A = native->a;
}

Flood::Color::Color(System::IntPtr native)
{
    auto __native = (::Color*)native.ToPointer();
    R = __native->r;
    G = __native->g;
    B = __native->b;
    A = __native->a;
}

Flood::Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    auto _native = ::Color((byte)(uint8)r, (byte)(uint8)g, (byte)(uint8)b, (byte)(uint8)a);
    this->R = _native.r;
    this->G = _native.g;
    this->B = _native.b;
    this->A = _native.a;
}

bool Flood::Color::operator==(Flood::Color v)
{
    auto _this0 = ::Color();
    _this0.r = (byte)(uint8)(*this).R;
    _this0.g = (byte)(uint8)(*this).G;
    _this0.b = (byte)(uint8)(*this).B;
    _this0.a = (byte)(uint8)(*this).A;
    auto _marshal0 = ::Color();
    _marshal0.r = (byte)(uint8)v.R;
    _marshal0.g = (byte)(uint8)v.G;
    _marshal0.b = (byte)(uint8)v.B;
    _marshal0.a = (byte)(uint8)v.A;
    auto arg0 = _marshal0;
    auto ret = _this0.operator==(arg0);
    R = _this0.r;
    G = _this0.g;
    B = _this0.b;
    A = _this0.a;
    return ret;
}

bool Flood::Color::NearEqual(Flood::Color a, float tolerance)
{
    auto _this0 = ::Color();
    _this0.r = (byte)(uint8)(*this).R;
    _this0.g = (byte)(uint8)(*this).G;
    _this0.b = (byte)(uint8)(*this).B;
    _this0.a = (byte)(uint8)(*this).A;
    auto _marshal0 = ::Color();
    _marshal0.r = (byte)(uint8)a.R;
    _marshal0.g = (byte)(uint8)a.G;
    _marshal0.b = (byte)(uint8)a.B;
    _marshal0.a = (byte)(uint8)a.A;
    auto arg0 = _marshal0;
    auto ret = _this0.nearEqual(arg0, tolerance);
    R = _this0.r;
    G = _this0.g;
    B = _this0.b;
    A = _this0.a;
    return ret;
}

Flood::Color Flood::Color::operator*(float s)
{
    auto _this0 = ::Color();
    _this0.r = (byte)(uint8)(*this).R;
    _this0.g = (byte)(uint8)(*this).G;
    _this0.b = (byte)(uint8)(*this).B;
    _this0.a = (byte)(uint8)(*this).A;
    auto ret = _this0.operator*(s);
    R = _this0.r;
    G = _this0.g;
    B = _this0.b;
    A = _this0.a;
    return Flood::Color((::Color*)&ret);
}

Flood::Color Flood::Color::White::get()
{
    return Flood::Color((::Color*)&::Color::White);
}

Flood::Color Flood::Color::Black::get()
{
    return Flood::Color((::Color*)&::Color::Black);
}

Flood::Color Flood::Color::LightGrey::get()
{
    return Flood::Color((::Color*)&::Color::LightGrey);
}

Flood::Color Flood::Color::Red::get()
{
    return Flood::Color((::Color*)&::Color::Red);
}

Flood::Color Flood::Color::Green::get()
{
    return Flood::Color((::Color*)&::Color::Green);
}

Flood::Color Flood::Color::Blue::get()
{
    return Flood::Color((::Color*)&::Color::Blue);
}

Flood::Color Flood::Color::NavyBlue::get()
{
    return Flood::Color((::Color*)&::Color::NavyBlue);
}

Flood::Color Flood::Color::SkyBlue::get()
{
    return Flood::Color((::Color*)&::Color::SkyBlue);
}

Flood::Color Flood::Color::Yellow::get()
{
    return Flood::Color((::Color*)&::Color::Yellow);
}

