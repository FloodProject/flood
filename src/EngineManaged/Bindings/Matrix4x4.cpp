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

Flood::Matrix4x4::Matrix4x4(Flood::Matrix4x3 _0)
{
    auto _marshal0 = ::Matrix4x3();
    auto _native = ::Matrix4x4(_marshal0);
    this->M11 = _native.m11;
    this->M12 = _native.m12;
    this->M13 = _native.m13;
    this->M14 = _native.m14;
    this->M21 = _native.m21;
    this->M22 = _native.m22;
    this->M23 = _native.m23;
    this->M24 = _native.m24;
    this->M31 = _native.m31;
    this->M32 = _native.m32;
    this->M33 = _native.m33;
    this->M34 = _native.m34;
    this->Tx = _native.tx;
    this->Ty = _native.ty;
    this->Tz = _native.tz;
    this->Tw = _native.tw;
}

void Flood::Matrix4x4::SetIdentity()
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
    auto _marshal0 = ::Matrix4x4();
    _marshal0.m11 = m.M11;
    _marshal0.m12 = m.M12;
    _marshal0.m13 = m.M13;
    _marshal0.m14 = m.M14;
    _marshal0.m21 = m.M21;
    _marshal0.m22 = m.M22;
    _marshal0.m23 = m.M23;
    _marshal0.m24 = m.M24;
    _marshal0.m31 = m.M31;
    _marshal0.m32 = m.M32;
    _marshal0.m33 = m.M33;
    _marshal0.m34 = m.M34;
    _marshal0.tx = m.Tx;
    _marshal0.ty = m.Ty;
    _marshal0.tz = m.Tz;
    _marshal0.tw = m.Tw;
    auto arg0 = _marshal0;
    auto ret = this0->operator*(arg0);
    return Flood::Matrix4x4((::Matrix4x4*)&ret);
}

Flood::Vector3 Flood::Matrix4x4::operator*(Flood::Vector3 v)
{
    auto this0 = (::Matrix4x4*) 0;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = this0->operator*(arg0);
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector4 Flood::Matrix4x4::operator*(Flood::Vector4 v)
{
    auto this0 = (::Matrix4x4*) 0;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
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

Flood::Matrix4x4 Flood::Matrix4x4::Identity::get()
{
    return Flood::Matrix4x4((::Matrix4x4*)&::Matrix4x4::Identity);
}

