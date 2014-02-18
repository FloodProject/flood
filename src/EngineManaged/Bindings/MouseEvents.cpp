/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "MouseEvents.h"
#include "Mouse.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::MouseEvent::MouseEvent(::MouseEvent* native)
    : Flood::InputEvent((::InputEvent*)native)
{
}

Flood::MouseEvent::MouseEvent(System::IntPtr native)
    : Flood::InputEvent(native)
{
    auto __native = (::MouseEvent*)native.ToPointer();
}

Flood::MouseEvent::MouseEvent(Flood::MouseEventType eventType)
    : Flood::InputEvent((::InputEvent*)nullptr)
{
    auto arg0 = (::MouseEventType)eventType;
    NativePtr = new ::MouseEvent(arg0);
}

bool Flood::MouseEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MouseEvent^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MouseEvent::GetHashCode()
{
    return (int)NativePtr;
}

Flood::MouseEventType Flood::MouseEvent::EventType::get()
{
    return (Flood::MouseEventType)((::MouseEvent*)NativePtr)->eventType;
}

void Flood::MouseEvent::EventType::set(Flood::MouseEventType value)
{
    ((::MouseEvent*)NativePtr)->eventType = (::MouseEventType)value;
}

Flood::MouseMoveEvent::MouseMoveEvent(::MouseMoveEvent* native)
    : Flood::MouseEvent((::MouseEvent*)native)
{
}

Flood::MouseMoveEvent::MouseMoveEvent(System::IntPtr native)
    : Flood::MouseEvent(native)
{
    auto __native = (::MouseMoveEvent*)native.ToPointer();
}

Flood::MouseMoveEvent::MouseMoveEvent()
    : Flood::MouseEvent((::MouseEvent*)nullptr)
{
    NativePtr = new ::MouseMoveEvent();
}

bool Flood::MouseMoveEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MouseMoveEvent^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MouseMoveEvent::GetHashCode()
{
    return (int)NativePtr;
}

short Flood::MouseMoveEvent::X::get()
{
    return ((::MouseMoveEvent*)NativePtr)->x;
}

void Flood::MouseMoveEvent::X::set(short value)
{
    ((::MouseMoveEvent*)NativePtr)->x = value;
}

short Flood::MouseMoveEvent::Y::get()
{
    return ((::MouseMoveEvent*)NativePtr)->y;
}

void Flood::MouseMoveEvent::Y::set(short value)
{
    ((::MouseMoveEvent*)NativePtr)->y = value;
}

Flood::MouseDragEvent::MouseDragEvent(::MouseDragEvent* native)
    : Flood::MouseEvent((::MouseEvent*)native)
{
}

Flood::MouseDragEvent::MouseDragEvent(System::IntPtr native)
    : Flood::MouseEvent(native)
{
    auto __native = (::MouseDragEvent*)native.ToPointer();
}

Flood::MouseDragEvent::MouseDragEvent()
    : Flood::MouseEvent((::MouseEvent*)nullptr)
{
    NativePtr = new ::MouseDragEvent();
}

bool Flood::MouseDragEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MouseDragEvent^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MouseDragEvent::GetHashCode()
{
    return (int)NativePtr;
}

short Flood::MouseDragEvent::Dx::get()
{
    return ((::MouseDragEvent*)NativePtr)->dx;
}

void Flood::MouseDragEvent::Dx::set(short value)
{
    ((::MouseDragEvent*)NativePtr)->dx = value;
}

short Flood::MouseDragEvent::Dy::get()
{
    return ((::MouseDragEvent*)NativePtr)->dy;
}

void Flood::MouseDragEvent::Dy::set(short value)
{
    ((::MouseDragEvent*)NativePtr)->dy = value;
}

short Flood::MouseDragEvent::X::get()
{
    return ((::MouseDragEvent*)NativePtr)->x;
}

void Flood::MouseDragEvent::X::set(short value)
{
    ((::MouseDragEvent*)NativePtr)->x = value;
}

