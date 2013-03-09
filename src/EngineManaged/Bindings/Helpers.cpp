/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Helpers.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

bool Flood::FloodHelpers::MathFloatCompare(float a, float b)
{
    auto ret = ::MathFloatCompare(a, b);
    return ret;
}

float Flood::FloodHelpers::MathFloatRandom(float min, float max)
{
    auto ret = ::MathFloatRandom(min, max);
    return ret;
}

float Flood::FloodHelpers::MathLogBase(float base, float value)
{
    auto ret = ::MathLogBase(base, value);
    return ret;
}

float Flood::FloodHelpers::MathDegreeToRadian(float degree)
{
    auto ret = ::MathDegreeToRadian(degree);
    return ret;
}

float Flood::FloodHelpers::MathRadianToDegree(float radian)
{
    auto ret = ::MathRadianToDegree(radian);
    return ret;
}

float Flood::FloodHelpers::InterpolateLinear(float a, float b, float t)
{
    auto ret = ::InterpolateLinear(a, b, t);
    return ret;
}

float Flood::FloodHelpers::InterpolateCosine(float a, float b, float t)
{
    auto ret = ::InterpolateCosine(a, b, t);
    return ret;
}

float Flood::FloodHelpers::InterpolateCubic(float a, float b, float t)
{
    auto ret = ::InterpolateCubic(a, b, t);
    return ret;
}

