/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Input/Mouse.h>
#include "Device.h"
#include "MouseEvents.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct InputDeviceType;
    enum struct MouseButton : unsigned char;
    ref class InputDevice;
    ref class InputEvent;
    ref class Mouse;
    ref class MouseButtonEvent;
    ref class MouseDragEvent;
    ref class MouseInfo;
    ref class MouseMoveEvent;
    ref class MouseWheelEvent;

    /// <summary>
    /// Holds the mouse state.
    /// </summary>
    public ref class MouseInfo
    {
    public:
        property ::MouseInfo* NativePtr;

        MouseInfo(::MouseInfo* native);
        MouseInfo(System::IntPtr native);
        MouseInfo();
        property short X
        {
            short get();
            void set(short);
        }
        property short Y
        {
            short get();
            void set(short);
        }
        property bool InsideWindow
        {
            bool get();
            void set(bool);
        }
        property bool LeftButton
        {
            bool get();
            void set(bool);
        }
        property bool RightButton
        {
            bool get();
            void set(bool);
        }
        property bool MiddleButton
        {
            bool get();
            void set(bool);
        }
        property bool Aux1Button
        {
            bool get();
            void set(bool);
        }
        property bool Aux2Button
        {
            bool get();
            void set(bool);
        }
    };

    public ref class Mouse : Flood::InputDevice
    {
    public:
        Mouse(::Mouse* native);
        Mouse(System::IntPtr native);
        Mouse();
    private:
        delegate void _MouseMoveDelegate(const ::MouseMoveEvent& _0);
        _MouseMoveDelegate^ _MouseMoveDelegateInstance;
        void _MouseMoveRaise(const ::MouseMoveEvent& _0);
        System::Action<Flood::MouseMoveEvent^>^ _MouseMove;
    public:
        event System::Action<Flood::MouseMoveEvent^>^ MouseMove
        {
            void add(System::Action<Flood::MouseMoveEvent^>^ evt);
            void remove(System::Action<Flood::MouseMoveEvent^>^ evt);
            void raise(Flood::MouseMoveEvent^ _0);
        }
    private:
        delegate void _MouseDragDelegate(const ::MouseDragEvent& _1);
        _MouseDragDelegate^ _MouseDragDelegateInstance;
        void _MouseDragRaise(const ::MouseDragEvent& _1);
        System::Action<Flood::MouseDragEvent^>^ _MouseDrag;
    public:
        event System::Action<Flood::MouseDragEvent^>^ MouseDrag
        {
            void add(System::Action<Flood::MouseDragEvent^>^ evt);
            void remove(System::Action<Flood::MouseDragEvent^>^ evt);
            void raise(Flood::MouseDragEvent^ _1);
        }
    private:
        delegate void _MouseButtonPressDelegate(const ::MouseButtonEvent& _2);
        _MouseButtonPressDelegate^ _MouseButtonPressDelegateInstance;
        void _MouseButtonPressRaise(const ::MouseButtonEvent& _2);
        System::Action<Flood::MouseButtonEvent^>^ _MouseButtonPress;
    public:
        event System::Action<Flood::MouseButtonEvent^>^ MouseButtonPress
        {
            void add(System::Action<Flood::MouseButtonEvent^>^ evt);
            void remove(System::Action<Flood::MouseButtonEvent^>^ evt);
            void raise(Flood::MouseButtonEvent^ _2);
        }
    private:
        delegate void _MouseButtonReleaseDelegate(const ::MouseButtonEvent& _3);
        _MouseButtonReleaseDelegate^ _MouseButtonReleaseDelegateInstance;
        void _MouseButtonReleaseRaise(const ::MouseButtonEvent& _3);
        System::Action<Flood::MouseButtonEvent^>^ _MouseButtonRelease;
    public:
        event System::Action<Flood::MouseButtonEvent^>^ MouseButtonRelease
        {
            void add(System::Action<Flood::MouseButtonEvent^>^ evt);
            void remove(System::Action<Flood::MouseButtonEvent^>^ evt);
            void raise(Flood::MouseButtonEvent^ _3);
        }
    private:
        delegate void _MouseWheelMoveDelegate(const ::MouseWheelEvent& _4);
        _MouseWheelMoveDelegate^ _MouseWheelMoveDelegateInstance;
        void _MouseWheelMoveRaise(const ::MouseWheelEvent& _4);
        System::Action<Flood::MouseWheelEvent^>^ _MouseWheelMove;
    public:
        event System::Action<Flood::MouseWheelEvent^>^ MouseWheelMove
        {
            void add(System::Action<Flood::MouseWheelEvent^>^ evt);
            void remove(System::Action<Flood::MouseWheelEvent^>^ evt);
            void raise(Flood::MouseWheelEvent^ _4);
        }
    private:
        delegate void _MouseEnterDelegate();
        _MouseEnterDelegate^ _MouseEnterDelegateInstance;
        void _MouseEnterRaise();
        System::Action^ _MouseEnter;
    public:
        event System::Action^ MouseEnter
        {
            void add(System::Action^ evt);
            void remove(System::Action^ evt);
            void raise();
        }
    private:
        delegate void _MouseExitDelegate();
        _MouseExitDelegate^ _MouseExitDelegateInstance;
        void _MouseExitRaise();
        System::Action^ _MouseExit;
    public:
        event System::Action^ MouseExit
        {
            void add(System::Action^ evt);
            void remove(System::Action^ evt);
            void raise();
        }
        bool IsButtonPressed(Flood::MouseButton button);
        void ProcessEvent(Flood::InputEvent^ event);
        Flood::MouseInfo^ GetMouseInfo();
        Flood::InputDeviceType GetType();
    };
}
