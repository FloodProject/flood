/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Matrix4x4.h"
#include "Matrix4x3.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Matrix4x4::Matrix4x4(::Matrix4x4* native)
{
    M11 = native->m11;
    M12 = native->m12;
    M13 = native->m13;
    M14 = native->m14;
    M21 = native->m21;
    M22 = native->m22;
    M23 = native->m23;
    M24 = native->m24;
    M31 = native->m31;
    M32 = native->m32;
    M33 = native->m33;
    M34 = native->m34;
    Tx = native->tx;
    Ty = native->ty;
    Tz = native->tz;
    Tw = native->tw;
}

Flood::Matrix4x4::Matrix4x4(System::IntPtr native)
{
    auto __native = (::Matrix4x4*)native.ToPointer();
    M11 = __native->m11;
    M12 = __native->m12;
    M13 = __native->m13;
    M14 = __native->m14;
    M21 = __native->m21;
    M22 = __native->m22;
    M23 = __native->m23;
    M24 = __native->m24;
    M31 = __native->m31;
    M32 = __native->m32;
    M33 = __native->m33;
    M34 = __native->m34;
    Tx = __native->tx;
    Ty = __native->ty;
    Tz = __native->tz;
    Tw = __native->tw;
}

Flood::Matrix4x4::Matrix4x4(Flood::Matrix4x3^ _185)
{
}

void Flood::Matrix4x4::Identity()
{
    auto this0 = (::Matrix4x4*) 0;
    this0->identity();
}

Flood::Matrix4x4 Flood::Matrix4x4::Inverse()
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->inverse();
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::Transpose()
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->transpose();
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::operator*(Flood::Matrix4x4 m)
{
    auto this0 = (::Matrix4x4*) 0;
    auto arg0 = ::Matrix4x4();
    arg0.m11 = m.M11;
    arg0.m12 = m.M12;
    arg0.m13 = m.M13;
    arg0.m14 = m.M14;
    arg0.m21 = m.M21;
    arg0.m22 = m.M22;
    arg0.m23 = m.M23;
    arg0.m24 = m.M24;
    arg0.m31 = m.M31;
    arg0.m32 = m.M32;
    arg0.m33 = m.M33;
    arg0.m34 = m.M34;
    arg0.tx = m.Tx;
    arg0.ty = m.Ty;
    arg0.tz = m.Tz;
    arg0.tw = m.Tw;

    auto ret = this0->operator*(arg0);
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Vector3 Flood::Matrix4x4::operator*(Flood::Vector3 v)
{
    auto this0 = (::Matrix4x4*) 0;
    auto arg0 = ::Vector3();
    auto ret = this0->operator*(arg0);
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector4 Flood::Matrix4x4::operator*(Flood::Vector4 v)
{
    auto this0 = (::Matrix4x4*) 0;
    auto arg0 = ::Vector4();
    arg0.x = v.X;
    arg0.y = v.Y;
    arg0.z = v.Z;
    arg0.w = v.W;

    auto ret = this0->operator*(arg0);
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::CreatePerspective(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->createPerspective(FOV, aspectRatio, nearPlane, farPlane);
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    auto this0 = (::Matrix4x4*) 0;
    auto ret = this0->createOrthographic(left, right, bottom, top, nearPlane, farPlane);
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

