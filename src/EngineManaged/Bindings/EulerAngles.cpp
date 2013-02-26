/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "EulerAngles.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::EulerAngles::EulerAngles(::EulerAngles* native)
{
    // TODO: Struct marshaling
}

Flood::EulerAngles::EulerAngles(System::IntPtr native)
{
    // TODO: Struct marshaling
}

Flood::EulerAngles::EulerAngles(float x, float y, float z)
{
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::EulerAngles _98)
{
    auto this0 = (::EulerAngles*) 0;
    auto _arg0 = (::EulerAngles*)&_98;
    auto arg0 = *_arg0;
    auto ret = this0->operator+=(arg0);
    return Flood::EulerAngles((::EulerAngles*)&ret);
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::Vector3 _99)
{
    auto this0 = (::EulerAngles*) 0;
    auto _arg0 = (::Vector3*)&_99;
    auto arg0 = *_arg0;
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

