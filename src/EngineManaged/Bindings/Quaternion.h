/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Quaternion.h>

namespace Flood
{
    value struct EulerAngles;
    value struct Quaternion;
    value struct Vector3;
}

namespace Flood
{
    public value struct Quaternion
    {
    public:

        Quaternion(::Quaternion* native);
        Quaternion(System::IntPtr native);
        Quaternion(float u);

        Quaternion(float a, float b, float c, float d);

        Quaternion(Flood::EulerAngles angles);

        property float ToRotateAboutX
        {
            void set(float);
        }

        property float ToRotateAboutY
        {
            void set(float);
        }

        property float ToRotateAboutZ
        {
            void set(float);
        }

        property float RotationAngle
        {
            float get();
        }

        property Flood::Vector3 RotationAxis
        {
            Flood::Vector3 get();
        }

        property Flood::EulerAngles EulerAngles
        {
            Flood::EulerAngles get();
        }

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

        property float W
        {
            float get();
            void set(float);
        }

        static bool operator==(Flood::Quaternion __op, Flood::Quaternion q);

        static bool operator!=(Flood::Quaternion __op, Flood::Quaternion q);

        void SetIdentity();

        Flood::Quaternion Conjugate(Flood::Quaternion q);

        static Flood::Quaternion operator*(Flood::Quaternion __op, Flood::Quaternion a);

        float Magnitude();

        void Normalize();

        void SetToRotateAboutAxis(Flood::Vector3 axis, float theta);

        float SafeAcos(float x);

        float Dot(Flood::Quaternion a);

        Flood::Quaternion Slerp(Flood::Quaternion q, float t);

        static property Flood::Quaternion Identity
        {
            Flood::Quaternion get();
        }

        private:
        float __X;
        float __Y;
        float __Z;
        float __W;
    };
}
