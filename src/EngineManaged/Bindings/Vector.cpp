/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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
    ::Vector3 _native(v);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
}

Flood::Vector3::Vector3(float a, float b, float c)
{
    ::Vector3 _native(a, b, c);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
}

bool Flood::Vector3::operator==(Flood::Vector3 __op, Flood::Vector3 v)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 == arg1;
    return __ret;
}

bool Flood::Vector3::operator!=(Flood::Vector3 __op, Flood::Vector3 v)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 != arg1;
    return __ret;
}

bool Flood::Vector3::operator<(Flood::Vector3 __op, Flood::Vector3 rhs)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = rhs.X;
    _marshal1.y = rhs.Y;
    _marshal1.z = rhs.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 < arg1;
    return __ret;
}

bool Flood::Vector3::operator>(Flood::Vector3 __op, Flood::Vector3 rhs)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = rhs.X;
    _marshal1.y = rhs.Y;
    _marshal1.z = rhs.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 > arg1;
    return __ret;
}

Flood::Vector3 Flood::Vector3::operator+(Flood::Vector3 __op, Flood::Vector3 v)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 + arg1;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Vector3 Flood::Vector3::operator-(Flood::Vector3 __op)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto __ret = - arg0;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Vector3 Flood::Vector3::operator-(Flood::Vector3 __op, Flood::Vector3 v)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 - arg1;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Vector3 Flood::Vector3::operator*(Flood::Vector3 __op, float s)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto __ret = arg0 * s;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Vector3 Flood::Vector3::operator*(Flood::Vector3 __op, Flood::Vector3 v)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    auto arg1 = _marshal1;
    auto __ret = arg0 * arg1;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Vector3 Flood::Vector3::operator/(Flood::Vector3 __op, float s)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    auto arg0 = _marshal0;
    auto __ret = arg0 / s;
    return Flood::Vector3((::Vector3*)&__ret);
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
    auto __ret = _this0.length();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return __ret;
}

float Flood::Vector3::LengthSquared()
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto __ret = _this0.lengthSquared();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return __ret;
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
    auto __ret = _this0.dot(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return __ret;
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
    auto __ret = _this0.cross(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&__ret);
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
    auto __ret = _this0.distance(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return __ret;
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
    auto __ret = _this0.project(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&__ret);
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
    auto __ret = _this0.lerp(arg0, blend);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&__ret);
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
    auto __ret = _this0.serp(arg0, t);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::Vector3 Flood::Vector3::Normalize()
{
    auto _this0 = ::Vector3();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto __ret = _this0.normalize();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::Vector3((::Vector3*)&__ret);
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
    ::Vector4 _native(v);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Vector4::Vector4(float a, float b, float c, float d)
{
    ::Vector4 _native(a, b, c, d);
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
    ::Vector4 _native(_marshal0, d);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

bool Flood::Vector4::operator==(Flood::Vector4 __op, Flood::Vector4 v)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector4();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    _marshal1.w = v.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 == arg1;
    return __ret;
}

bool Flood::Vector4::operator!=(Flood::Vector4 __op, Flood::Vector4 v)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector4();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    _marshal1.w = v.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 != arg1;
    return __ret;
}

bool Flood::Vector4::operator<(Flood::Vector4 __op, Flood::Vector4 rhs)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector4();
    _marshal1.x = rhs.X;
    _marshal1.y = rhs.Y;
    _marshal1.z = rhs.Z;
    _marshal1.w = rhs.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 < arg1;
    return __ret;
}

bool Flood::Vector4::operator>(Flood::Vector4 __op, Flood::Vector4 rhs)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector4();
    _marshal1.x = rhs.X;
    _marshal1.y = rhs.Y;
    _marshal1.z = rhs.Z;
    _marshal1.w = rhs.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 > arg1;
    return __ret;
}

Flood::Vector4 Flood::Vector4::operator+(Flood::Vector4 __op, Flood::Vector4 v)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector4();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    _marshal1.w = v.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 + arg1;
    return Flood::Vector4((::Vector4*)&__ret);
}

Flood::Vector4 Flood::Vector4::operator-(Flood::Vector4 __op)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto __ret = - arg0;
    return Flood::Vector4((::Vector4*)&__ret);
}

Flood::Vector4 Flood::Vector4::operator-(Flood::Vector4 __op, Flood::Vector4 v)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector4();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    _marshal1.z = v.Z;
    _marshal1.w = v.W;
    auto arg1 = _marshal1;
    auto __ret = arg0 - arg1;
    return Flood::Vector4((::Vector4*)&__ret);
}

Flood::Vector4 Flood::Vector4::operator*(Flood::Vector4 __op, float s)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto __ret = arg0 * s;
    return Flood::Vector4((::Vector4*)&__ret);
}

Flood::Vector4 Flood::Vector4::operator*(Flood::Vector4 __op, Flood::Vector4 v)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
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

Flood::Vector4 Flood::Vector4::operator/(Flood::Vector4 __op, float s)
{
    auto _marshal0 = ::Vector4();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    _marshal0.z = __op.Z;
    _marshal0.w = __op.W;
    auto arg0 = _marshal0;
    auto __ret = arg0 / s;
    return Flood::Vector4((::Vector4*)&__ret);
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
    ::Vector2 _native(x, y);
    this->X = _native.x;
    this->Y = _native.y;
}

