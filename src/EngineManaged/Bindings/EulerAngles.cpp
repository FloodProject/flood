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
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::EulerAngles _86)
{
    auto this0 = (::EulerAngles*) 0;
    auto arg0 = ::EulerAngles();
    arg0.x = _86.X;
    arg0.y = _86.Y;
    arg0.z = _86.Z;

    auto ret = this0->operator+=(arg0);
    return Flood::EulerAngles((::EulerAngles*)&ret);
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::Vector3 _87)
{
    auto this0 = (::EulerAngles*) 0;
    auto arg0 = ::Vector3();
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

