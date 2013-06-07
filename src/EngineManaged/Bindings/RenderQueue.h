/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderQueue.h>
#include "Matrix4x3.h"
#include "Matrix4x4.h"
#include "ResourceHandle.h"

namespace Flood
{
    ref class Light;
    ref class Material;
    ref class RenderBatch;
    ref class RenderBlock;
    ref class Texture;
    ref class Transform;
    value struct LightState;
    value struct Matrix4x3;
    value struct Matrix4x4;
    value struct RenderState;

    /// <summary>
    /// This contains all the rendering state information that is needed by the
    /// rendering device to properly render the associated.renderable. This should
    /// be set by the camera and then appended to a render queue where it will be
    /// sorted to minimize the state changes and finally rendered to the render
    /// target that is currently active on the device.
    /// </summary>
    public value struct RenderState
    {
    public:
        RenderState(::RenderState* native);
        RenderState(System::IntPtr native);
        RenderState(Flood::RenderBatch^ renderable);
        Flood::RenderBatch^ Renderable;
        Flood::Material^ Material;
        Flood::Matrix4x3 ModelMatrix;
        int Priority;
    };

    /// <summary>
    /// Light properties that will be passed down to the renderer.
    /// </summary>
    public value struct LightState
    {
    public:
        LightState(::LightState* native);
        LightState(System::IntPtr native);
        Flood::Texture^ Depth;
        Flood::Matrix4x4 Projection;
    };

    public ref class RenderBlock
    {
    public:
        property ::RenderBlock* NativePtr;

        RenderBlock(::RenderBlock* native);
        RenderBlock(System::IntPtr native);
        RenderBlock();
        property System::Collections::Generic::List<Flood::RenderState>^ Renderables
        {
            System::Collections::Generic::List<Flood::RenderState>^ get();
            void set(System::Collections::Generic::List<Flood::RenderState>^);
        }
        property System::Collections::Generic::List<Flood::LightState>^ Lights
        {
            System::Collections::Generic::List<Flood::LightState>^ get();
            void set(System::Collections::Generic::List<Flood::LightState>^);
        }
        void AddState(Flood::RenderState renderState);
    };
}
