/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "EulerAngles.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::EulerAngles::EulerAngles(::EulerAngles* native)
{
    __X = native->x;
    __Y = native->y;
    __Z = native->z;
}

Flood::EulerAngles::EulerAngles(System::IntPtr native)
{
    auto __native = (::EulerAngles*)native.ToPointer();
    __X = __native->x;
    __Y = __native->y;
    __Z = __native->z;
}

Flood::EulerAngles::EulerAngles(float x, float y, float z)
{
    ::EulerAngles _native(x, y, z);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
}

void Flood::EulerAngles::Rotate(float x, float y, float z)
{
    auto _this0 = ::EulerAngles();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.rotate(x, y, z);
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
}

void Flood::EulerAngles::Identity()
{
    auto _this0 = ::EulerAngles();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.identity();
    __X = _this0.x;
    __Y = _this0.y;
    __Z = _this0.z;
}

float Flood::EulerAngles::X::get()
{
    return __X;
}

void Flood::EulerAngles::X::set(float value)
{
    __X = value;
}

float Flood::EulerAngles::Y::get()
{
    return __Y;
}

void Flood::EulerAngles::Y::set(float value)
{
    __Y = value;
}

float Flood::EulerAngles::Z::get()
{
    return __Z;
}

void Flood::EulerAngles::Z::set(float value)
{
    __Z = value;
}

