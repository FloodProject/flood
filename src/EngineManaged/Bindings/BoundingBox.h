/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/BoundingBox.h>
#include "Vector.h"

namespace Flood
{
    value struct BoundingBox;
    value struct Matrix4x3;
    value struct Ray;
}

namespace Flood
{
    /// <summary>
    /// Axis-aligned Bounding Box.
    /// </summary>
    public value struct BoundingBox
    {
    public:

        BoundingBox(::BoundingBox* native);
        BoundingBox(System::IntPtr native);
        BoundingBox(Flood::Vector3 min, Flood::Vector3 max);

        property Flood::Vector3 Center
        {
            Flood::Vector3 get();
        }

        Flood::Vector3 Min;
        Flood::Vector3 Max;
        void Add(Flood::Vector3 vertex);

        void Add(Flood::BoundingBox box);

        void Reset();

        void SetZero();

        bool IsInfinite();

        Flood::Vector3 GetCorner(int index);

        bool Intersects(Flood::Ray ray, [System::Runtime::InteropServices::Out] float% distance);

        Flood::BoundingBox Transform(Flood::Matrix4x3 _0);
    };
}
