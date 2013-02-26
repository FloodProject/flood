/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Color.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Color::Color(::Color* native)
{
    // TODO: Struct marshaling
}

Flood::Color::Color(System::IntPtr native)
{
    // TODO: Struct marshaling
}

Flood::Color::Color(float r, float g, float b, float a)
{
}

Flood::Color::Color(int r, int g, int b, int a)
{
}

bool Flood::Color::operator==(Flood::Color v)
{
    auto this0 = (::Color*) 0;
    auto _arg0 = (::Color*)&v;
    auto arg0 = *_arg0;
    auto ret = this0->operator==(arg0);
    return ret;
}

bool Flood::Color::NearEqual(Flood::Color a, float tolerance)
{
    auto this0 = (::Color*) 0;
    auto _arg0 = (::Color*)&a;
    auto arg0 = *_arg0;
    auto ret = this0->nearEqual(arg0, tolerance);
    return ret;
}

Flood::Color Flood::Color::operator*(float s)
{
    auto this0 = (::Color*) 0;
    auto ret = this0->operator*(s);
    return Flood::Color((::Color*)&ret);
}

