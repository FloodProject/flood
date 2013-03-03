/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Matrix4x3.h>
#include "Vector.h"
#include "Quaternion.h"

namespace Flood
{
    ref class Matrix4x3;
    value struct Vector3;
    value struct EulerAngles;
    value struct Quaternion;

    /// <summary>
    /// Implements a 4x3 transformation matrix. This class can represent any 3D
    /// affine transformation, like scaling, translation, rotation, shearing, etc.
    /// </summary>
    public ref class Matrix4x3
    {
    public:
        property ::Matrix4x3* NativePtr;

        Matrix4x3(::Matrix4x3* native);
        Matrix4x3(System::IntPtr native);
        Matrix4x3();
        property float M11;
        property float M12;
        property float M13;
        property float M21;
        property float M22;
        property float M23;
        property float M31;
        property float M32;
        property float M33;
        property float Tx;
        property float Ty;
        property float Tz;
        void Identity();
        float Determinant();
        Flood::Matrix4x3^ Inverse();
        Flood::Matrix4x3^ Transpose();
        Flood::Vector3 operator*(Flood::Vector3 p);
        Flood::Matrix4x3^ operator*(Flood::Matrix4x3^ m);
        Flood::Matrix4x3^ Lerp(Flood::Matrix4x3^ a, Flood::Matrix4x3^ b, float t);
        Flood::Matrix4x3^ CreateTranslation(Flood::Vector3 translate);
        Flood::Matrix4x3^ CreateRotation(Flood::EulerAngles angles);
        Flood::Matrix4x3^ CreateScale(Flood::Vector3 scale);
        Flood::Matrix4x3^ CreateFromQuaternion(Flood::Quaternion q);
        Flood::Matrix4x3^ RotateX(float ang);
        Flood::Matrix4x3^ RotateY(float ang);
        Flood::Matrix4x3^ RotateZ(float ang);
    };
}
