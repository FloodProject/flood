/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Scene/Geometry.h>
#include "BoundingBox.h"
#include "Component.h"

namespace Flood
{
    ref class Geometry;
    ref class RenderBatch;
    ref class Transform;
    value struct RenderState;
}

namespace Flood
{
    /// <summary>
    /// This component holds geometry data for rendering. It can hold multiple
    /// renderable objects, each with different materials. When the camera node
    /// traverses the scene graph, Geometry nodes are picked and passed to the
    /// rendering device for rendering.
    /// </summary>
    public ref class Geometry : Flood::Component
    {
    public:

        Geometry(::Geometry* native);
        Geometry(System::IntPtr native);
        Geometry();

        property System::Collections::Generic::List<Flood::RenderBatch^>^ Renderables
        {
            System::Collections::Generic::List<Flood::RenderBatch^>^ get();
        }

        property Flood::BoundingBox BoundingVolume
        {
            Flood::BoundingBox get();
        }

        property Flood::BoundingBox WorldBoundingVolume
        {
            Flood::BoundingBox get();
        }

        void AddRenderable(Flood::RenderBatch^ renderable);

        virtual void AppendRenderables(System::Collections::Generic::List<Flood::RenderState>^ queue, Flood::Transform^ transform);

        virtual void UpdateBounds();

        virtual void Update(float delta) override;

        void RebuildBoundingBox();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
