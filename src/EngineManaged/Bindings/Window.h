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
#include "Color.h"

namespace Flood
{
    enum struct WindowStyles;
    value struct WindowSettings;
    ref class Window;
    ref class Event0;
    ref class Delegate0;
    ref class ClosurePtr;
    ref class GenericClass;
    ref class DelegateMemento;
    ref class Event1;
    ref class RefPtr;
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

    [System::Flags]
    public enum struct WindowStyles
    {
        None = 0,
        TopLevel = 1,
        MiniFrame = 2
    };

    public value struct WindowSettings
    {
    public:
        WindowSettings(::WindowSettings* native);
        WindowSettings(System::IntPtr native);
        WindowSettings(unsigned short width, unsigned short height, System::String^ title, bool fullscreen);
        System::String^ Title;
        bool FullScreen;
        System::IntPtr Handle;
        Flood::WindowStyles Styles;
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
        Window(::Window* native);
        Window(System::IntPtr native);
        Window(Flood::WindowSettings settings);
        property Flood::WindowSettings Settings;
        Flood::RenderContext^ CreateContext(Flood::RenderContextSettings^ _224);
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
        bool HasFocus();
        Flood::InputManager^ GetInput();
        Flood::Settings GetSettings();
    };
}
