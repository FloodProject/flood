/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderView.h>
#include "Vector.h"
#include "Color.h"
#include "Matrix4x4.h"

namespace Flood
{
    ref class RenderTarget;
    ref class RenderView;
    ref class Matrix4x3;
    value struct Vector2i;
    value struct Vector3;
    value struct Color;

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
        property Flood::Vector2i Origin;
        property Flood::Vector2i Size;
        property Flood::RenderTarget^ Target;
        property Flood::Color ClearColor;
        property int DepthPriority;
        property Flood::Matrix4x4 ProjectionMatrix;
        property Flood::Matrix4x3^ ViewMatrix;
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
