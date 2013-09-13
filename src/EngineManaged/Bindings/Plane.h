/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Plane.h>
#include "Vector.h"

namespace Flood
{
    enum struct PlaneIntersection;
    value struct Plane;
    value struct Ray;

    public enum struct PlaneIntersection
    {
        OnPlane = 0,
        NegativeSide = 1,
        PositiveSide = 2
    };

    public value struct Plane
    {
    public:

        Plane(::Plane* native);
        Plane(System::IntPtr native);
        Plane(Flood::Vector3 normal, Flood::Vector3 point);

        Plane(Flood::Vector3 normal, float distance);

        Flood::Vector3 Normal;
        float Offset;
        void Normalize();

        float Distance(Flood::Vector3 point);

        float Distance(float x, float y, float z);

        Flood::Vector3 Project(Flood::Vector3 vec);

        bool Intersects(Flood::Ray ray, [System::Runtime::InteropServices::Out] float% distance);

    };
}
