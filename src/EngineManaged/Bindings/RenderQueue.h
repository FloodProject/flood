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
    value struct RenderState;
}

namespace Flood
{
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

        property Flood::RenderBatch^ Renderable
        {
            Flood::RenderBatch^ get();
            void set(Flood::RenderBatch^);
        }

        property Flood::Material^ Material
        {
            Flood::Material^ get();
            void set(Flood::Material^);
        }

        Flood::Matrix4x3 ModelMatrix;
        property int Priority
        {
            int get();
            void set(int);
        }

        private:
        Flood::RenderBatch^ __Renderable;
        Flood::Material^ __Material;
        int __Priority;
    };

    public ref class RenderBlock : ICppInstance
    {
    public:

        property ::RenderBlock* NativePtr;
        property System::IntPtr __Instance
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

        void AddState(Flood::RenderState renderState);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
