/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Frustum.h>
#include "Matrix4x4.h"
#include "Vector.h"

namespace Flood
{
    enum struct FrustumProjection;
    value struct BoundingBox;
    value struct Frustum;
    value struct Matrix4x3;
    value struct Plane;
}

namespace Flood
{
    /// <summary>
    /// Projection types.
    /// </summary>
    public enum struct FrustumProjection
    {
        Orthographic = 0,
        Perspective = 1
    };

    /// <summary>
    /// Frustum is a shape similiar to a pyramid with the top truncated.
    /// </summary>
    public value struct Frustum
    {
    public:

        Frustum(::Frustum* native);
        Frustum(System::IntPtr native);
        property Flood::FrustumProjection Projection
        {
            Flood::FrustumProjection get();
            void set(Flood::FrustumProjection);
        }

        Flood::Matrix4x4 MatProjection;
        property float FieldOfView
        {
            float get();
            void set(float);
        }

        property float NearPlane
        {
            float get();
            void set(float);
        }

        property float FarPlane
        {
            float get();
            void set(float);
        }

        property float AspectRatio
        {
            float get();
            void set(float);
        }

        Flood::Vector3 OrthoSize;
        void UpdateProjection();

        void UpdatePlanes(Flood::Matrix4x3 matView);

        void UpdateCorners(Flood::Matrix4x3 matView);

        bool Intersects(Flood::BoundingBox box);

        private:
        Flood::FrustumProjection __Projection;
        float __FieldOfView;
        float __NearPlane;
        float __FarPlane;
        float __AspectRatio;
    };
}
