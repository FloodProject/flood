/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Matrix4x4.h"
#include "Matrix4x3.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Matrix4x4::Matrix4x4(::Matrix4x4* native)
{
    __M11 = native->m11;
    __M12 = native->m12;
    __M13 = native->m13;
    __M14 = native->m14;
    __M21 = native->m21;
    __M22 = native->m22;
    __M23 = native->m23;
    __M24 = native->m24;
    __M31 = native->m31;
    __M32 = native->m32;
    __M33 = native->m33;
    __M34 = native->m34;
    __Tx = native->tx;
    __Ty = native->ty;
    __Tz = native->tz;
    __Tw = native->tw;
}

Flood::Matrix4x4::Matrix4x4(System::IntPtr native)
{
    auto __native = (::Matrix4x4*)native.ToPointer();
    __M11 = __native->m11;
    __M12 = __native->m12;
    __M13 = __native->m13;
    __M14 = __native->m14;
    __M21 = __native->m21;
    __M22 = __native->m22;
    __M23 = __native->m23;
    __M24 = __native->m24;
    __M31 = __native->m31;
    __M32 = __native->m32;
    __M33 = __native->m33;
    __M34 = __native->m34;
    __Tx = __native->tx;
    __Ty = __native->ty;
    __Tz = __native->tz;
    __Tw = __native->tw;
}

Flood::Matrix4x4::Matrix4x4(Flood::Matrix4x3 _0)
{
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = _0.M11;
    _marshal0.m12 = _0.M12;
    _marshal0.m13 = _0.M13;
    _marshal0.m21 = _0.M21;
    _marshal0.m22 = _0.M22;
    _marshal0.m23 = _0.M23;
    _marshal0.m31 = _0.M31;
    _marshal0.m32 = _0.M32;
    _marshal0.m33 = _0.M33;
    _marshal0.tx = _0.Tx;
    _marshal0.ty = _0.Ty;
    _marshal0.tz = _0.Tz;
    ::Matrix4x4 _native(_marshal0);
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
    auto _this0 = ::Matrix4x4();
    _this0.m11 = (*this).M11;
    _this0.m12 = (*this).M12;
    _this0.m13 = (*this).M13;
    _this0.m14 = (*this).M14;
    _this0.m21 = (*this).M21;
    _this0.m22 = (*this).M22;
    _this0.m23 = (*this).M23;
    _this0.m24 = (*this).M24;
    _this0.m31 = (*this).M31;
    _this0.m32 = (*this).M32;
    _this0.m33 = (*this).M33;
    _this0.m34 = (*this).M34;
    _this0.tx = (*this).Tx;
    _this0.ty = (*this).Ty;
    _this0.tz = (*this).Tz;
    _this0.tw = (*this).Tw;
    _this0.identity();
    __M11 = _this0.m11;
    __M12 = _this0.m12;
    __M13 = _this0.m13;
    __M14 = _this0.m14;
    __M21 = _this0.m21;
    __M22 = _this0.m22;
    __M23 = _this0.m23;
    __M24 = _this0.m24;
    __M31 = _this0.m31;
    __M32 = _this0.m32;
    __M33 = _this0.m33;
    __M34 = _this0.m34;
    __Tx = _this0.tx;
    __Ty = _this0.ty;
    __Tz = _this0.tz;
    __Tw = _this0.tw;
}

