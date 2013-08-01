/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Window/Window.h>
#include "RenderTarget.h"

namespace Flood
{
    enum struct WindowStyles;
    ref class InputManager;
    ref class RenderContext;
    ref class Window;
    value struct RenderContextSettings;
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

    public ref class Window : Flood::RenderTarget
    {
    public:
        Window(::Window* native);
        Window(System::IntPtr native);
        Window(Flood::WindowSettings settings);
        property System::String^ Title
        {
            void set(System::String^);
        }
        property bool CursorVisible
        {
            void set(bool);
        }
        property bool CursorCapture
        {
            void set(bool);
        }
        property Flood::Vector2i CursorPosition
        {
            Flood::Vector2i get();
        }
        property Flood::InputManager^ Input
        {
            Flood::InputManager^ get();
        }
        property Flood::Settings Settings
        {
            Flood::Settings get();
        }
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
        delegate void _WindowFocusChangeDelegate(bool _0);
        _WindowFocusChangeDelegate^ _WindowFocusChangeDelegateInstance;
        void _WindowFocusChangeRaise(bool _0);
        System::Action<bool>^ _WindowFocusChange;
    public:
        event System::Action<bool>^ WindowFocusChange
        {
            void add(System::Action<bool>^ evt);
            void remove(System::Action<bool>^ evt);
            void raise(bool _0);
        }
        Flood::RenderContext^ CreateContext(Flood::RenderContextSettings _0);
        void Update();
        void MakeCurrent();
        void Show(bool visible);
        bool PumpEvents();
        bool IsCursorVisible();
        void SetCursorPosition(int x, int y);
        bool HasFocus();
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
