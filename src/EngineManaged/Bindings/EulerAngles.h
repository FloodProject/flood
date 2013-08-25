/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/EulerAngles.h>

namespace Flood
{
    value struct EulerAngles;
    value struct Vector3;

    /// <summary>
    /// Representation of an orientation as a set of Euler angles that can be
    /// transformed into and back in a rotation matrix and quaternions.
    /// </summary>
    public value struct EulerAngles
    {
    public:

        EulerAngles(::EulerAngles* native);
        EulerAngles(System::IntPtr native);
        EulerAngles(float x, float y, float z);

        float X;
        float Y;
        float Z;
        void Rotate(float x, float y, float z);

        void Identity();

    };
}
