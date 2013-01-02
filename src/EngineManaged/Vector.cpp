/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Vector3::Vector3(float v)
{
}

Flood::Vector3::Vector3(float a, float b, float c)
{
}

bool Flood::Vector3::operator==(Flood::Vector3 v)
{
    return false;
}

bool Flood::Vector3::operator!=(Flood::Vector3 v)
{
    return false;
}

bool Flood::Vector3::operator<(Flood::Vector3 rhs)
{
    return false;
}

bool Flood::Vector3::operator>(Flood::Vector3 rhs)
{
    return false;
}

Flood::Vector3 Flood::Vector3::operator+(Flood::Vector3 v)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator+=(Flood::Vector3 v)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator-()
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator-(Flood::Vector3 v)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator-=(Flood::Vector3 v)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator*(float s)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator*(Flood::Vector3 v)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator*=(float s)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator/(float s)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::operator/=(float s)
{
    return Vector3();
}

void Flood::Vector3::Zero()
{
}

float Flood::Vector3::Length()
{
    return 0.0f;
}

float Flood::Vector3::LengthSquared()
{
    return 0.0f;
}

float Flood::Vector3::Dot(Flood::Vector3 v)
{
    return 0.0f;
}

Flood::Vector3 Flood::Vector3::Cross(Flood::Vector3 v)
{
    return Vector3();
}

float Flood::Vector3::Distance(Flood::Vector3 v)
{
    return 0.0f;
}

Flood::Vector3 Flood::Vector3::Project(Flood::Vector3 v)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::Lerp(Flood::Vector3 vec, float blend)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::Serp(Flood::Vector3 vec, float t)
{
    return Vector3();
}

Flood::Vector3 Flood::Vector3::Normalize()
{
    return Vector3();
}

Flood::Vector4::Vector4(float v)
{
}

Flood::Vector4::Vector4(float a, float b, float c, float d)
{
}

Flood::Vector4::Vector4(Flood::Vector3 v, float d)
{
}

bool Flood::Vector4::operator==(Flood::Vector4 v)
{
    return false;
}

bool Flood::Vector4::operator!=(Flood::Vector4 v)
{
    return false;
}

bool Flood::Vector4::operator<(Flood::Vector4 rhs)
{
    return false;
}

bool Flood::Vector4::operator>(Flood::Vector4 rhs)
{
    return false;
}

Flood::Vector4 Flood::Vector4::operator+(Flood::Vector4 v)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator+=(Flood::Vector4 v)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator-()
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator-(Flood::Vector4 v)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator-=(Flood::Vector4 v)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator*(float s)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator*(Flood::Vector4 v)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator*=(float s)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator/(float s)
{
    return Vector4();
}

Flood::Vector4 Flood::Vector4::operator/=(float s)
{
    return Vector4();
}

void Flood::Vector4::Zero()
{
}

Flood::Vector2::Vector2(float x, float y)
{
}

bool Flood::Vector2::operator==(Flood::Vector2 v)
{
    return false;
}

bool Flood::Vector2::operator!=(Flood::Vector2 v)
{
    return false;
}

bool Flood::Vector2::operator<(Flood::Vector2 rhs)
{
    return false;
}

bool Flood::Vector2::operator>(Flood::Vector2 rhs)
{
    return false;
}

Flood::Vector2 Flood::Vector2::operator+(Flood::Vector2 v)
{
    return Vector2();
}

Flood::Vector2 Flood::Vector2::operator+=(Flood::Vector2 v)
{
    return Vector2();
}

Flood::Vector2 Flood::Vector2::operator-()
{
    return Vector2();
}

Flood::Vector2 Flood::Vector2::operator-(Flood::Vector2 v)
{
    return Vector2();
}

Flood::Vector2 Flood::Vector2::operator-=(Flood::Vector2 v)
{
    return Vector2();
}

Flood::Vector2 Flood::Vector2::operator*(float s)
{
    return Vector2();
}

Flood::Vector2 Flood::Vector2::operator/(int s)
{
    return Vector2();
}

void Flood::Vector2::Zero()
{
}

Flood::Vector2i::Vector2i(int x, int y)
{
}

bool Flood::Vector2i::operator==(Flood::Vector2i v)
{
    return false;
}

bool Flood::Vector2i::operator!=(Flood::Vector2i v)
{
    return false;
}

bool Flood::Vector2i::operator<(Flood::Vector2i rhs)
{
    return false;
}

bool Flood::Vector2i::operator>(Flood::Vector2i rhs)
{
    return false;
}

Flood::Vector2i Flood::Vector2i::operator+(Flood::Vector2i v)
{
    return Vector2i();
}

Flood::Vector2i Flood::Vector2i::operator+=(Flood::Vector2i v)
{
    return Vector2i();
}

Flood::Vector2i Flood::Vector2i::operator-()
{
    return Vector2i();
}

Flood::Vector2i Flood::Vector2i::operator-(Flood::Vector2i v)
{
    return Vector2i();
}

Flood::Vector2i Flood::Vector2i::operator-=(Flood::Vector2i v)
{
    return Vector2i();
}

Flood::Vector2i Flood::Vector2i::operator*(int s)
{
    return Vector2i();
}

Flood::Vector2i Flood::Vector2i::operator/(int s)
{
    return Vector2i();
}

void Flood::Vector2i::Zero()
{
}


