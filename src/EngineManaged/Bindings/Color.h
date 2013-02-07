/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Color.h>

namespace Flood
{
    value struct ColorP;
    value struct Color;
    value struct Vector3;

    /// <summary>
    /// Representation of colors in RGBA format.
    /// </summary>
    public value struct ColorP
    {
    public:
        ColorP(::ColorP* native);
        ColorP(System::IntPtr native);
        float R;
        float G;
        float B;
        float A;
    };

    public value struct Color
    {
    public:
        Color(::Color* native);
        Color(System::IntPtr native);
        Color(int r, int g, int b, int a);
        bool operator==(Flood::Color v);
        bool NearEqual(Flood::Color a, float tolerance);
        Flood::Color operator*(float s);
    };
}
