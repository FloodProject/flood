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
    return false;
}

bool Flood::Quaternion::operator!=(Flood::Quaternion q)
{
    return false;
}

void Flood::Quaternion::Identity()
{
}

Flood::Quaternion Flood::Quaternion::Conjugate(Flood::Quaternion q)
{
    return Quaternion();
}

Flood::Quaternion Flood::Quaternion::operator*(Flood::Quaternion a)
{
    return Quaternion();
}

Flood::Quaternion Flood::Quaternion::operator*=(Flood::Quaternion a)
{
    return Quaternion();
}

float Flood::Quaternion::Magnitude()
{
    return 0.0f;
}

void Flood::Quaternion::Normalize()
{
}

void Flood::Quaternion::SetToRotateAboutX(float theta)
{
}

void Flood::Quaternion::SetToRotateAboutY(float theta)
{
}

void Flood::Quaternion::SetToRotateAboutZ(float theta)
{
}

void Flood::Quaternion::SetToRotateAboutAxis(Flood::Vector3 axis, float theta)
{
}

float Flood::Quaternion::SafeAcos(float x)
{
    return 0.0f;
}

float Flood::Quaternion::GetRotationAngle()
{
    return 0.0f;
}

Flood::Vector3 Flood::Quaternion::GetRotationAxis()
{
    return Vector3();
}

float Flood::Quaternion::Dot(Flood::Quaternion a)
{
    return 0.0f;
}

Flood::EulerAngles Flood::Quaternion::GetEulerAngles()
{
    return EulerAngles();
}

Flood::Quaternion Flood::Quaternion::Slerp(Flood::Quaternion q, float t)
{
    return Quaternion();
}


