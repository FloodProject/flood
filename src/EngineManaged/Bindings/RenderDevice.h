/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/RenderDevice.h>

namespace Flood
{
    enum struct RenderPipeline;
    ref class Material;
    ref class RenderBackend;
    ref class RenderBatch;
    ref class RenderBlock;
    ref class RenderBuffer;
    ref class RenderContext;
    ref class RenderDevice;
    ref class RenderTarget;
    ref class RenderView;
    ref class Texture;
    value struct RenderState;
}

namespace Flood
{
    /// <summary>
    /// There are a couple rendering pipelines. You can choose each one depending
    /// on the hardware functionality (shaders don't exist on some old or mobile
    /// hardware) or user preference (fixed function is faster on some machines).
    /// </summary>
    public enum struct RenderPipeline
    {
        Fixed = 0,
        ShaderForward = 1,
        ShaderDeferred = 2
    };

    public ref class RenderDevice : ICppInstance
    {
    public:

        property ::RenderDevice* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        RenderDevice(::RenderDevice* native);
        RenderDevice(System::IntPtr native);
        RenderDevice();

        property Flood::RenderPipeline Pipeline
        {
            Flood::RenderPipeline get();
            void set(Flood::RenderPipeline);
        }

        property Flood::RenderBackend^ Backend
        {
            Flood::RenderBackend^ get();
            void set(Flood::RenderBackend^);
        }

        property Flood::RenderTarget^ RenderTarget
        {
            Flood::RenderTarget^ get();
            void set(Flood::RenderTarget^);
        }

        property Flood::RenderContext^ ActiveContext
        {
            Flood::RenderContext^ get();
            void set(Flood::RenderContext^);
        }

        property Flood::RenderView^ ActiveView
        {
            Flood::RenderView^ get();
            void set(Flood::RenderView^);
        }

        void Render(Flood::RenderBlock^ queue);

        void UpdateRenderTargets();

        void ClearView();

        bool IsFixedPipeline();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

        static Flood::RenderDevice^ GetRenderDevice();
    };
}
