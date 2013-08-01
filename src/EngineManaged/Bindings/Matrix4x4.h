/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Matrix4x4.h>

namespace Flood
{
    value struct Matrix4x3;
    value struct Matrix4x4;
    value struct Vector3;
    value struct Vector4;

    public value struct Matrix4x4
    {
    public:
        Matrix4x4(::Matrix4x4* native);
        Matrix4x4(System::IntPtr native);
        Matrix4x4(Flood::Matrix4x3 _0);
        float M11;
        float M12;
        float M13;
        float M14;
        float M21;
        float M22;
        float M23;
        float M24;
        float M31;
        float M32;
        float M33;
        float M34;
        float Tx;
        float Ty;
        float Tz;
        float Tw;
        void SetIdentity();
        Flood::Matrix4x4 Inverse();
        Flood::Matrix4x4 Transpose();
        Flood::Matrix4x4 operator*(Flood::Matrix4x4 m);
        Flood::Vector3 operator*(Flood::Vector3 v);
        Flood::Vector4 operator*(Flood::Vector4 v);
        static Flood::Matrix4x4 CreatePerspective(float FOV, float aspectRatio, float nearPlane, float farPlane);
        static Flood::Matrix4x4 CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
        static property Flood::Matrix4x4 Identity
        {
            Flood::Matrix4x4 get();
        }
    };
}
