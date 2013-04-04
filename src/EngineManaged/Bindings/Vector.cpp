/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Vector.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Vector3::Vector3(::Vector3* native)
{
    X = native->x;
    Y = native->y;
    Z = native->z;
}

Flood::Vector3::Vector3(System::IntPtr native)
{
    auto __native = (::Vector3*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Z = __native->z;
}

Flood::Vector3::Vector3(float v)
{
    auto _native = ::Vector3(v);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
}

Flood::Vector3::Vector3(float a, float b, float c)
{
    auto _native = ::Vector3(a, b, c);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
}

bool Flood::Vector3::operator==(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.operator==(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

bool Flood::Vector3::operator!=(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.operator!=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

bool Flood::Vector3::operator<(Flood::Vector3 rhs)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = rhs.X;
    _marshal0.y = rhs.Y;
    _marshal0.z = rhs.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.operator<(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

bool Flood::Vector3::operator>(Flood::Vector3 rhs)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = rhs.X;
    _marshal0.y = rhs.Y;
    _marshal0.z = rhs.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.operator>(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

Flood::Vector3 Flood::Vector3::operator+(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.operator+(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator+=(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator+=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator-()
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto ret = _this0.operator-();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator-(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.operator-(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator-=(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator-=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator*(float s)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto ret = _this0.operator*(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator*(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.operator*(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator*=(float s)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto &ret = _this0.operator*=(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator/(float s)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto ret = _this0.operator/(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::operator/=(float s)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto &ret = _this0.operator/=(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

void Flood::Vector3::SetZero()
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.zero();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
}

float Flood::Vector3::Length()
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto ret = _this0.length();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

float Flood::Vector3::LengthSquared()
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto ret = _this0.lengthSquared();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

float Flood::Vector3::Dot(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.dot(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

Flood::Vector3 Flood::Vector3::Cross(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.cross(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

float Flood::Vector3::Distance(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.distance(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return ret;
}

Flood::Vector3 Flood::Vector3::Project(Flood::Vector3 v)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.project(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::Lerp(Flood::Vector3 vec, float blend)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = vec.X;
    _marshal0.y = vec.Y;
    _marshal0.z = vec.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.lerp(arg0, blend);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::Serp(Flood::Vector3 vec, float t)
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = vec.X;
    _marshal0.y = vec.Y;
    _marshal0.z = vec.Z;
    auto arg0 = _marshal0;
    auto ret = _this0.serp(arg0, t);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::Normalize()
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto ret = _this0.normalize();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::Vector3 Flood::Vector3::Zero::get()
{
    return Flood::Vector3((::Vector3*)&::Vector3::Zero);
}

Flood::Vector3 Flood::Vector3::One::get()
{
    return Flood::Vector3((::Vector3*)&::Vector3::One);
}

Flood::Vector3 Flood::Vector3::UnitX::get()
{
    return Flood::Vector3((::Vector3*)&::Vector3::UnitX);
}

Flood::Vector3 Flood::Vector3::UnitY::get()
{
    return Flood::Vector3((::Vector3*)&::Vector3::UnitY);
}

Flood::Vector3 Flood::Vector3::UnitZ::get()
{
    return Flood::Vector3((::Vector3*)&::Vector3::UnitZ);
}

Flood::Vector4::Vector4(::Vector4* native)
{
    X = native->x;
    Y = native->y;
    Z = native->z;
    W = native->w;
}

Flood::Vector4::Vector4(System::IntPtr native)
{
    auto __native = (::Vector4*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Z = __native->z;
    W = __native->w;
}

Flood::Vector4::Vector4(float v)
{
    auto _native = ::Vector4(v);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Vector4::Vector4(float a, float b, float c, float d)
{
    auto _native = ::Vector4(a, b, c, d);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Vector4::Vector4(Flood::Vector3 v, float d)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    auto _native = ::Vector4(_marshal0, d);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

bool Flood::Vector4::operator==(Flood::Vector4 v)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator==(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

bool Flood::Vector4::operator!=(Flood::Vector4 v)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator!=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

bool Flood::Vector4::operator<(Flood::Vector4 rhs)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = rhs.X;
    _marshal0.y = rhs.Y;
    _marshal0.z = rhs.Z;
    _marshal0.w = rhs.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator<(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

bool Flood::Vector4::operator>(Flood::Vector4 rhs)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = rhs.X;
    _marshal0.y = rhs.Y;
    _marshal0.z = rhs.Z;
    _marshal0.w = rhs.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator>(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

Flood::Vector4 Flood::Vector4::operator+(Flood::Vector4 v)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator+(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator+=(Flood::Vector4 v)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator+=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator-()
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.operator-();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator-(Flood::Vector4 v)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator-(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator-=(Flood::Vector4 v)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator-=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator*(float s)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.operator*(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator*(Flood::Vector4 v)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector4();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    _marshal0.z = v.Z;
    _marshal0.w = v.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator*(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator*=(float s)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto &ret = _this0.operator*=(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator/(float s)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.operator/(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

Flood::Vector4 Flood::Vector4::operator/=(float s)
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto &ret = _this0.operator/=(s);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector4((::Vector4*)&ret);
}

void Flood::Vector4::SetZero()
{
    auto _this0 = ::Vector4();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.zero();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
}

Flood::Vector4 Flood::Vector4::Zero::get()
{
    return Flood::Vector4((::Vector4*)&::Vector4::Zero);
}

Flood::Vector4 Flood::Vector4::UnitX::get()
{
    return Flood::Vector4((::Vector4*)&::Vector4::UnitX);
}

Flood::Vector4 Flood::Vector4::UnitY::get()
{
    return Flood::Vector4((::Vector4*)&::Vector4::UnitY);
}

Flood::Vector4 Flood::Vector4::UnitZ::get()
{
    return Flood::Vector4((::Vector4*)&::Vector4::UnitZ);
}

Flood::Vector4 Flood::Vector4::UnitW::get()
{
    return Flood::Vector4((::Vector4*)&::Vector4::UnitW);
}

Flood::Vector2::Vector2(::Vector2* native)
{
    X = native->x;
    Y = native->y;
}

Flood::Vector2::Vector2(System::IntPtr native)
{
    auto __native = (::Vector2*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
}

Flood::Vector2::Vector2(float x, float y)
{
    auto _native = ::Vector2(x, y);
    this->X = _native.x;
    this->Y = _native.y;
}

bool Flood::Vector2::operator==(Flood::Vector2 v)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator==(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

bool Flood::Vector2::operator!=(Flood::Vector2 v)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator!=(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

bool Flood::Vector2::operator<(Flood::Vector2 rhs)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = rhs.X;
    _marshal0.y = rhs.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator<(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

bool Flood::Vector2::operator>(Flood::Vector2 rhs)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = rhs.X;
    _marshal0.y = rhs.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator>(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

Flood::Vector2 Flood::Vector2::operator+(Flood::Vector2 v)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator+(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2((::Vector2*)&ret);
}

Flood::Vector2 Flood::Vector2::operator+=(Flood::Vector2 v)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator+=(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2((::Vector2*)&ret);
}

Flood::Vector2 Flood::Vector2::operator-()
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto ret = _this0.operator-();
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2((::Vector2*)&ret);
}

Flood::Vector2 Flood::Vector2::operator-(Flood::Vector2 v)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator-(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2((::Vector2*)&ret);
}

Flood::Vector2 Flood::Vector2::operator-=(Flood::Vector2 v)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto _marshal0 = ::Vector2();
    _marshal0.x = v.X;
    _marshal0.y = v.Y;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator-=(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2((::Vector2*)&ret);
}

Flood::Vector2 Flood::Vector2::operator*(float s)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto ret = _this0.operator*(s);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2((::Vector2*)&ret);
}

Flood::Vector2 Flood::Vector2::operator/(int s)
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    auto ret = _this0.operator/(s);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2((::Vector2*)&ret);
}

void Flood::Vector2::SetZero()
{
    auto _this0 = ::Vector2();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.zero();
    X = _this0.x;
    Y = _this0.y;
}

Flood::Vector2 Flood::Vector2::Zero::get()
{
    return Flood::Vector2((::Vector2*)&::Vector2::Zero);
}

Flood::Vector2 Flood::Vector2::UnitX::get()
{
    return Flood::Vector2((::Vector2*)&::Vector2::UnitX);
}

Flood::Vector2 Flood::Vector2::UnitY::get()
{
    return Flood::Vector2((::Vector2*)&::Vector2::UnitY);
}

Flood::Vector2i::Vector2i(::Vector2i* native)
{
    X = native->x;
    Y = native->y;
}

Flood::Vector2i::Vector2i(System::IntPtr native)
{
    auto __native = (::Vector2i*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
}

Flood::Vector2i::Vector2i(int x, int y)
{
    auto _native = ::Vector2i((int32)x, (int32)y);
    this->X = _native.x;
    this->Y = _native.y;
}

bool Flood::Vector2i::operator==(Flood::Vector2i v)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator==(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

bool Flood::Vector2i::operator!=(Flood::Vector2i v)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator!=(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

bool Flood::Vector2i::operator<(Flood::Vector2i rhs)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)rhs.X;
    _marshal0.y = (int32)rhs.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator<(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

bool Flood::Vector2i::operator>(Flood::Vector2i rhs)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)rhs.X;
    _marshal0.y = (int32)rhs.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator>(arg0);
    X = _this0.x;
    Y = _this0.y;
    return ret;
}

Flood::Vector2i Flood::Vector2i::operator+(Flood::Vector2i v)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator+(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::Vector2i Flood::Vector2i::operator+=(Flood::Vector2i v)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator+=(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::Vector2i Flood::Vector2i::operator-()
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto ret = _this0.operator-();
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::Vector2i Flood::Vector2i::operator-(Flood::Vector2i v)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    auto ret = _this0.operator-(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::Vector2i Flood::Vector2i::operator-=(Flood::Vector2i v)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (int32)v.X;
    _marshal0.y = (int32)v.Y;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator-=(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::Vector2i Flood::Vector2i::operator*(int s)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto arg0 = (int32)s;
    auto ret = _this0.operator*(arg0);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2i((::Vector2i*)&ret);
}

Flood::Vector2i Flood::Vector2i::operator/(int s)
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    auto ret = _this0.operator/(s);
    X = _this0.x;
    Y = _this0.y;
    return Flood::Vector2i((::Vector2i*)&ret);
}

void Flood::Vector2i::Zero()
{
    auto _this0 = ::Vector2i();
    _this0.x = (int32)(*this).X;
    _this0.y = (int32)(*this).Y;
    _this0.zero();
    X = _this0.x;
    Y = _this0.y;
}

