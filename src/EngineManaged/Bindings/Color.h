/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Color.h>
#include "ResourceHandle.h"

namespace Flood
{
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
        static property Flood::Color White;
        static property Flood::Color Black;
        static property Flood::Color LightGrey;
        static property Flood::Color Red;
        static property Flood::Color Green;
        static property Flood::Color Blue;
        static property Flood::Color NavyBlue;
        static property Flood::Color SkyBlue;
        static property Flood::Color Yellow;
    };
}
