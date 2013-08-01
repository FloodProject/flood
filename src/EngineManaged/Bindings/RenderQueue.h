/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/RenderQueue.h>
#include "Matrix4x3.h"
#include "Matrix4x4.h"

namespace Flood
{
    ref class Light;
    ref class Material;
    ref class RenderBatch;
    ref class RenderBlock;
    ref class Texture;
    ref class Transform;
    value struct LightState;
    value struct RenderState;

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

    public value struct LightState
    {
    public:
        LightState(::LightState* native);
        LightState(System::IntPtr native);
        Flood::Texture^ Depth;
        Flood::Matrix4x4 Projection;
    };

    public ref class RenderBlock : ICppInstance
    {
    public:
        property ::RenderBlock* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

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
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
