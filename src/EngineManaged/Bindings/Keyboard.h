/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Input/Keyboard.h>
#include "Device.h"

namespace Flood
{
    enum struct InputDeviceType;
    enum struct Keys;
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

        property Flood::InputDeviceType Type
        {
            Flood::InputDeviceType get();
        }
        property System::Collections::Generic::List<bool>^ KeyState
        {
            System::Collections::Generic::List<bool>^ get();
        }
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
            void raise(Flood::KeyEvent^ _0);
        }
    private:
        delegate void _KeyReleaseDelegate(const ::KeyEvent& _1);
        _KeyReleaseDelegate^ _KeyReleaseDelegateInstance;
        void _KeyReleaseRaise(const ::KeyEvent& _1);
        System::Action<Flood::KeyEvent^>^ _KeyRelease;
    public:
        event System::Action<Flood::KeyEvent^>^ KeyRelease
        {
            void add(System::Action<Flood::KeyEvent^>^ evt);
            void remove(System::Action<Flood::KeyEvent^>^ evt);
            void raise(Flood::KeyEvent^ _1);
        }
    private:
        delegate void _KeyTextDelegate(const ::KeyEvent& _2);
        _KeyTextDelegate^ _KeyTextDelegateInstance;
        void _KeyTextRaise(const ::KeyEvent& _2);
        System::Action<Flood::KeyEvent^>^ _KeyText;
    public:
        event System::Action<Flood::KeyEvent^>^ KeyText
        {
            void add(System::Action<Flood::KeyEvent^>^ evt);
            void remove(System::Action<Flood::KeyEvent^>^ evt);
            void raise(Flood::KeyEvent^ _2);
        }
        bool IsKeyPressed(Flood::Keys keyCode);

        void ResetKeys();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
