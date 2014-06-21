/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Helpers.h"

using namespace System;
using namespace System::Runtime::InteropServices;

bool Flood::Helpers::MathFloatCompare(float a, float b)
{
    auto __ret = ::MathFloatCompare(a, b);
    return __ret;
}

float Flood::Helpers::MathFloatRandom(float min, float max)
{
    auto __ret = ::MathFloatRandom(min, max);
    return __ret;
}

float Flood::Helpers::MathLogBase(float base, float value)
{
    auto __ret = ::MathLogBase(base, value);
    return __ret;
}

float Flood::Helpers::MathDegreeToRadian(float degree)
{
    auto __ret = ::MathDegreeToRadian(degree);
    return __ret;
}

float Flood::Helpers::MathRadianToDegree(float radian)
{
    auto __ret = ::MathRadianToDegree(radian);
    return __ret;
}

float Flood::Helpers::InterpolateLinear(float a, float b, float t)
{
    auto __ret = ::InterpolateLinear(a, b, t);
    return __ret;
}

float Flood::Helpers::InterpolateCosine(float a, float b, float t)
{
    auto __ret = ::InterpolateCosine(a, b, t);
    return __ret;
}

float Flood::Helpers::InterpolateCubic(float a, float b, float t)
{
    auto __ret = ::InterpolateCubic(a, b, t);
    return __ret;
}