bool Flood::Vector2::operator==(Flood::Vector2 __op, Flood::Vector2 v)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 == arg1;
    return __ret;
}

bool Flood::Vector2::operator!=(Flood::Vector2 __op, Flood::Vector2 v)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 != arg1;
    return __ret;
}

bool Flood::Vector2::operator<(Flood::Vector2 __op, Flood::Vector2 rhs)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2();
    _marshal1.x = rhs.X;
    _marshal1.y = rhs.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 < arg1;
    return __ret;
}

bool Flood::Vector2::operator>(Flood::Vector2 __op, Flood::Vector2 rhs)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2();
    _marshal1.x = rhs.X;
    _marshal1.y = rhs.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 > arg1;
    return __ret;
}

Flood::Vector2 Flood::Vector2::operator+(Flood::Vector2 __op, Flood::Vector2 v)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 + arg1;
    return Flood::Vector2((::Vector2*)&__ret);
}

Flood::Vector2 Flood::Vector2::operator-(Flood::Vector2 __op)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto __ret = - arg0;
    return Flood::Vector2((::Vector2*)&__ret);
}

Flood::Vector2 Flood::Vector2::operator-(Flood::Vector2 __op, Flood::Vector2 v)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2();
    _marshal1.x = v.X;
    _marshal1.y = v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 - arg1;
    return Flood::Vector2((::Vector2*)&__ret);
}

Flood::Vector2 Flood::Vector2::operator*(Flood::Vector2 __op, float s)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto __ret = arg0 * s;
    return Flood::Vector2((::Vector2*)&__ret);
}

Flood::Vector2 Flood::Vector2::operator/(Flood::Vector2 __op, int s)
{
    auto _marshal0 = ::Vector2();
    _marshal0.x = __op.X;
    _marshal0.y = __op.Y;
    auto arg0 = _marshal0;
    auto __ret = arg0 / s;
    return Flood::Vector2((::Vector2*)&__ret);
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
    ::Vector2i _native((::int32)(::int32_t)x, (::int32)(::int32_t)y);
    this->X = _native.x;
    this->Y = _native.y;
}

bool Flood::Vector2i::operator==(Flood::Vector2i __op, Flood::Vector2i v)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (::int32)(::int32_t)v.X;
    _marshal1.y = (::int32)(::int32_t)v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 == arg1;
    return __ret;
}

bool Flood::Vector2i::operator!=(Flood::Vector2i __op, Flood::Vector2i v)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (::int32)(::int32_t)v.X;
    _marshal1.y = (::int32)(::int32_t)v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 != arg1;
    return __ret;
}

bool Flood::Vector2i::operator<(Flood::Vector2i __op, Flood::Vector2i rhs)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (::int32)(::int32_t)rhs.X;
    _marshal1.y = (::int32)(::int32_t)rhs.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 < arg1;
    return __ret;
}

bool Flood::Vector2i::operator>(Flood::Vector2i __op, Flood::Vector2i rhs)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (::int32)(::int32_t)rhs.X;
    _marshal1.y = (::int32)(::int32_t)rhs.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 > arg1;
    return __ret;
}

Flood::Vector2i Flood::Vector2i::operator+(Flood::Vector2i __op, Flood::Vector2i v)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (::int32)(::int32_t)v.X;
    _marshal1.y = (::int32)(::int32_t)v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 + arg1;
    return Flood::Vector2i((::Vector2i*)&__ret);
}

Flood::Vector2i Flood::Vector2i::operator-(Flood::Vector2i __op)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto __ret = - arg0;
    return Flood::Vector2i((::Vector2i*)&__ret);
}

Flood::Vector2i Flood::Vector2i::operator-(Flood::Vector2i __op, Flood::Vector2i v)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector2i();
    _marshal1.x = (::int32)(::int32_t)v.X;
    _marshal1.y = (::int32)(::int32_t)v.Y;
    auto arg1 = _marshal1;
    auto __ret = arg0 - arg1;
    return Flood::Vector2i((::Vector2i*)&__ret);
}

Flood::Vector2i Flood::Vector2i::operator*(Flood::Vector2i __op, int s)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto arg1 = (::int32)(::int32_t)s;
    auto __ret = arg0 * arg1;
    return Flood::Vector2i((::Vector2i*)&__ret);
}

Flood::Vector2i Flood::Vector2i::operator/(Flood::Vector2i __op, int s)
{
    auto _marshal0 = ::Vector2i();
    _marshal0.x = (::int32)(::int32_t)__op.X;
    _marshal0.y = (::int32)(::int32_t)__op.Y;
    auto arg0 = _marshal0;
    auto __ret = arg0 / s;
    return Flood::Vector2i((::Vector2i*)&__ret);
}

void Flood::Vector2i::Zero()
{
    auto _this0 = ::Vector2i();
    _this0.x = (::int32)(::int32_t)(*this).X;
    _this0.y = (::int32)(::int32_t)(*this).Y;
    _this0.zero();
    X = _this0.x;
    Y = _this0.y;
}

