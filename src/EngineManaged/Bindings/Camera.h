/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Scene/Camera.h>
#include "Component.h"
#include "Frustum.h"
#include "Matrix4x3.h"
#include "Vector.h"

namespace Flood
{
    enum struct DebugDrawFlags : unsigned char;
    ref class Camera;
    ref class DebugDrawer;
    ref class Entity;
    ref class RenderBlock;
    ref class RenderDevice;
    ref class RenderView;
    ref class Transform;
    value struct Ray;

    /// <summary>
    /// Represents a view from a specific point in the world. Has an associated
    /// projection type, like ortographic or perspective and also holds a frustum
    /// that will be used for performing viewing frustum culling. Culling helps
    /// speed up the rendering by cutting nodes that are outside the view range.
    /// </summary>
    public ref class Camera : Flood::Component
    {
    public:

        Camera(::Camera* native);
        Camera(System::IntPtr native);
        Camera();

        property Flood::Vector3 LookAtVector
        {
            Flood::Vector3 get();
        }
        property Flood::Matrix4x3 ViewMatrix
        {
            Flood::Matrix4x3 get();
        }
        property Flood::RenderView^ View
        {
            Flood::RenderView^ get();
            void set(Flood::RenderView^);
        }
        property Flood::Frustum Frustum
        {
            Flood::Frustum get();
        }
        property Flood::Frustum Frustum1
        {
            Flood::Frustum get();
        }
        property Flood::DebugDrawer^ Drawer
        {
            Flood::DebugDrawer^ get();
            void set(Flood::DebugDrawer^);
        }
        void Render(Flood::RenderBlock^ block, bool clearView);

        void Cull(Flood::RenderBlock^ queue, Flood::Entity^ entity);

        Flood::Ray GetRay(float x, float y, Flood::Vector3 outFar);

        Flood::Frustum GetVolume(float screenLeft, float screenRight, float screenTop, float screenBottom);

        virtual void Update(float delta) override;

        void UpdateFrustum();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