short Flood::MouseDragEvent::Y::get()
{
    return ((::MouseDragEvent*)NativePtr)->y;
}

void Flood::MouseDragEvent::Y::set(short value)
{
    ((::MouseDragEvent*)NativePtr)->y = value;
}

Flood::MouseInfo^ Flood::MouseDragEvent::Info::get()
{
    return gcnew Flood::MouseInfo((::MouseInfo*)((::MouseDragEvent*)NativePtr)->info);
}

void Flood::MouseDragEvent::Info::set(Flood::MouseInfo^ value)
{
    ((::MouseDragEvent*)NativePtr)->info = (::MouseInfo*)value->NativePtr;
}

Flood::MouseButtonEvent::MouseButtonEvent(::MouseButtonEvent* native)
    : Flood::MouseEvent((::MouseEvent*)native)
{
}

Flood::MouseButtonEvent::MouseButtonEvent(System::IntPtr native)
    : Flood::MouseEvent(native)
{
    auto __native = (::MouseButtonEvent*)native.ToPointer();
}

Flood::MouseButtonEvent::MouseButtonEvent(Flood::MouseEventType eventType)
    : Flood::MouseEvent((::MouseEvent*)nullptr)
{
    auto arg0 = (::MouseEventType)eventType;
    NativePtr = new ::MouseButtonEvent(arg0);
}

bool Flood::MouseButtonEvent::IsLeftButton()
{
    auto __ret = ((::MouseButtonEvent*)NativePtr)->isLeftButton();
    return __ret;
}

bool Flood::MouseButtonEvent::IsRightButton()
{
    auto __ret = ((::MouseButtonEvent*)NativePtr)->isRightButton();
    return __ret;
}

bool Flood::MouseButtonEvent::IsMiddleButton()
{
    auto __ret = ((::MouseButtonEvent*)NativePtr)->isMiddleButton();
    return __ret;
}

bool Flood::MouseButtonEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MouseButtonEvent^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MouseButtonEvent::GetHashCode()
{
    return (int)NativePtr;
}

short Flood::MouseButtonEvent::X::get()
{
    return ((::MouseButtonEvent*)NativePtr)->x;
}

void Flood::MouseButtonEvent::X::set(short value)
{
    ((::MouseButtonEvent*)NativePtr)->x = value;
}

short Flood::MouseButtonEvent::Y::get()
{
    return ((::MouseButtonEvent*)NativePtr)->y;
}

void Flood::MouseButtonEvent::Y::set(short value)
{
    ((::MouseButtonEvent*)NativePtr)->y = value;
}

Flood::MouseButton Flood::MouseButtonEvent::Button::get()
{
    return (Flood::MouseButton)((::MouseButtonEvent*)NativePtr)->button;
}

void Flood::MouseButtonEvent::Button::set(Flood::MouseButton value)
{
    ((::MouseButtonEvent*)NativePtr)->button = (::MouseButton)value;
}

Flood::MouseWheelEvent::MouseWheelEvent(::MouseWheelEvent* native)
    : Flood::MouseEvent((::MouseEvent*)native)
{
}

Flood::MouseWheelEvent::MouseWheelEvent(System::IntPtr native)
    : Flood::MouseEvent(native)
{
    auto __native = (::MouseWheelEvent*)native.ToPointer();
}

Flood::MouseWheelEvent::MouseWheelEvent()
    : Flood::MouseEvent((::MouseEvent*)nullptr)
{
    NativePtr = new ::MouseWheelEvent();
}

bool Flood::MouseWheelEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MouseWheelEvent^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MouseWheelEvent::GetHashCode()
{
    return (int)NativePtr;
}

short Flood::MouseWheelEvent::Delta::get()
{
    return ((::MouseWheelEvent*)NativePtr)->delta;
}

void Flood::MouseWheelEvent::Delta::set(short value)
{
    ((::MouseWheelEvent*)NativePtr)->delta = value;
}

