/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Matrix4x3.h"
#include "EulerAngles.h"
#include "Quaternion.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Matrix4x3::Matrix4x3(::Matrix4x3* native)
{
    __M11 = native->m11;
    __M12 = native->m12;
    __M13 = native->m13;
    __M21 = native->m21;
    __M22 = native->m22;
    __M23 = native->m23;
    __M31 = native->m31;
    __M32 = native->m32;
    __M33 = native->m33;
    __Tx = native->tx;
    __Ty = native->ty;
    __Tz = native->tz;
}

Flood::Matrix4x3::Matrix4x3(System::IntPtr native)
{
    auto __native = (::Matrix4x3*)native.ToPointer();
    __M11 = __native->m11;
    __M12 = __native->m12;
    __M13 = __native->m13;
    __M21 = __native->m21;
    __M22 = __native->m22;
    __M23 = __native->m23;
    __M31 = __native->m31;
    __M32 = __native->m32;
    __M33 = __native->m33;
    __Tx = __native->tx;
    __Ty = __native->ty;
    __Tz = __native->tz;
}

void Flood::Matrix4x3::SetIdentity()
{
    auto _this0 = ::Matrix4x3();
    _this0.m11 = (*this).M11;
    _this0.m12 = (*this).M12;
    _this0.m13 = (*this).M13;
    _this0.m21 = (*this).M21;
    _this0.m22 = (*this).M22;
    _this0.m23 = (*this).M23;
    _this0.m31 = (*this).M31;
    _this0.m32 = (*this).M32;
    _this0.m33 = (*this).M33;
    _this0.tx = (*this).Tx;
    _this0.ty = (*this).Ty;
    _this0.tz = (*this).Tz;
    _this0.identity();
    __M11 = _this0.m11;
    __M12 = _this0.m12;
    __M13 = _this0.m13;
    __M21 = _this0.m21;
    __M22 = _this0.m22;
    __M23 = _this0.m23;
    __M31 = _this0.m31;
    __M32 = _this0.m32;
    __M33 = _this0.m33;
    __Tx = _this0.tx;
    __Ty = _this0.ty;
    __Tz = _this0.tz;
}

float Flood::Matrix4x3::Determinant()
{
    auto _this0 = ::Matrix4x3();
    _this0.m11 = (*this).M11;
    _this0.m12 = (*this).M12;
    _this0.m13 = (*this).M13;
    _this0.m21 = (*this).M21;
    _this0.m22 = (*this).M22;
    _this0.m23 = (*this).M23;
    _this0.m31 = (*this).M31;
    _this0.m32 = (*this).M32;
    _this0.m33 = (*this).M33;
    _this0.tx = (*this).Tx;
    _this0.ty = (*this).Ty;
    _this0.tz = (*this).Tz;
    auto __ret = _this0.determinant();
    __M11 = _this0.m11;
    __M12 = _this0.m12;
    __M13 = _this0.m13;
    __M21 = _this0.m21;
    __M22 = _this0.m22;
    __M23 = _this0.m23;
    __M31 = _this0.m31;
    __M32 = _this0.m32;
    __M33 = _this0.m33;
    __Tx = _this0.tx;
    __Ty = _this0.ty;
    __Tz = _this0.tz;
    return __ret;
}

