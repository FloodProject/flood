/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Color.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Color::Color(int r, int g, int b, int a)
{
}

bool Flood::Color::operator==(Flood::Color v)
{
    return false;
}

bool Flood::Color::NearEqual(Flood::Color a, float tolerance)
{
    return false;
}

Flood::Color Flood::Color::operator*(float s)
{
    return Color();
}


