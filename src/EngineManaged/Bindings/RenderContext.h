/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderContext.h>
#include "Vector.h"
#include "Color.h"

namespace Flood
{
    ref class RenderCapabilities;
    ref class RenderDevice;
    ref class RenderTarget;
    ref class RenderBackend;
    ref class BufferManager;
    ref class ProgramManager;
    ref class TextureManager;
    value struct Color;
    ref class RenderBuffer;
    value struct Settings;
    ref class RenderContextSettings;
    ref class RenderContext;
    value struct Vector3;
    ref class RefPtr;

    public ref class RenderContextSettings
    {
    public:
        property ::RenderContextSettings* NativePtr;

        RenderContextSettings(::RenderContextSettings* native);
        RenderContextSettings(System::IntPtr native);
        RenderContextSettings();
        property unsigned short BitsPerPixel;
        property unsigned short DepthBits;
        property unsigned short StencilBits;
        property unsigned short AntialiasLevel;
    };

    /// <summary>
    /// Represents a rendering context.
    /// </summary>
    public ref class RenderContext
    {
    public:
        property ::RenderContext* NativePtr;

        RenderContext(::RenderContext* native);
        RenderContext(System::IntPtr native);
        RenderContext();
        property bool InitDone;
        property Flood::RenderTarget^ CurrentTarget;
        void MakeCurrent(Flood::RenderTarget^ target);
        void SetTarget(Flood::RenderTarget^ target);
        void Init();
        void ResetState();
        Flood::Color GetPixel(unsigned short x, unsigned short y);
    };
}
