/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Helpers.h>
#include "ResourceHandle.h"

namespace Flood
{
    public ref class FloodHelpers
    {
    public:
        static bool MathFloatCompare(float a, float b);
        static float MathFloatRandom(float min, float max);
        static float MathLogBase(float base, float value);
        static float MathDegreeToRadian(float degree);
        static float MathRadianToDegree(float radian);
        static float InterpolateLinear(float a, float b, float t);
        static float InterpolateCosine(float a, float b, float t);
        static float InterpolateCubic(float a, float b, float t);
    };
}
