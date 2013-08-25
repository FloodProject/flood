/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Color.h>

namespace Flood
{
    value struct Color;
    value struct Vector3;

    public value struct Color
    {
    public:

        Color(::Color* native);
        Color(System::IntPtr native);
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;
        static bool operator==(Flood::Color __op, Flood::Color v);

        bool NearEqual(Flood::Color a, float tolerance);

        static Flood::Color operator*(Flood::Color __op, float s);

        static property Flood::Color White
        {
            Flood::Color get();
        }
        static property Flood::Color Black
        {
            Flood::Color get();
        }
        static property Flood::Color LightGrey
        {
            Flood::Color get();
        }
        static property Flood::Color Red
        {
            Flood::Color get();
        }
        static property Flood::Color Green
        {
            Flood::Color get();
        }
        static property Flood::Color Blue
        {
            Flood::Color get();
        }
        static property Flood::Color NavyBlue
        {
            Flood::Color get();
        }
        static property Flood::Color SkyBlue
        {
            Flood::Color get();
        }
        static property Flood::Color Yellow
        {
            Flood::Color get();
        }
    };
}
