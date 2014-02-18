/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Quaternion.h"
#include "EulerAngles.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Quaternion::Quaternion(::Quaternion* native)
{
    __X = native->x;
    __Y = native->y;
    __Z = native->z;
    __W = native->w;
}

Flood::Quaternion::Quaternion(System::IntPtr native)
{
    auto __native = (::Quaternion*)native.ToPointer();
    __X = __native->x;
    __Y = __native->y;
    __Z = __native->z;
    __W = __native->w;
}

Flood::Quaternion::Quaternion(float u)
{
    ::Quaternion _native(u);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Quaternion::Quaternion(float a, float b, float c, float d)
{
    ::Quaternion _native(a, b, c, d);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Quaternion::Quaternion(Flood::EulerAngles angles)
{
    auto _marshal0 = ::EulerAngles();
    _marshal0.x = angles.X;
    _marshal0.y = angles.Y;
    _marshal0.z = angles.Z;
    ::Quaternion _native(_marshal0);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

bool Flood::Quaternion::operator==(Flood::Quaternion __op, Flood::Quaternion q)
{
    auto _marshal0 = ::Quaternion();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Quaternion();
    _marshal1.x = q.X;
    _marshal1.y = q.Y;
    _marshal1.z = q.Z;
    _marshal1.w = q.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 == arg1;
    return __ret;
}

bool Flood::Quaternion::operator!=(Flood::Quaternion __op, Flood::Quaternion q)
{
    auto _marshal0 = ::Quaternion();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Quaternion();
    _marshal1.x = q.X;
    _marshal1.y = q.Y;
    _marshal1.z = q.Z;
    _marshal1.w = q.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 != arg1;
    return __ret;
}

void Flood::Quaternion::SetIdentity()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.identity();
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
}

Flood::Quaternion Flood::Quaternion::Conjugate(Flood::Quaternion q)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto __ret = _this0.conjugate(arg0);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return Flood::Quaternion((::Quaternion*)&__ret);
}

Flood::Quaternion Flood::Quaternion::operator*(Flood::Quaternion __op, Flood::Quaternion a)
{
    auto _marshal0 = ::Quaternion();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Quaternion();
    _marshal1.x = a.X;
    _marshal1.y = a.Y;
    _marshal1.z = a.Z;
    _marshal1.w = a.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 * arg1;
    return Flood::Quaternion((::Quaternion*)&__ret);
}

float Flood::Quaternion::Magnitude()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto __ret = _this0.magnitude();
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return __ret;
}

void Flood::Quaternion::Normalize()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.normalize();
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
}

void Flood::Quaternion::SetToRotateAboutAxis(Flood::Vector3 axis, float theta)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector3();
    _marshal0.x = axis.X;
    _marshal0.y = axis.Y;
    _marshal0.z = axis.Z;
    auto arg0 = _marshal0;
    _this0.setToRotateAboutAxis(arg0, theta);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
}

float Flood::Quaternion::SafeAcos(float x)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto __ret = _this0.safeAcos(x);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return __ret;
}

float Flood::Quaternion::Dot(Flood::Quaternion a)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.z = a.Z;
    _marshal0.w = a.W;
    auto arg0 = _marshal0;
    auto __ret = _this0.dot(arg0);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return __ret;
}

Flood::Quaternion Flood::Quaternion::Slerp(Flood::Quaternion q, float t)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto __ret = _this0.slerp(arg0, t);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return Flood::Quaternion((::Quaternion*)&__ret);
}

void Flood::Quaternion::ToRotateAboutX::set(float value)
{
    auto theta = value;
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.setToRotateAboutX(theta);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
}

void Flood::Quaternion::ToRotateAboutY::set(float value)
{
    auto theta = value;
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.setToRotateAboutY(theta);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
}

void Flood::Quaternion::ToRotateAboutZ::set(float value)
{
    auto theta = value;
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.setToRotateAboutZ(theta);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
}

float Flood::Quaternion::RotationAngle::get()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto __ret = _this0.getRotationAngle();
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return __ret;
}

Flood::Vector3 Flood::Quaternion::RotationAxis::get()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto __ret = _this0.getRotationAxis();
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::EulerAngles Flood::Quaternion::EulerAngles::get()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto __ret = _this0.getEulerAngles();
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
    __W = _this0.w;
    return Flood::EulerAngles((::EulerAngles*)&__ret);
}

float Flood::Quaternion::X::get()
{
    return __X;
}

void Flood::Quaternion::X::set(float value)
{
    __X = value;
}

float Flood::Quaternion::Y::get()
{
    return __Y;
}

void Flood::Quaternion::Y::set(float value)
{
    __Y = value;
}

float Flood::Quaternion::Z::get()
{
    return __Z;
}

void Flood::Quaternion::Z::set(float value)
{
    __Z = value;
}

float Flood::Quaternion::W::get()
{
    return __W;
}

void Flood::Quaternion::W::set(float value)
{
    __W = value;
}

Flood::Quaternion Flood::Quaternion::Identity::get()
{
    return Flood::Quaternion((::Quaternion*)&::Quaternion::Identity);
}

