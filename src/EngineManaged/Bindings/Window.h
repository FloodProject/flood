/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Window/Window.h>
#include "RenderTarget.h"
#include "Vector.h"

namespace Flood
{
    value struct WindowSettings;
    ref class Window;
    ref class Event0;
    ref class Delegate0;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class Event1;
    value struct Vector2i;
    value struct Vector3;
    ref class InputManager;
    ref class InputDevice;
    ref class Keyboard;
    ref class Mouse;
    ref class InputEvent;
    value struct Settings;

    public value struct WindowSettings
    {
    public:
        WindowSettings(::WindowSettings* native);
        WindowSettings(System::IntPtr native);
        WindowSettings(unsigned short width, unsigned short height, System::String^ title, bool fullscreen);
        System::String^ Title;
        bool FullScreen;
        unsigned short BitsPerPixel;
        unsigned short DepthBits;
        unsigned short StencilBits;
        unsigned short AntialiasLevel;
        System::IntPtr Handle;
        unsigned short Width;
        unsigned short Height;
    };

    /// <summary>
    /// In most platforms (PCs) this will be just a normal window on the desktop,
    /// but on some platforms (consoles, for instance) this might be slighty
    /// different, so some methods might not make much sense.
    /// </summary>
    public ref class Window : RenderTarget
    {
    public:
        property ::Window* NativePtr;

        Window(::Window* native);
        Window(System::IntPtr native);
        Window(Flood::WindowSettings settings);
        property Flood::WindowSettings Settings;
        void Update();
        void MakeCurrent();
        void Show(bool visible);
        bool PumpEvents();
        void SetTitle(System::String^ title);
        void SetCursorVisible(bool state);
        bool IsCursorVisible();
        void SetCursorCapture(bool state);
        Flood::Vector2i GetCursorPosition();
        void SetCursorPosition(int x, int y);
        void SetCursorPosition(Flood::Vector2i pos);
        bool HasFocus();
        Flood::InputManager^ GetInput();
        Flood::Settings GetSettings();
    };
}
