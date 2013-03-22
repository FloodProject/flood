/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Input/Keyboard.h>
#include "Device.h"
#include "KeyboardEvents.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct InputDeviceType;
    enum struct Keys;
    ref class Event1;
    ref class InputDevice;
    ref class InputEvent;
    ref class Keyboard;
    ref class KeyEvent;

    /// <summary>
    /// Keyboard device.
    /// </summary>
    public ref class Keyboard : Flood::InputDevice
    {
    public:
        Keyboard(::Keyboard* native);
        Keyboard(System::IntPtr native);
        Keyboard();
    private:
        delegate void _KeyPressDelegate(const ::KeyEvent& _0);
        _KeyPressDelegate^ _KeyPressDelegateInstance;
        void _KeyPressRaise(const ::KeyEvent& _0);
        System::Action<Flood::KeyEvent^>^ _KeyPress;
    public:
        event System::Action<Flood::KeyEvent^>^ KeyPress
        {
            void add(System::Action<Flood::KeyEvent^>^ evt);
            void remove(System::Action<Flood::KeyEvent^>^ evt);
            void raise(Flood::KeyEvent^);
        }
    private:
        delegate void _KeyReleaseDelegate(const ::KeyEvent& _0);
        _KeyReleaseDelegate^ _KeyReleaseDelegateInstance;
        void _KeyReleaseRaise(const ::KeyEvent& _0);
        System::Action<Flood::KeyEvent^>^ _KeyRelease;
    public:
        event System::Action<Flood::KeyEvent^>^ KeyRelease
        {
            void add(System::Action<Flood::KeyEvent^>^ evt);
            void remove(System::Action<Flood::KeyEvent^>^ evt);
            void raise(Flood::KeyEvent^);
        }
    private:
        delegate void _KeyTextDelegate(const ::KeyEvent& _0);
        _KeyTextDelegate^ _KeyTextDelegateInstance;
        void _KeyTextRaise(const ::KeyEvent& _0);
        System::Action<Flood::KeyEvent^>^ _KeyText;
    public:
        event System::Action<Flood::KeyEvent^>^ KeyText
        {
            void add(System::Action<Flood::KeyEvent^>^ evt);
            void remove(System::Action<Flood::KeyEvent^>^ evt);
            void raise(Flood::KeyEvent^);
        }
        bool IsKeyPressed(Flood::Keys keyCode);
        void ResetKeys();
    };
}
