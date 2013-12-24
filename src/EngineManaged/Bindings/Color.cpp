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
    ::Color _native((::byte)(::uint8)(::uint8_t)r, (::byte)(::uint8)(::uint8_t)g, (::byte)(::uint8)(::uint8_t)b, (::byte)(::uint8)(::uint8_t)a);
    this->R = _native.r;
    this->G = _native.g;
    this->B = _native.b;
    this->A = _native.a;
}

bool Flood::Color::operator==(Flood::Color __op, Flood::Color v)
{
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)__op.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)__op.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)__op.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)__op.A;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Color();
    _marshal1.r = (::byte)(::uint8)(::uint8_t)v.R;
    _marshal1.g = (::byte)(::uint8)(::uint8_t)v.G;
    _marshal1.b = (::byte)(::uint8)(::uint8_t)v.B;
    _marshal1.a = (::byte)(::uint8)(::uint8_t)v.A;
    auto arg1 = _marshal1;
    auto __ret = arg0 == arg1;
    return __ret;
}

bool Flood::Color::NearEqual(Flood::Color a, float tolerance)
{
    auto _this0 = ::Color();
    _this0.r = (::byte)(::uint8)(::uint8_t)(*this).R;
    _this0.g = (::byte)(::uint8)(::uint8_t)(*this).G;
    _this0.b = (::byte)(::uint8)(::uint8_t)(*this).B;
    _this0.a = (::byte)(::uint8)(::uint8_t)(*this).A;
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)a.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)a.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)a.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)a.A;
    auto arg0 = _marshal0;
    auto __ret = _this0.nearEqual(arg0, tolerance);
    R = _this0.r;
    G = _this0.g;
    B = _this0.b;
    A = _this0.a;
    return __ret;
}

Flood::Color Flood::Color::operator*(Flood::Color __op, float s)
{
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)__op.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)__op.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)__op.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)__op.A;
    auto arg0 = _marshal0;
    auto __ret = arg0 * s;
    return Flood::Color((::Color*)&__ret);
}

Flood::Color::operator Flood::Vector3(Flood::Color __op)
{
    auto _marshal0 = ::Color();
    _marshal0.r = (::byte)(::uint8)(::uint8_t)__op.R;
    _marshal0.g = (::byte)(::uint8)(::uint8_t)__op.G;
    _marshal0.b = (::byte)(::uint8)(::uint8_t)__op.B;
    _marshal0.a = (::byte)(::uint8)(::uint8_t)__op.A;
    auto arg0 = _marshal0;
    auto __ret = (::Vector3) arg0;
    return Flood::Vector3((::Vector3*)&__ret);
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

