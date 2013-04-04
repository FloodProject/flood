/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "EulerAngles.h"
#include "ResourceHandle.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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
    auto _native = ::EulerAngles(x, y, z);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::EulerAngles _0)
{
    auto _this0 = ::EulerAngles();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::EulerAngles();
    _marshal0.x = _0.X;
    _marshal0.y = _0.Y;
    _marshal0.z = _0.Z;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator+=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::EulerAngles((::EulerAngles*)&ret);
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::Vector3 _0)
{
    auto _this0 = ::EulerAngles();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    auto _marshal0 = ::Vector3();
    _marshal0.x = _0.X;
    _marshal0.y = _0.Y;
    _marshal0.z = _0.Z;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator+=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    return Flood::EulerAngles((::EulerAngles*)&ret);
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

