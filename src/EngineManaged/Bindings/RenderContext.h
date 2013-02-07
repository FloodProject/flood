/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderContext.h>

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
    ref class Settings;
    ref class RenderContext;
    ref class Event1;
    ref class Delegate0;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class RenderView;
    value struct Vector2i;
    value struct Vector3;
    ref class RefPtr;

    /// <summary>
    /// Represents a rendering context.
    /// </summary>
    public ref class RenderContext
    {
    public:
        property ::RenderContext* NativePtr;

        RenderContext(::RenderContext* native);
        RenderContext(System::IntPtr native);
        property bool InitDone;
        property Flood::RenderTarget^ MainTarget;
        RenderContext();
        void MakeCurrent();
        void Init();
        void ResetState();
        Flood::Color GetPixel(unsigned short x, unsigned short y);
    };
}
