/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/RenderTarget.h>
#include "Vector.h"
#include "Color.h"

namespace Flood
{
    ref class RenderView;
    value struct Settings;
    value struct Vector2i;
    value struct Vector3;
    ref class RenderContext;
    ref class RenderTarget;
    ref class Event1;
    ref class Delegate0;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class RenderCapabilities;
    ref class BufferManager;
    ref class TextureManager;
    ref class ProgramManager;
    ref class RenderBackend;
    value struct Color;
    ref class RenderBuffer;

    public value struct Settings
    {
    public:
        Settings(::Settings* native);
        Settings(System::IntPtr native);
        Settings(unsigned short width, unsigned short height);
        unsigned short Width;
        unsigned short Height;
        Flood::Vector2i GetSize();
    };

    public ref class RenderTarget
    {
    public:
        property ::RenderTarget* NativePtr;

        RenderTarget(::RenderTarget* native);
        RenderTarget(System::IntPtr native);
        RenderTarget();
        property Flood::RenderContext^ Context;
        Flood::RenderView^ CreateView();
        void RemoveViews();
        void MakeCurrent();
        void Update();
        Flood::Settings GetSettings();
        Flood::RenderContext^ GetContext();
        void SetContext(Flood::RenderContext^ context);
    };
}
