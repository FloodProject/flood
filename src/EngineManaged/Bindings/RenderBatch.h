/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/RenderBatch.h>
#include "Material.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct PrimitiveRasterMode : unsigned char;
    enum struct PrimitiveType : unsigned char;
    enum struct RenderLayer : unsigned char;
    ref class Allocator;
    ref class GeometryBuffer;
    ref class RenderBatch;
    ref class RenderView;
    ref class UniformBuffer;
    value struct RenderBatchRange;
    value struct RenderState;

    /// <summary>
    /// Type of primitive of the RenderBatch.
    /// </summary>
    public enum struct PrimitiveType : unsigned char
    {
        Points = 0,
        Lines = 1,
        LineLoop = 2,
        LineStrip = 3,
        Triangles = 4,
        TriangleStrip = 5,
        TriangleFan = 6,
        Quads = 7,
        QuadStrip = 8,
        Polygon = 9
    };

    /// <summary>
    /// Type of rendering mode of the RenderBatch.
    /// </summary>
    public enum struct PrimitiveRasterMode : unsigned char
    {
        Solid = 0,
        Wireframe = 1,
        Point = 2
    };

    /// <summary>
    /// Use these different kinds of render groups to signal to the renderer how
    /// you want your RenderBatchs to be sorted by the render device. Lower numbers
    /// render before higher numbers.
    /// </summary>
    public enum struct RenderLayer : unsigned char
    {
        Normal = 0,
        Transparency = 5,
        PostTransparency = 7,
        Overlays = 10
    };

    /// <summary>
    /// The range of indices in the render batch
    /// </summary>
    public value struct RenderBatchRange
    {
    public:

        RenderBatchRange(::RenderBatchRange* native);
        RenderBatchRange(System::IntPtr native);
        unsigned short Start;
        unsigned short End;
    };

    public ref class RenderBatch : ICppInstance
    {
    public:

        property ::RenderBatch* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        RenderBatch(::RenderBatch* native);
        RenderBatch(System::IntPtr native);
        RenderBatch();

        property Flood::RenderLayer RenderLayer
        {
            Flood::RenderLayer get();
            void set(Flood::RenderLayer);
        }
        property int RenderPriority
        {
            int get();
            void set(int);
        }
        property Flood::GeometryBuffer^ GeometryBuffer
        {
            Flood::GeometryBuffer^ get();
            void set(Flood::GeometryBuffer^);
        }
        property Flood::UniformBuffer^ UniformBuffer
        {
            Flood::UniformBuffer^ get();
            void set(Flood::UniformBuffer^);
        }
        property Flood::ResourceHandle<Flood::Material^> Material
        {
            Flood::ResourceHandle<Flood::Material^> get();
            void set(Flood::ResourceHandle<Flood::Material^>);
        }
        property Flood::PrimitiveRasterMode PrimitiveRasterMode
        {
            Flood::PrimitiveRasterMode get();
            void set(Flood::PrimitiveRasterMode);
        }
        property Flood::PrimitiveType PrimitiveType
        {
            Flood::PrimitiveType get();
            void set(Flood::PrimitiveType);
        }
        property Flood::RenderBatchRange Range
        {
            Flood::RenderBatchRange get();
            void set(Flood::RenderBatchRange);
        }
        property Flood::RenderLayer Stage
        {
            Flood::RenderLayer get();
            void set(Flood::RenderLayer);
        }
        property int Priority
        {
            int get();
            void set(int);
        }
        property Flood::PrimitiveType Type
        {
            Flood::PrimitiveType get();
            void set(Flood::PrimitiveType);
        }
        property Flood::PrimitiveRasterMode Mode
        {
            Flood::PrimitiveRasterMode get();
            void set(Flood::PrimitiveRasterMode);
        }
        property Flood::GeometryBuffer^ Gb
        {
            Flood::GeometryBuffer^ get();
            void set(Flood::GeometryBuffer^);
        }
        property Flood::UniformBuffer^ Ub
        {
            Flood::UniformBuffer^ get();
            void set(Flood::UniformBuffer^);
        }
        property Flood::ResourceHandle<Flood::Material^> Material1
        {
            Flood::ResourceHandle<Flood::Material^> get();
            void set(Flood::ResourceHandle<Flood::Material^>);
        }
        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };

    public ref class FloodRenderBatch
    {
    public:
        static Flood::RenderBatch^ RenderBatchCreate(Flood::Allocator^ _0);
    };
}
