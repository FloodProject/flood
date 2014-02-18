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
}

namespace Flood
{
    /// <summary>
    /// Implements a 4x4 transformation matrix that can represent the full range of
    /// types of operations that can be used in computer graphics. Usually you just
    /// want to use the 4x3 because it suffices for the most common operations,
    /// like translation, rotation and scaling, but in the case of projections a
    /// full 4x4 matrix is needed.
    /// </summary>
    public value struct Matrix4x4
    {
    public:

        Matrix4x4(::Matrix4x4* native);
        Matrix4x4(System::IntPtr native);
        Matrix4x4(Flood::Matrix4x3 _0);

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

        property float M14
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

        property float M24
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

        property float M34
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

        property float Tw
        {
            float get();
            void set(float);
        }

        void SetIdentity();

        Flood::Matrix4x4 Inverse();

        Flood::Matrix4x4 Transpose();

        static Flood::Matrix4x4 operator*(Flood::Matrix4x4 __op, Flood::Matrix4x4 m);

        static Flood::Vector3 operator*(Flood::Matrix4x4 __op, Flood::Vector3 v);

        static Flood::Vector4 operator*(Flood::Matrix4x4 __op, Flood::Vector4 v);

        static Flood::Matrix4x4 CreatePerspective(float FOV, float aspectRatio, float nearPlane, float farPlane);

        static Flood::Matrix4x4 CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

        static property Flood::Matrix4x4 Identity
        {
            Flood::Matrix4x4 get();
        }

        private:
        float __M11;
        float __M12;
        float __M13;
        float __M14;
        float __M21;
        float __M22;
        float __M23;
        float __M24;
        float __M31;
        float __M32;
        float __M33;
        float __M34;
        float __Tx;
        float __Ty;
        float __Tz;
        float __Tw;
    };
}
