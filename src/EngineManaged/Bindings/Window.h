/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Window/Window.h>
#include "RenderTarget.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct WindowStyles;
    ref class InputManager;
    ref class RenderContext;
    ref class RenderTarget;
    ref class Window;
    value struct RenderContextSettings;
    value struct Settings;
    value struct Vector2i;
    value struct WindowSettings;

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
        unsigned short Width;
        unsigned short Height;
        System::String^ Title;
        bool FullScreen;
        System::IntPtr Handle;
        Flood::WindowStyles Styles;
    };

    /// <summary>
    /// In most platforms (PCs) this will be just a normal window on the desktop,
    /// but on some platforms (consoles, for instance) this might be slighty
    /// different, so some methods might not make much sense.
    /// </summary>
    public ref class Window : Flood::RenderTarget
    {
    public:
        Window(::Window* native);
        Window(System::IntPtr native);
        Window(Flood::WindowSettings settings);
    private:
        delegate void _WindowCloseDelegate();
        _WindowCloseDelegate^ _WindowCloseDelegateInstance;
        void _WindowCloseRaise();
        System::Action^ _WindowClose;
    public:
        event System::Action^ WindowClose
        {
            void add(System::Action^ evt);
            void remove(System::Action^ evt);
            void raise();
        }
    private:
        delegate void _WindowFocusChangeDelegate(bool _2);
        _WindowFocusChangeDelegate^ _WindowFocusChangeDelegateInstance;
        void _WindowFocusChangeRaise(bool _2);
        System::Action<bool>^ _WindowFocusChange;
    public:
        event System::Action<bool>^ WindowFocusChange
        {
            void add(System::Action<bool>^ evt);
            void remove(System::Action<bool>^ evt);
            void raise(bool _2);
        }
        Flood::RenderContext^ CreateContext(Flood::RenderContextSettings _0);
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
