/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Matrix4x3.h"
#include "EulerAngles.h"
#include "Quaternion.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Matrix4x3::Matrix4x3(::Matrix4x3* native)
{
    M11 = native->m11;
    M12 = native->m12;
    M13 = native->m13;
    M21 = native->m21;
    M22 = native->m22;
    M23 = native->m23;
    M31 = native->m31;
    M32 = native->m32;
    M33 = native->m33;
    Tx = native->tx;
    Ty = native->ty;
    Tz = native->tz;
}

Flood::Matrix4x3::Matrix4x3(System::IntPtr native)
{
    auto __native = (::Matrix4x3*)native.ToPointer();
    M11 = __native->m11;
    M12 = __native->m12;
    M13 = __native->m13;
    M21 = __native->m21;
    M22 = __native->m22;
    M23 = __native->m23;
    M31 = __native->m31;
    M32 = __native->m32;
    M33 = __native->m33;
    Tx = __native->tx;
    Ty = __native->ty;
    Tz = __native->tz;
}

void Flood::Matrix4x3::SetIdentity()
{
    auto this0 = (::Matrix4x3*) 0;
    this0->identity();
}

float Flood::Matrix4x3::Determinant()
{
    auto this0 = (::Matrix4x3*) 0;
    auto ret = this0->determinant();
    return ret;
}

Flood::Matrix4x3 Flood::Matrix4x3::Inverse()
{
    auto this0 = (::Matrix4x3*) 0;
    auto ret = this0->inverse();
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::Transpose()
{
    auto this0 = (::Matrix4x3*) 0;
    auto ret = this0->transpose();
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Vector3 Flood::Matrix4x3::operator*(Flood::Vector3 p)
{
    auto this0 = (::Matrix4x3*) 0;
    auto _marshal0 = ::Vector3();
    _marshal0.x = p.X;
    _marshal0.y = p.Y;
    _marshal0.z = p.Z;
    auto arg0 = _marshal0;
    auto ret = this0->operator*(arg0);
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::operator*(Flood::Matrix4x3 m)
{
    auto this0 = (::Matrix4x3*) 0;
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = m.M11;
    _marshal0.m12 = m.M12;
    _marshal0.m13 = m.M13;
    _marshal0.m21 = m.M21;
    _marshal0.m22 = m.M22;
    _marshal0.m23 = m.M23;
    _marshal0.m31 = m.M31;
    _marshal0.m32 = m.M32;
    _marshal0.m33 = m.M33;
    _marshal0.tx = m.Tx;
    _marshal0.ty = m.Ty;
    _marshal0.tz = m.Tz;
    auto arg0 = _marshal0;
    auto ret = this0->operator*(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::Lerp(Flood::Matrix4x3 a, Flood::Matrix4x3 b, float t)
{
    auto this0 = (::Matrix4x3*) 0;
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = a.M11;
    _marshal0.m12 = a.M12;
    _marshal0.m13 = a.M13;
    _marshal0.m21 = a.M21;
    _marshal0.m22 = a.M22;
    _marshal0.m23 = a.M23;
    _marshal0.m31 = a.M31;
    _marshal0.m32 = a.M32;
    _marshal0.m33 = a.M33;
    _marshal0.tx = a.Tx;
    _marshal0.ty = a.Ty;
    _marshal0.tz = a.Tz;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Matrix4x3();
    _marshal1.m11 = b.M11;
    _marshal1.m12 = b.M12;
    _marshal1.m13 = b.M13;
    _marshal1.m21 = b.M21;
    _marshal1.m22 = b.M22;
    _marshal1.m23 = b.M23;
    _marshal1.m31 = b.M31;
    _marshal1.m32 = b.M32;
    _marshal1.m33 = b.M33;
    _marshal1.tx = b.Tx;
    _marshal1.ty = b.Ty;
    _marshal1.tz = b.Tz;
    auto arg1 = _marshal1;
    auto ret = this0->lerp(arg0, arg1, t);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateTranslation(Flood::Vector3 translate)
{
    auto this0 = (::Matrix4x3*) 0;
    auto _marshal0 = ::Vector3();
    _marshal0.x = translate.X;
    _marshal0.y = translate.Y;
    _marshal0.z = translate.Z;
    auto arg0 = _marshal0;
    auto ret = this0->createTranslation(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateRotation(Flood::EulerAngles angles)
{
    auto this0 = (::Matrix4x3*) 0;
    auto _marshal0 = ::EulerAngles();
    _marshal0.x = angles.X;
    _marshal0.y = angles.Y;
    _marshal0.z = angles.Z;
    auto arg0 = _marshal0;
    auto ret = this0->createRotation(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateScale(Flood::Vector3 scale)
{
    auto this0 = (::Matrix4x3*) 0;
    auto _marshal0 = ::Vector3();
    _marshal0.x = scale.X;
    _marshal0.y = scale.Y;
    _marshal0.z = scale.Z;
    auto arg0 = _marshal0;
    auto ret = this0->createScale(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateFromQuaternion(Flood::Quaternion q)
{
    auto this0 = (::Matrix4x3*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = this0->createFromQuaternion(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::RotateX(float ang)
{
    auto this0 = (::Matrix4x3*) 0;
    auto ret = this0->rotateX(ang);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::RotateY(float ang)
{
    auto this0 = (::Matrix4x3*) 0;
    auto ret = this0->rotateY(ang);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::RotateZ(float ang)
{
    auto this0 = (::Matrix4x3*) 0;
    auto ret = this0->rotateZ(ang);
    return Flood::Matrix4x3((::Matrix4x3*)&ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::Identity::get()
{
    return Flood::Matrix4x3((::Matrix4x3*)&::Matrix4x3::Identity);
}

