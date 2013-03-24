/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Quaternion.h"
#include "EulerAngles.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Quaternion::Quaternion(::Quaternion* native)
{
    X = native->x;
    Y = native->y;
    Z = native->z;
    W = native->w;
}

Flood::Quaternion::Quaternion(System::IntPtr native)
{
    auto __native = (::Quaternion*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Z = __native->z;
    W = __native->w;
}

Flood::Quaternion::Quaternion(float u)
{
    auto _native = ::Quaternion(u);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Quaternion::Quaternion(float a, float b, float c, float d)
{
    auto _native = ::Quaternion(a, b, c, d);
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
    auto _native = ::Quaternion(_marshal0);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

bool Flood::Quaternion::operator==(Flood::Quaternion q)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = this0->operator==(arg0);
    return ret;
}

bool Flood::Quaternion::operator!=(Flood::Quaternion q)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = this0->operator!=(arg0);
    return ret;
}

void Flood::Quaternion::SetIdentity()
{
    auto this0 = (::Quaternion*) 0;
    this0->identity();
}

Flood::Quaternion Flood::Quaternion::Conjugate(Flood::Quaternion q)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = this0->conjugate(arg0);
    return Flood::Quaternion((::Quaternion*)&ret);
}

Flood::Quaternion Flood::Quaternion::operator*(Flood::Quaternion a)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.z = a.Z;
    _marshal0.w = a.W;
    auto arg0 = _marshal0;
    auto ret = this0->operator*(arg0);
    return Flood::Quaternion((::Quaternion*)&ret);
}

Flood::Quaternion Flood::Quaternion::operator*=(Flood::Quaternion a)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.z = a.Z;
    _marshal0.w = a.W;
    auto arg0 = _marshal0;
    auto ret = this0->operator*=(arg0);
    return Flood::Quaternion((::Quaternion*)&ret);
}

float Flood::Quaternion::Magnitude()
{
    auto this0 = (::Quaternion*) 0;
    auto ret = this0->magnitude();
    return ret;
}

void Flood::Quaternion::Normalize()
{
    auto this0 = (::Quaternion*) 0;
    this0->normalize();
}

void Flood::Quaternion::SetToRotateAboutX(float theta)
{
    auto this0 = (::Quaternion*) 0;
    this0->setToRotateAboutX(theta);
}

void Flood::Quaternion::SetToRotateAboutY(float theta)
{
    auto this0 = (::Quaternion*) 0;
    this0->setToRotateAboutY(theta);
}

void Flood::Quaternion::SetToRotateAboutZ(float theta)
{
    auto this0 = (::Quaternion*) 0;
    this0->setToRotateAboutZ(theta);
}

void Flood::Quaternion::SetToRotateAboutAxis(Flood::Vector3 axis, float theta)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Vector3();
    _marshal0.x = axis.X;
    _marshal0.y = axis.Y;
    _marshal0.z = axis.Z;
    auto arg0 = _marshal0;
    this0->setToRotateAboutAxis(arg0, theta);
}

float Flood::Quaternion::SafeAcos(float x)
{
    auto this0 = (::Quaternion*) 0;
    auto ret = this0->safeAcos(x);
    return ret;
}

float Flood::Quaternion::GetRotationAngle()
{
    auto this0 = (::Quaternion*) 0;
    auto ret = this0->getRotationAngle();
    return ret;
}

Flood::Vector3 Flood::Quaternion::GetRotationAxis()
{
    auto this0 = (::Quaternion*) 0;
    auto ret = this0->getRotationAxis();
    return Flood::Vector3((::Vector3*)&ret);
}

float Flood::Quaternion::Dot(Flood::Quaternion a)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.z = a.Z;
    _marshal0.w = a.W;
    auto arg0 = _marshal0;
    auto ret = this0->dot(arg0);
    return ret;
}

Flood::EulerAngles Flood::Quaternion::GetEulerAngles()
{
    auto this0 = (::Quaternion*) 0;
    auto ret = this0->getEulerAngles();
    return Flood::EulerAngles((::EulerAngles*)&ret);
}

Flood::Quaternion Flood::Quaternion::Slerp(Flood::Quaternion q, float t)
{
    auto this0 = (::Quaternion*) 0;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = this0->slerp(arg0, t);
    return Flood::Quaternion((::Quaternion*)&ret);
}

