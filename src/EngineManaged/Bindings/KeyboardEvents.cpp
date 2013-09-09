/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "KeyboardEvents.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::KeyEvent::KeyEvent(::KeyEvent* native)
    : Flood::InputEvent((::InputEvent*)native)
{
}

Flood::KeyEvent::KeyEvent(System::IntPtr native)
    : Flood::InputEvent(native)
{
    auto __native = (::KeyEvent*)native.ToPointer();
}

Flood::KeyEvent::KeyEvent(Flood::KeyboardEventType eventType)
    : Flood::InputEvent((::InputEvent*)nullptr)
{
    auto arg0 = (::KeyboardEventType)eventType;
    NativePtr = new ::KeyEvent(arg0);
}

bool Flood::KeyEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<KeyEvent^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::KeyEvent::GetHashCode()
{
    return (int)NativePtr;
}

Flood::KeyboardEventType Flood::KeyEvent::EventType::get()
{
    return (Flood::KeyboardEventType)((::KeyEvent*)NativePtr)->eventType;
}

void Flood::KeyEvent::EventType::set(Flood::KeyboardEventType value)
{
    ((::KeyEvent*)NativePtr)->eventType = (::KeyboardEventType)value;
}

Flood::Keys Flood::KeyEvent::KeyCode::get()
{
    return (Flood::Keys)((::KeyEvent*)NativePtr)->keyCode;
}

void Flood::KeyEvent::KeyCode::set(Flood::Keys value)
{
    ((::KeyEvent*)NativePtr)->keyCode = (::Keys)value;
}

bool Flood::KeyEvent::AltPressed::get()
{
    return ((::KeyEvent*)NativePtr)->altPressed;
}

void Flood::KeyEvent::AltPressed::set(bool value)
{
    ((::KeyEvent*)NativePtr)->altPressed = value;
}

bool Flood::KeyEvent::ShiftPressed::get()
{
    return ((::KeyEvent*)NativePtr)->shiftPressed;
}

void Flood::KeyEvent::ShiftPressed::set(bool value)
{
    ((::KeyEvent*)NativePtr)->shiftPressed = value;
}

bool Flood::KeyEvent::CtrlPressed::get()
{
    return ((::KeyEvent*)NativePtr)->ctrlPressed;
}

void Flood::KeyEvent::CtrlPressed::set(bool value)
{
    ((::KeyEvent*)NativePtr)->ctrlPressed = value;
}

unsigned int Flood::KeyEvent::Unicode::get()
{
    return ((::KeyEvent*)NativePtr)->unicode;
}

void Flood::KeyEvent::Unicode::set(unsigned int value)
{
    ((::KeyEvent*)NativePtr)->unicode = (::uint32)(::uint32_t)value;
}

