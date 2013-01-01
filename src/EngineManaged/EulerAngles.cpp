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

Flood::EulerAngles::EulerAngles(float x, float y, float z)
{
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::EulerAngles)
{
    return EulerAngles();
}

Flood::EulerAngles Flood::EulerAngles::operator+=(Flood::Vector3)
{
    return EulerAngles();
}

void Flood::EulerAngles::Rotate(float x, float y, float z)
{
}

void Flood::EulerAngles::Identity()
{
}


