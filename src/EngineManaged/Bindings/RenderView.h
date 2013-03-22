/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderView.h>
#include "Color.h"
#include "Matrix4x3.h"
#include "Matrix4x4.h"
#include "ResourceHandle.h"
#include "Vector.h"

namespace Flood
{
    ref class RenderTarget;
    ref class RenderView;
    value struct Color;
    value struct Matrix4x3;
    value struct Matrix4x4;
    value struct Vector2i;

    /// <summary>
    /// A view is a region in a render target that can be rendered into. It has an
    /// associated renderer that will render into the view.
    /// </summary>
    public ref class RenderView
    {
    public:
        property ::RenderView* NativePtr;

        RenderView(::RenderView* native);
        RenderView(System::IntPtr native);
        RenderView();
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
        property Flood::RenderTarget^ Target
        {
            Flood::RenderTarget^ get();
            void set(Flood::RenderTarget^);
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
        Flood::RenderTarget^ GetRenderTarget();
        void SetRenderTarget(Flood::RenderTarget^ v);
        Flood::Vector2i GetOrigin();
        void SetOrigin(Flood::Vector2i v);
        Flood::Vector2i GetSize();
        void SetSize(Flood::Vector2i v);
        Flood::Color GetClearColor();
        void SetClearColor(Flood::Color v);
        int GetDepthPriority();
        void SetDepthPriority(int v);
        float GetAspectRatio();
        bool operator<(Flood::RenderView^ view);
        void HandleRenderTargetResize();
    };
}