Flood::Matrix4x4 Flood::Matrix4x4::Inverse()
{
    auto _this0 = ::Matrix4x4();
    _this0.m11 = (*this).M11;
    _this0.m12 = (*this).M12;
    _this0.m13 = (*this).M13;
    _this0.m14 = (*this).M14;
    _this0.m21 = (*this).M21;
    _this0.m22 = (*this).M22;
    _this0.m23 = (*this).M23;
    _this0.m24 = (*this).M24;
    _this0.m31 = (*this).M31;
    _this0.m32 = (*this).M32;
    _this0.m33 = (*this).M33;
    _this0.m34 = (*this).M34;
    _this0.tx = (*this).Tx;
    _this0.ty = (*this).Ty;
    _this0.tz = (*this).Tz;
    _this0.tw = (*this).Tw;
    auto __ret = _this0.inverse();
    __M11 = _this0.m11;
    __M12 = _this0.m12;
    __M13 = _this0.m13;
    __M14 = _this0.m14;
    __M21 = _this0.m21;
    __M22 = _this0.m22;
    __M23 = _this0.m23;
    __M24 = _this0.m24;
    __M31 = _this0.m31;
    __M32 = _this0.m32;
    __M33 = _this0.m33;
    __M34 = _this0.m34;
    __Tx = _this0.tx;
    __Ty = _this0.ty;
    __Tz = _this0.tz;
    __Tw = _this0.tw;
    return Flood::Matrix4x4((::Matrix4x4*)&__ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::Transpose()
{
    auto _this0 = ::Matrix4x4();
    _this0.m11 = (*this).M11;
    _this0.m12 = (*this).M12;
    _this0.m13 = (*this).M13;
    _this0.m14 = (*this).M14;
    _this0.m21 = (*this).M21;
    _this0.m22 = (*this).M22;
    _this0.m23 = (*this).M23;
    _this0.m24 = (*this).M24;
    _this0.m31 = (*this).M31;
    _this0.m32 = (*this).M32;
    _this0.m33 = (*this).M33;
    _this0.m34 = (*this).M34;
    _this0.tx = (*this).Tx;
    _this0.ty = (*this).Ty;
    _this0.tz = (*this).Tz;
    _this0.tw = (*this).Tw;
    auto __ret = _this0.transpose();
    __M11 = _this0.m11;
    __M12 = _this0.m12;
    __M13 = _this0.m13;
    __M14 = _this0.m14;
    __M21 = _this0.m21;
    __M22 = _this0.m22;
    __M23 = _this0.m23;
    __M24 = _this0.m24;
    __M31 = _this0.m31;
    __M32 = _this0.m32;
    __M33 = _this0.m33;
    __M34 = _this0.m34;
    __Tx = _this0.tx;
    __Ty = _this0.ty;
    __Tz = _this0.tz;
    __Tw = _this0.tw;
    return Flood::Matrix4x4((::Matrix4x4*)&__ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::operator*(Flood::Matrix4x4 __op, Flood::Matrix4x4 m)
{
    auto _marshal0 = ::Matrix4x4();
    _marshal0.m11 = __op.M11;
    _marshal0.m12 = __op.M12;
    _marshal0.m13 = __op.M13;
    _marshal0.m14 = __op.M14;
    _marshal0.m21 = __op.M21;
    _marshal0.m22 = __op.M22;
    _marshal0.m23 = __op.M23;
    _marshal0.m24 = __op.M24;
    _marshal0.m31 = __op.M31;
    _marshal0.m32 = __op.M32;
    _marshal0.m33 = __op.M33;
    _marshal0.m34 = __op.M34;
    _marshal0.tx = __op.Tx;
    _marshal0.ty = __op.Ty;
    _marshal0.tz = __op.Tz;
    _marshal0.tw = __op.Tw;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Matrix4x4();
    _marshal1.m11 = m.M11;
    _marshal1.m12 = m.M12;
    _marshal1.m13 = m.M13;
    _marshal1.m14 = m.M14;
    _marshal1.m21 = m.M21;
    _marshal1.m22 = m.M22;
    _marshal1.m23 = m.M23;
    _marshal1.m24 = m.M24;
    _marshal1.m31 = m.M31;
    _marshal1.m32 = m.M32;
    _marshal1.m33 = m.M33;
    _marshal1.m34 = m.M34;
    _marshal1.tx = m.Tx;
    _marshal1.ty = m.Ty;
    _marshal1.tz = m.Tz;
    _marshal1.tw = m.Tw;
    auto arg1 = _marshal1;
    auto __ret = arg0 * arg1;
    return Flood::Matrix4x4((::Matrix4x4*)&__ret);
}

Flood::Vector3 Flood::Matrix4x4::operator*(Flood::Matrix4x4 __op, Flood::Vector3 v)
{
    auto _marshal0 = ::Matrix4x4();
    _marshal0.m11 = __op.M11;
    _marshal0.m12 = __op.M12;
    _marshal0.m13 = __op.M13;
    _marshal0.m14 = __op.M14;
    _marshal0.m21 = __op.M21;
    _marshal0.m22 = __op.M22;
    _marshal0.m23 = __op.M23;
    _marshal0.m24 = __op.M24;
    _marshal0.m31 = __op.M31;
    _marshal0.m32 = __op.M32;
    _marshal0.m33 = __op.M33;
    _marshal0.m34 = __op.M34;
    _marshal0.tx = __op.Tx;
    _marshal0.ty = __op.Ty;
    _marshal0.tz = __op.Tz;
    _marshal0.tw = __op.Tw;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 * arg1;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Vector4 Flood::Matrix4x4::operator*(Flood::Matrix4x4 __op, Flood::Vector4 v)
{
    auto _marshal0 = ::Matrix4x4();
    _marshal0.m11 = __op.M11;
    _marshal0.m12 = __op.M12;
    _marshal0.m13 = __op.M13;
    _marshal0.m14 = __op.M14;
    _marshal0.m21 = __op.M21;
    _marshal0.m22 = __op.M22;
    _marshal0.m23 = __op.M23;
    _marshal0.m24 = __op.M24;
    _marshal0.m31 = __op.M31;
    _marshal0.m32 = __op.M32;
    _marshal0.m33 = __op.M33;
    _marshal0.m34 = __op.M34;
    _marshal0.tx = __op.Tx;
    _marshal0.ty = __op.Ty;
    _marshal0.tz = __op.Tz;
    _marshal0.tw = __op.Tw;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector4();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    _marshal1.w = v.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 * arg1;
    return Flood::Vector4((::Vector4*)&__ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::CreatePerspective(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
    auto __ret = ::Matrix4x4::createPerspective(FOV, aspectRatio, nearPlane, farPlane);
    return Flood::Matrix4x4((::Matrix4x4*)&__ret);
}

Flood::Matrix4x4 Flood::Matrix4x4::CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    auto __ret = ::Matrix4x4::createOrthographic(left, right, bottom, top, nearPlane, farPlane);
    return Flood::Matrix4x4((::Matrix4x4*)&__ret);
}

float Flood::Matrix4x4::M11::get()
{
    return __M11;
}

void Flood::Matrix4x4::M11::set(float value)
{
    __M11 = value;
}

float Flood::Matrix4x4::M12::get()
{
    return __M12;
}

void Flood::Matrix4x4::M12::set(float value)
{
    __M12 = value;
}

float Flood::Matrix4x4::M13::get()
{
    return __M13;
}

void Flood::Matrix4x4::M13::set(float value)
{
    __M13 = value;
}

float Flood::Matrix4x4::M14::get()
{
    return __M14;
}

void Flood::Matrix4x4::M14::set(float value)
{
    __M14 = value;
}

float Flood::Matrix4x4::M21::get()
{
    return __M21;
}

void Flood::Matrix4x4::M21::set(float value)
{
    __M21 = value;
}

float Flood::Matrix4x4::M22::get()
{
    return __M22;
}

void Flood::Matrix4x4::M22::set(float value)
{
    __M22 = value;
}

float Flood::Matrix4x4::M23::get()
{
    return __M23;
}

void Flood::Matrix4x4::M23::set(float value)
{
    __M23 = value;
}

float Flood::Matrix4x4::M24::get()
{
    return __M24;
}

void Flood::Matrix4x4::M24::set(float value)
{
    __M24 = value;
}

float Flood::Matrix4x4::M31::get()
{
    return __M31;
}

void Flood::Matrix4x4::M31::set(float value)
{
    __M31 = value;
}

float Flood::Matrix4x4::M32::get()
{
    return __M32;
}

void Flood::Matrix4x4::M32::set(float value)
{
    __M32 = value;
}

float Flood::Matrix4x4::M33::get()
{
    return __M33;
}

void Flood::Matrix4x4::M33::set(float value)
{
    __M33 = value;
}

float Flood::Matrix4x4::M34::get()
{
    return __M34;
}

void Flood::Matrix4x4::M34::set(float value)
{
    __M34 = value;
}

float Flood::Matrix4x4::Tx::get()
{
    return __Tx;
}

void Flood::Matrix4x4::Tx::set(float value)
{
    __Tx = value;
}

float Flood::Matrix4x4::Ty::get()
{
    return __Ty;
}

void Flood::Matrix4x4::Ty::set(float value)
{
    __Ty = value;
}

float Flood::Matrix4x4::Tz::get()
{
    return __Tz;
}

void Flood::Matrix4x4::Tz::set(float value)
{
    __Tz = value;
}

float Flood::Matrix4x4::Tw::get()
{
    return __Tw;
}

void Flood::Matrix4x4::Tw::set(float value)
{
    __Tw = value;
}

Flood::Matrix4x4 Flood::Matrix4x4::Identity::get()
{
    return Flood::Matrix4x4((::Matrix4x4*)&::Matrix4x4::Identity);
}

