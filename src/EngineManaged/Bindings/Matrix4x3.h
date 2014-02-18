/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Matrix4x3.h>

namespace Flood
{
    value struct EulerAngles;
    value struct Matrix4x3;
    value struct Quaternion;
    value struct Vector3;
}

namespace Flood
{
    /// <summary>
    /// Implements a 4x3 transformation matrix. This class can represent any 3D
    /// affine transformation, like scaling, translation, rotation, shearing, etc.
    /// </summary>
    public value struct Matrix4x3
    {
    public:

        Matrix4x3(::Matrix4x3* native);
        Matrix4x3(System::IntPtr native);
        property float M11
        {
            float get();
            void set(float);
        }

        property float M12
        {
            float get();
            void set(float);
        }

        property float M13
        {
            float get();
            void set(float);
        }

        property float M21
        {
            float get();
            void set(float);
        }

        property float M22
        {
            float get();
            void set(float);
        }

        property float M23
        {
            float get();
            void set(float);
        }

        property float M31
        {
            float get();
            void set(float);
        }

        property float M32
        {
            float get();
            void set(float);
        }

        property float M33
        {
            float get();
            void set(float);
        }

        property float Tx
        {
            float get();
            void set(float);
        }

        property float Ty
        {
            float get();
            void set(float);
        }

        property float Tz
        {
            float get();
            void set(float);
        }

        void SetIdentity();

        float Determinant();

        Flood::Matrix4x3 Inverse();

        Flood::Matrix4x3 Transpose();

        static Flood::Vector3 operator*(Flood::Matrix4x3 __op, Flood::Vector3 p);

        static Flood::Matrix4x3 operator*(Flood::Matrix4x3 __op, Flood::Matrix4x3 m);

        static Flood::Matrix4x3 Lerp(Flood::Matrix4x3 a, Flood::Matrix4x3 b, float t);

        static Flood::Matrix4x3 CreateTranslation(Flood::Vector3 translate);

        static Flood::Matrix4x3 CreateRotation(Flood::EulerAngles angles);

        static Flood::Matrix4x3 CreateScale(Flood::Vector3 scale);

        static Flood::Matrix4x3 CreateFromQuaternion(Flood::Quaternion q);

        static Flood::Matrix4x3 RotateX(float ang);

        static Flood::Matrix4x3 RotateY(float ang);

        static Flood::Matrix4x3 RotateZ(float ang);

        static property Flood::Matrix4x3 Identity
        {
            Flood::Matrix4x3 get();
        }

        private:
        float __M11;
        float __M12;
        float __M13;
        float __M21;
        float __M22;
        float __M23;
        float __M31;
        float __M32;
        float __M33;
        float __Tx;
        float __Ty;
        float __Tz;
    };
}
