/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Ray.h>
#include "Vector.h"

namespace Flood
{
    value struct Ray;
}

namespace Flood
{
    /// <summary>
    /// A ray is an infinite line starting at origin and going in some direction.
    /// </summary>
    public value struct Ray
    {
    public:

        Ray(::Ray* native);
        Ray(System::IntPtr native);
        Ray(Flood::Vector3 origin, Flood::Vector3 direction);

        Flood::Vector3 Origin;
        Flood::Vector3 Direction;
        Flood::Vector3 GetPoint(float distance);
    };
}
