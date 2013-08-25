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
    X = native->x;
    Y = native->y;
    Z = native->z;
}

Flood::EulerAngles::EulerAngles(System::IntPtr native)
{
    auto __native = (::EulerAngles*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Z = __native->z;
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
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
}

void Flood::EulerAngles::Identity()
{
    auto _this0 = ::EulerAngles();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.identity();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
}

