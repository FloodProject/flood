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
}

namespace Flood
{
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

        property float X
        {
            float get();
            void set(float);
        }

        property float Y
        {
            float get();
            void set(float);
        }

        property float Z
        {
            float get();
            void set(float);
        }

        void Rotate(float x, float y, float z);

        void Identity();

        private:
        float __X;
        float __Y;
        float __Z;
    };
}
