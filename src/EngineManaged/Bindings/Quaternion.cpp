/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Quaternion.h"
#include "EulerAngles.h"
#include "Vector.h"
#include "Reflection.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Quaternion::Quaternion(::Quaternion* native)
{
}

Flood::Quaternion::Quaternion(System::IntPtr native)
{
    auto __native = (::Quaternion*)native.ToPointer();
}

Flood::Quaternion::Quaternion(float u)
{
}

Flood::Quaternion::Quaternion(float a, float b, float c, float d)
{
}

Flood::Quaternion::Quaternion(Flood::EulerAngles angles)
{
}

bool Flood::Quaternion::operator==(Flood::Quaternion q)
{
    auto this0 = (::Quaternion*) 0;
    auto arg0 = ::Quaternion();
    auto ret = this0->operator==(arg0);
    return ret;
}

bool Flood::Quaternion::operator!=(Flood::Quaternion q)
{
    auto this0 = (::Quaternion*) 0;
    auto arg0 = ::Quaternion();
    auto ret = this0->operator!=(arg0);
    return ret;
}

void Flood::Quaternion::Identity()
{
    auto this0 = (::Quaternion*) 0;
    this0->identity();
}

Flood::Quaternion Flood::Quaternion::Conjugate(Flood::Quaternion q)
{
    auto this0 = (::Quaternion*) 0;
    auto arg0 = ::Quaternion();
    auto ret = this0->conjugate(arg0);
    return Flood::Quaternion((::Quaternion*)&ret);
}

Flood::Quaternion Flood::Quaternion::operator*(Flood::Quaternion a)
{
    auto this0 = (::Quaternion*) 0;
    auto arg0 = ::Quaternion();
    auto ret = this0->operator*(arg0);
    return Flood::Quaternion((::Quaternion*)&ret);
}

Flood::Quaternion Flood::Quaternion::operator*=(Flood::Quaternion a)
{
    auto this0 = (::Quaternion*) 0;
    auto arg0 = ::Quaternion();
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
    auto arg0 = ::Vector3();
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
    auto arg0 = ::Quaternion();
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
    auto arg0 = ::Quaternion();
    auto ret = this0->slerp(arg0, t);
    return Flood::Quaternion((::Quaternion*)&ret);
}

