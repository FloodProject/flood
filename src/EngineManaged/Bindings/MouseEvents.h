/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Input/MouseEvents.h>
#include "Device.h"

namespace Flood
{
    enum struct MouseButton : unsigned char;
    enum struct MouseEventType : unsigned char;
    ref class MouseButtonEvent;
    ref class MouseDragEvent;
    ref class MouseEvent;
    ref class MouseInfo;
    ref class MouseMoveEvent;
    ref class MouseWheelEvent;

    public enum struct MouseButton : unsigned char
    {
        Left = 0,
        Right = 1,
        Middle = 2,
        Mouse4 = 3,
        Mouse5 = 4
    };

    public enum struct MouseEventType : unsigned char
    {
        MouseWheelMove = 0,
        MouseMove = 1,
        MouseDrag = 2,
        MousePress = 3,
        MouseRelease = 4,
        MouseEnter = 5,
        MouseExit = 6
    };

    public ref class MouseEvent : Flood::InputEvent
    {
    public:
        MouseEvent(::MouseEvent* native);
        MouseEvent(System::IntPtr native);
        MouseEvent(Flood::MouseEventType eventType);
        property Flood::MouseEventType EventType
        {
            Flood::MouseEventType get();
            void set(Flood::MouseEventType);
        }
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };

    public ref class MouseMoveEvent : Flood::MouseEvent
    {
    public:
        MouseMoveEvent(::MouseMoveEvent* native);
        MouseMoveEvent(System::IntPtr native);
        MouseMoveEvent();
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
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };

    public ref class MouseDragEvent : Flood::MouseEvent
    {
    public:
        MouseDragEvent(::MouseDragEvent* native);
        MouseDragEvent(System::IntPtr native);
        MouseDragEvent();
        property short Dx
        {
            short get();
            void set(short);
        }
        property short Dy
        {
            short get();
            void set(short);
        }
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
        property Flood::MouseInfo^ Info
        {
            Flood::MouseInfo^ get();
            void set(Flood::MouseInfo^);
        }
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };

    public ref class MouseButtonEvent : Flood::MouseEvent
    {
    public:
        MouseButtonEvent(::MouseButtonEvent* native);
        MouseButtonEvent(System::IntPtr native);
        MouseButtonEvent(Flood::MouseEventType eventType);
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
        property Flood::MouseButton Button
        {
            Flood::MouseButton get();
            void set(Flood::MouseButton);
        }
        bool IsLeftButton();
        bool IsRightButton();
        bool IsMiddleButton();
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };

    public ref class MouseWheelEvent : Flood::MouseEvent
    {
    public:
        MouseWheelEvent(::MouseWheelEvent* native);
        MouseWheelEvent(System::IntPtr native);
        MouseWheelEvent();
        property short Delta
        {
            short get();
            void set(short);
        }
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
