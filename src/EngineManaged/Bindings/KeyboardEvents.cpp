/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "KeyboardEvents.h"
#include "Device.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::KeyEvent::KeyEvent(::KeyEvent* native)
    : Flood::InputEvent(native)
{
}

Flood::KeyEvent::KeyEvent(System::IntPtr native)
    : Flood::InputEvent(native)
{
    auto __native = (::KeyEvent*)native.ToPointer();
}

Flood::KeyEvent::KeyEvent(Flood::KeyboardEventType eventType)
    : Flood::InputEvent(nullptr)
{
    auto arg0 = (::KeyboardEventType)eventType;
    NativePtr = new ::KeyEvent(arg0);
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
    ((::KeyEvent*)NativePtr)->unicode = (uint32)value;
}

