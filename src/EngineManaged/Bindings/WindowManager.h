/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Window/WindowManager.h>
#include "Window.h"
#include "RenderTarget.h"
#include "Vector.h"
#include "Color.h"

namespace Flood
{
    ref class Window;
    ref class WindowManager;
    ref class Event0;
    ref class Delegate0;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class Event1;
    ref class RefPtr;
    value struct WindowSettings;
    enum struct WindowStyles;
    ref class RenderContext;
    ref class RenderCapabilities;
    ref class BufferManager;
    ref class TextureManager;
    ref class ProgramManager;
    ref class RenderTarget;
    ref class RenderBackend;
    value struct Color;
    value struct Vector3;
    ref class RenderBuffer;
    value struct Settings;
    ref class RenderContextSettings;
    value struct Vector2i;
    ref class InputManager;
    ref class InputDevice;
    ref class Keyboard;
    ref class Mouse;
    ref class InputEvent;

    /// <summary>
    /// Window manager manages windows (Windows baby!).
    /// </summary>
    public ref class WindowManager
    {
    public:
        property ::WindowManager* NativePtr;

        WindowManager(::WindowManager* native);
        WindowManager(System::IntPtr native);
        WindowManager();
        Flood::Window^ CreateWindow(Flood::WindowSettings settings);
        void DestroyWindows();
    };
}
