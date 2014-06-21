/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/RenderContext.h>

namespace Flood
{
    ref class BufferManager;
    ref class ProgramManager;
    ref class RenderBackend;
    ref class RenderBuffer;
    ref class RenderCapabilities;
    ref class RenderContext;
    ref class RenderDevice;
    ref class RenderTarget;
    ref class TextureManager;
    value struct Color;
    value struct RenderContextSettings;
    value struct Settings;
}

namespace Flood
{
    public value struct RenderContextSettings
    {
    public:

        RenderContextSettings(::RenderContextSettings* native);
        RenderContextSettings(System::IntPtr native);
        property unsigned short BitsPerPixel
        {
            unsigned short get();
            void set(unsigned short);
        }

        property unsigned short DepthBits
        {
            unsigned short get();
            void set(unsigned short);
        }

        property unsigned short StencilBits
        {
            unsigned short get();
            void set(unsigned short);
        }

        property unsigned short AntialiasLevel
        {
            unsigned short get();
            void set(unsigned short);
        }

        private:
        unsigned short __BitsPerPixel;
        unsigned short __DepthBits;
        unsigned short __StencilBits;
        unsigned short __AntialiasLevel;
    };

    /// <summary>
    /// Represents a rendering context.
    /// </summary>
    public ref class RenderContext : ICppInstance
    {
    public:

        property ::RenderContext* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        RenderContext(::RenderContext* native);
        RenderContext(System::IntPtr native);
        RenderContext();

        property Flood::RenderTarget^ Target
        {
            void set(Flood::RenderTarget^);
        }

        property bool InitDone
        {
            bool get();
            void set(bool);
        }

        property Flood::RenderTarget^ CurrentTarget
        {
            Flood::RenderTarget^ get();
            void set(Flood::RenderTarget^);
        }

        property Flood::RenderBackend^ Backend
        {
            Flood::RenderBackend^ get();
            void set(Flood::RenderBackend^);
        }

        virtual void MakeCurrent(Flood::RenderTarget^ target);

        void Init();

        void ResetState();

        Flood::Color GetPixel(unsigned short x, unsigned short y);

        Flood::RenderBuffer^ CreateRenderBuffer(Flood::Settings _0);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
