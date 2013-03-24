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
    auto this0 = (::EulerAngles*) 0;
    auto _marshal0 = ::EulerAngles();
    _marshal0.x = _0.X;
    _marshal0.y = _0.Y;
    _marshal0.z = _0.Z;
    auto arg0 = _marshal0;
    auto ret = this0->operator+=(arg0);
    return Flood::EulerAngles((::EulerAngles*)&ret);
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::Vector3 _0)
{
    auto this0 = (::EulerAngles*) 0;
    auto _marshal0 = ::Vector3();
    _marshal0.x = _0.X;
    _marshal0.y = _0.Y;
    _marshal0.z = _0.Z;
    auto arg0 = _marshal0;
    auto ret = this0->operator+=(arg0);
    return Flood::EulerAngles((::EulerAngles*)&ret);
}

void Flood::EulerAngles::Rotate(float x, float y, float z)
{
    auto this0 = (::EulerAngles*) 0;
    this0->rotate(x, y, z);
}

void Flood::EulerAngles::Identity()
{
    auto this0 = (::EulerAngles*) 0;
    this0->identity();
}

