/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Geometry/DebugGeometry.h>
#include "Color.h"
#include "ContainerConverter.h"

namespace Flood
{
    enum struct DebugDrawFlags : unsigned char;
    ref class DebugDrawer;
    ref class GeometryBuffer;
    ref class RenderBatch;
    value struct BoundingBox;
    value struct Frustum;
    value struct Ray;
    value struct Vector3;
}

namespace Flood
{
    [System::Flags]
    public enum struct DebugDrawFlags : unsigned char
    {
        Selected = 1
    };

    /// <summary>
    /// Debug drawing is used to draw debugging representations of objects by
    /// components and scripts. To make this easier to the user, the rendering of
    /// objects is done in an immediate-mode like fashion, with the management of
    /// the vertex buffers and materials done by the engine. Picking of objects is
    /// also supported.
    /// </summary>
    public ref class DebugDrawer : ICppInstance
    {
    public:

        property ::DebugDrawer* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        DebugDrawer(::DebugDrawer* native);
        DebugDrawer(System::IntPtr native);
        DebugDrawer();

        property Flood::Color Color
        {
            void set(Flood::Color);
        }

        property Flood::Color CurrentColor
        {
            Flood::Color get();
            void set(Flood::Color);
        }

        property Flood::RenderBatch^ Lines
        {
            Flood::RenderBatch^ get();
            void set(Flood::RenderBatch^);
        }

        property Flood::RenderBatch^ Triangles
        {
            Flood::RenderBatch^ get();
            void set(Flood::RenderBatch^);
        }

        property Flood::RenderBatch^ Quads
        {
            Flood::RenderBatch^ get();
            void set(Flood::RenderBatch^);
        }

        property System::Collections::Generic::List<Flood::RenderBatch^>^ Renderables
        {
            System::Collections::Generic::List<Flood::RenderBatch^>^ get();
            void set(System::Collections::Generic::List<Flood::RenderBatch^>^);
        }

        void DrawBox(Flood::BoundingBox box);

        void DrawRay(Flood::Ray ray, float length);

        void DrawFrustum(Flood::Frustum frustum);

        void DrawIcon(Flood::Vector3 pos);

        void Reset();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
