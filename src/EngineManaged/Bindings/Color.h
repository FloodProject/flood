/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Color.h>
#include "Vector.h"

namespace Flood
{
    value struct ColorP;
    value struct Color;
    value struct Vector3;

    public value struct Color
    {
    public:
        Color(::Color* native);
        Color(System::IntPtr native);
        Color(float r, float g, float b, float a);
        Color(int r, int g, int b, int a);
        bool operator==(Flood::Color v);
        bool NearEqual(Flood::Color a, float tolerance);
        Flood::Color operator*(float s);
    };
}