Flood::Matrix4x3 Flood::Matrix4x3::Inverse()
{
    auto _this0 = ::Matrix4x3();
    _this0.m11 = (*this).M11;
    _this0.m12 = (*this).M12;
    _this0.m13 = (*this).M13;
    _this0.m21 = (*this).M21;
    _this0.m22 = (*this).M22;
    _this0.m23 = (*this).M23;
    _this0.m31 = (*this).M31;
    _this0.m32 = (*this).M32;
    _this0.m33 = (*this).M33;
    _this0.tx = (*this).Tx;
    _this0.ty = (*this).Ty;
    _this0.tz = (*this).Tz;
    auto __ret = _this0.inverse();
    __M11 = _this0.m11;
    __M12 = _this0.m12;
    __M13 = _this0.m13;
    __M21 = _this0.m21;
    __M22 = _this0.m22;
    __M23 = _this0.m23;
    __M31 = _this0.m31;
    __M32 = _this0.m32;
    __M33 = _this0.m33;
    __Tx = _this0.tx;
    __Ty = _this0.ty;
    __Tz = _this0.tz;
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::Transpose()
{
    auto _this0 = ::Matrix4x3();
    _this0.m11 = (*this).M11;
    _this0.m12 = (*this).M12;
    _this0.m13 = (*this).M13;
    _this0.m21 = (*this).M21;
    _this0.m22 = (*this).M22;
    _this0.m23 = (*this).M23;
    _this0.m31 = (*this).M31;
    _this0.m32 = (*this).M32;
    _this0.m33 = (*this).M33;
    _this0.tx = (*this).Tx;
    _this0.ty = (*this).Ty;
    _this0.tz = (*this).Tz;
    auto __ret = _this0.transpose();
    __M11 = _this0.m11;
    __M12 = _this0.m12;
    __M13 = _this0.m13;
    __M21 = _this0.m21;
    __M22 = _this0.m22;
    __M23 = _this0.m23;
    __M31 = _this0.m31;
    __M32 = _this0.m32;
    __M33 = _this0.m33;
    __Tx = _this0.tx;
    __Ty = _this0.ty;
    __Tz = _this0.tz;
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Vector3 Flood::Matrix4x3::operator*(Flood::Matrix4x3 __op, Flood::Vector3 p)
{
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = __op.M11;
    _marshal0.m12 = __op.M12;
    _marshal0.m13 = __op.M13;
    _marshal0.m21 = __op.M21;
    _marshal0.m22 = __op.M22;
    _marshal0.m23 = __op.M23;
    _marshal0.m31 = __op.M31;
    _marshal0.m32 = __op.M32;
    _marshal0.m33 = __op.M33;
    _marshal0.tx = __op.Tx;
    _marshal0.ty = __op.Ty;
    _marshal0.tz = __op.Tz;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = p.X;
    _marshal1.y = p.Y;
    _marshal1.z = p.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 * arg1;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::operator*(Flood::Matrix4x3 __op, Flood::Matrix4x3 m)
{
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = __op.M11;
    _marshal0.m12 = __op.M12;
    _marshal0.m13 = __op.M13;
    _marshal0.m21 = __op.M21;
    _marshal0.m22 = __op.M22;
    _marshal0.m23 = __op.M23;
    _marshal0.m31 = __op.M31;
    _marshal0.m32 = __op.M32;
    _marshal0.m33 = __op.M33;
    _marshal0.tx = __op.Tx;
    _marshal0.ty = __op.Ty;
    _marshal0.tz = __op.Tz;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Matrix4x3();
    _marshal1.m11 = m.M11;
    _marshal1.m12 = m.M12;
    _marshal1.m13 = m.M13;
    _marshal1.m21 = m.M21;
    _marshal1.m22 = m.M22;
    _marshal1.m23 = m.M23;
    _marshal1.m31 = m.M31;
    _marshal1.m32 = m.M32;
    _marshal1.m33 = m.M33;
    _marshal1.tx = m.Tx;
    _marshal1.ty = m.Ty;
    _marshal1.tz = m.Tz;
    auto arg1 = _marshal1;
    auto __ret = arg0 * arg1;
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::Lerp(Flood::Matrix4x3 a, Flood::Matrix4x3 b, float t)
{
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
    auto __ret = ::Matrix4x3::lerp(arg0, arg1, t);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateTranslation(Flood::Vector3 translate)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = translate.X;
    _marshal0.y = translate.Y;
    _marshal0.z = translate.Z;
    auto arg0 = _marshal0;
    auto __ret = ::Matrix4x3::createTranslation(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateRotation(Flood::EulerAngles angles)
{
    auto _marshal0 = ::EulerAngles();
    _marshal0.x = angles.X;
    _marshal0.y = angles.Y;
    _marshal0.z = angles.Z;
    auto arg0 = _marshal0;
    auto __ret = ::Matrix4x3::createRotation(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateScale(Flood::Vector3 scale)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = scale.X;
    _marshal0.y = scale.Y;
    _marshal0.z = scale.Z;
    auto arg0 = _marshal0;
    auto __ret = ::Matrix4x3::createScale(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::CreateFromQuaternion(Flood::Quaternion q)
{
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto __ret = ::Matrix4x3::createFromQuaternion(arg0);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::RotateX(float ang)
{
    auto __ret = ::Matrix4x3::rotateX(ang);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::RotateY(float ang)
{
    auto __ret = ::Matrix4x3::rotateY(ang);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Matrix4x3::RotateZ(float ang)
{
    auto __ret = ::Matrix4x3::rotateZ(ang);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

float Flood::Matrix4x3::M11::get()
{
    return __M11;
}

void Flood::Matrix4x3::M11::set(float value)
{
    __M11 = value;
}

float Flood::Matrix4x3::M12::get()
{
    return __M12;
}

void Flood::Matrix4x3::M12::set(float value)
{
    __M12 = value;
}

float Flood::Matrix4x3::M13::get()
{
    return __M13;
}

void Flood::Matrix4x3::M13::set(float value)
{
    __M13 = value;
}

float Flood::Matrix4x3::M21::get()
{
    return __M21;
}

void Flood::Matrix4x3::M21::set(float value)
{
    __M21 = value;
}

float Flood::Matrix4x3::M22::get()
{
    return __M22;
}

void Flood::Matrix4x3::M22::set(float value)
{
    __M22 = value;
}

float Flood::Matrix4x3::M23::get()
{
    return __M23;
}

void Flood::Matrix4x3::M23::set(float value)
{
    __M23 = value;
}

float Flood::Matrix4x3::M31::get()
{
    return __M31;
}

void Flood::Matrix4x3::M31::set(float value)
{
    __M31 = value;
}

float Flood::Matrix4x3::M32::get()
{
    return __M32;
}

void Flood::Matrix4x3::M32::set(float value)
{
    __M32 = value;
}

float Flood::Matrix4x3::M33::get()
{
    return __M33;
}

void Flood::Matrix4x3::M33::set(float value)
{
    __M33 = value;
}

float Flood::Matrix4x3::Tx::get()
{
    return __Tx;
}

void Flood::Matrix4x3::Tx::set(float value)
{
    __Tx = value;
}

float Flood::Matrix4x3::Ty::get()
{
    return __Ty;
}

void Flood::Matrix4x3::Ty::set(float value)
{
    __Ty = value;
}

float Flood::Matrix4x3::Tz::get()
{
    return __Tz;
}

void Flood::Matrix4x3::Tz::set(float value)
{
    __Tz = value;
}

Flood::Matrix4x3 Flood::Matrix4x3::Identity::get()
{
    return Flood::Matrix4x3((::Matrix4x3*)&::Matrix4x3::Identity);
}

