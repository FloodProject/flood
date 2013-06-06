/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Matrix4x3.h>
#include "ResourceHandle.h"

namespace Flood
{
    value struct EulerAngles;
    value struct Matrix4x3;
    value struct Quaternion;
    value struct Vector3;

    /// <summary>
    /// Implements a 4x3 transformation matrix. This class can represent any 3D
    /// affine transformation, like scaling, translation, rotation, shearing, etc.
    /// </summary>
    public value struct Matrix4x3
    {
    public:
        Matrix4x3(::Matrix4x3* native);
        Matrix4x3(System::IntPtr native);
        float M11;
        float M12;
        float M13;
        float M21;
        float M22;
        float M23;
        float M31;
        float M32;
        float M33;
        float Tx;
        float Ty;
        float Tz;
        void SetIdentity();
        float Determinant();
        Flood::Matrix4x3 Inverse();
        Flood::Matrix4x3 Transpose();
        Flood::Vector3 operator*(Flood::Vector3 p);
        Flood::Matrix4x3 operator*(Flood::Matrix4x3 m);
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
    };
}
