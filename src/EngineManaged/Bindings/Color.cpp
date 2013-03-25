/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Color.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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

Flood::Color::Color(float r, float g, float b, float a)
{
    auto _native = ::Color(r, g, b, a);
    this->R = _native.r;
    this->G = _native.g;
    this->B = _native.b;
    this->A = _native.a;
}

Flood::Color::Color(int r, int g, int b, int a)
{
    auto _native = ::Color(r, g, b, a);
    this->R = _native.r;
    this->G = _native.g;
    this->B = _native.b;
    this->A = _native.a;
}

bool Flood::Color::operator==(Flood::Color v)
{
    auto this0 = (::Color*) 0;
    auto _marshal0 = ::Color();
    _marshal0.r = v.R;
    _marshal0.g = v.G;
    _marshal0.b = v.B;
    _marshal0.a = v.A;
    auto arg0 = _marshal0;
    auto ret = this0->operator==(arg0);
    return ret;
}

bool Flood::Color::NearEqual(Flood::Color a, float tolerance)
{
    auto this0 = (::Color*) 0;
    auto _marshal0 = ::Color();
    _marshal0.r = a.R;
    _marshal0.g = a.G;
    _marshal0.b = a.B;
    _marshal0.a = a.A;
    auto arg0 = _marshal0;
    auto ret = this0->nearEqual(arg0, tolerance);
    return ret;
}

Flood::Color Flood::Color::operator*(float s)
{
    auto this0 = (::Color*) 0;
    auto ret = this0->operator*(s);
    return Flood::Color((::Color*)&ret);
}

