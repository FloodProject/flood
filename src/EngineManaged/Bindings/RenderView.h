/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/RenderView.h>
#include "Color.h"
#include "Matrix4x3.h"
#include "Matrix4x4.h"
#include "Vector.h"

namespace Flood
{
    ref class RenderTarget;
    ref class RenderView;

    /// <summary>
    /// A view is a region in a render target that can be rendered into. It has an
    /// associated renderer that will render into the view.
    /// </summary>
    public ref class RenderView : ICppInstance
    {
    public:

        property ::RenderView* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        RenderView(::RenderView* native);
        RenderView(System::IntPtr native);
        RenderView();

        property Flood::RenderTarget^ RenderTarget
        {
            Flood::RenderTarget^ get();
            void set(Flood::RenderTarget^);
        }
        property Flood::Vector2i Origin
        {
            Flood::Vector2i get();
            void set(Flood::Vector2i);
        }
        property Flood::Vector2i Size
        {
            Flood::Vector2i get();
            void set(Flood::Vector2i);
        }
        property Flood::Color ClearColor
        {
            Flood::Color get();
            void set(Flood::Color);
        }
        property int DepthPriority
        {
            int get();
            void set(int);
        }
        property float AspectRatio
        {
            float get();
        }
        property Flood::Vector2i Origin1
        {
            Flood::Vector2i get();
            void set(Flood::Vector2i);
        }
        property Flood::Vector2i Size1
        {
            Flood::Vector2i get();
            void set(Flood::Vector2i);
        }
        property Flood::RenderTarget^ Target
        {
            Flood::RenderTarget^ get();
            void set(Flood::RenderTarget^);
        }
        property Flood::Color ClearColor1
        {
            Flood::Color get();
            void set(Flood::Color);
        }
        property int DepthPriority1
        {
            int get();
            void set(int);
        }
        property Flood::Matrix4x4 ProjectionMatrix
        {
            Flood::Matrix4x4 get();
            void set(Flood::Matrix4x4);
        }
        property Flood::Matrix4x3 ViewMatrix
        {
            Flood::Matrix4x3 get();
            void set(Flood::Matrix4x3);
        }
        bool operator<(Flood::RenderView^ view);

        void HandleRenderTargetResize();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
