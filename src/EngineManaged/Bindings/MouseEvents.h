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

    /// <summary>
    /// Different types of mouse buttons.
    /// </summary>
    public enum struct MouseButton : unsigned char
    {
        Left = 0,
        Right = 1,
        Middle = 2,
        Mouse4 = 3,
        Mouse5 = 4
    };

    /// <summary>
    /// Different types of mouse events.
    /// </summary>
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

    /// <summary>
    /// Generic mouse event that will be inherited by the more specific mouse event
    /// types. This will be used for communication between the input manager, so it
    /// has private members and then adds friend access to the Mouse class. That
    /// way the private data is hidden to the public.
    /// </summary>
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

    /// <summary>
    /// Mouse event that occurs when the mouse is moved and provides the current
    /// mouse position on the screen in screen-coordinates.
    /// </summary>
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

    /// <summary>
    /// Mouse event that occurs when the mouse is dragged. Use MouseInfo to get
    /// more information about which mouse buttons are pressed.
    /// </summary>
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

    /// <summary>
    /// Mouse event that occurs when a button of the mouse is pressed. It holds the
    /// button that was pressed and also the position of the mouse in the window at
    /// the time of the click.
    /// </summary>
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

    /// <summary>
    /// Mouse events that occurs when the mouse wheel is scrolled. In this case the
    /// extra information is a delta value that specifies the relative amount of
    /// the mouse wheel that was scrolled.
    /// </summary>
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
