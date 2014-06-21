/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Mouse.h"
#include "Device.h"
#include "MouseEvents.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::MouseInfo::MouseInfo(::MouseInfo* native)
{
    NativePtr = native;
}

Flood::MouseInfo::MouseInfo(System::IntPtr native)
{
    auto __native = (::MouseInfo*)native.ToPointer();
    NativePtr = __native;
}

Flood::MouseInfo::MouseInfo()
{
    NativePtr = new ::MouseInfo();
}

bool Flood::MouseInfo::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<MouseInfo^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::MouseInfo::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::MouseInfo::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::MouseInfo::__Instance::set(System::IntPtr object)
{
    NativePtr = (::MouseInfo*)object.ToPointer();
}

short Flood::MouseInfo::X::get()
{
    return ((::MouseInfo*)NativePtr)->x;
}

void Flood::MouseInfo::X::set(short value)
{
    ((::MouseInfo*)NativePtr)->x = value;
}

short Flood::MouseInfo::Y::get()
{
    return ((::MouseInfo*)NativePtr)->y;
}

void Flood::MouseInfo::Y::set(short value)
{
    ((::MouseInfo*)NativePtr)->y = value;
}

bool Flood::MouseInfo::InsideWindow::get()
{
    return ((::MouseInfo*)NativePtr)->insideWindow;
}

void Flood::MouseInfo::InsideWindow::set(bool value)
{
    ((::MouseInfo*)NativePtr)->insideWindow = value;
}

bool Flood::MouseInfo::LeftButton::get()
{
    return ((::MouseInfo*)NativePtr)->leftButton;
}

void Flood::MouseInfo::LeftButton::set(bool value)
{
    ((::MouseInfo*)NativePtr)->leftButton = value;
}

bool Flood::MouseInfo::RightButton::get()
{
    return ((::MouseInfo*)NativePtr)->rightButton;
}

void Flood::MouseInfo::RightButton::set(bool value)
{
    ((::MouseInfo*)NativePtr)->rightButton = value;
}

bool Flood::MouseInfo::MiddleButton::get()
{
    return ((::MouseInfo*)NativePtr)->middleButton;
}

void Flood::MouseInfo::MiddleButton::set(bool value)
{
    ((::MouseInfo*)NativePtr)->middleButton = value;
}

bool Flood::MouseInfo::Aux1Button::get()
{
    return ((::MouseInfo*)NativePtr)->aux1Button;
}

void Flood::MouseInfo::Aux1Button::set(bool value)
{
    ((::MouseInfo*)NativePtr)->aux1Button = value;
}

bool Flood::MouseInfo::Aux2Button::get()
{
    return ((::MouseInfo*)NativePtr)->aux2Button;
}

void Flood::MouseInfo::Aux2Button::set(bool value)
{
    ((::MouseInfo*)NativePtr)->aux2Button = value;
}

Flood::Mouse::Mouse(::Mouse* native)
    : Flood::InputDevice((::InputDevice*)native)
{
}

Flood::Mouse::Mouse(System::IntPtr native)
    : Flood::InputDevice(native)
{
    auto __native = (::Mouse*)native.ToPointer();
}

bool Flood::Mouse::IsButtonPressed(Flood::MouseButton button)
{
    auto arg0 = (::MouseButton)button;
    auto __ret = ((::Mouse*)NativePtr)->isButtonPressed(arg0);
    return __ret;
}

Flood::Mouse::Mouse()
    : Flood::InputDevice((::InputDevice*)nullptr)
{
    NativePtr = new ::Mouse();
}

bool Flood::Mouse::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Mouse^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Mouse::GetHashCode()
{
    return (int)NativePtr;
}

Flood::InputDeviceType Flood::Mouse::Type::get()
{
    auto __ret = ((::Mouse*)NativePtr)->getType();
    return (Flood::InputDeviceType)__ret;
}

Flood::MouseInfo^ Flood::Mouse::MouseInfo::get()
{
    auto &__ret = ((::Mouse*)NativePtr)->getMouseInfo();
    return (Flood::MouseInfo^)((&__ret == nullptr) ? nullptr : gcnew Flood::MouseInfo((::MouseInfo*)&__ret));
}

void Flood::Mouse::MouseMove::add(System::Action<Flood::MouseMoveEvent^>^ evt)
{
    if (!_MouseMoveDelegateInstance)
    {
        _MouseMoveDelegateInstance = gcnew _MouseMoveDelegate(this, &Flood::Mouse::_MouseMoveRaise);
        auto _fptr = (void (*)(const ::MouseMoveEvent&))Marshal::GetFunctionPointerForDelegate(_MouseMoveDelegateInstance).ToPointer();
        ((::Mouse*)NativePtr)->onMouseMove.Connect(_fptr);
    }
    _MouseMove = static_cast<System::Action<Flood::MouseMoveEvent^>^>(System::Delegate::Combine(_MouseMove, evt));
}

void Flood::Mouse::MouseMove::remove(System::Action<Flood::MouseMoveEvent^>^ evt)
{
    _MouseMove = static_cast<System::Action<Flood::MouseMoveEvent^>^>(System::Delegate::Remove(_MouseMove, evt));
}

void Flood::Mouse::MouseMove::raise(Flood::MouseMoveEvent^ _0)
{
    _MouseMove(_0);
}

void Flood::Mouse::_MouseMoveRaise(const ::MouseMoveEvent& _0)
{
    MouseMove::raise((&_0 == nullptr) ? nullptr : gcnew Flood::MouseMoveEvent((::MouseMoveEvent*)&_0));
}

void Flood::Mouse::MouseDrag::add(System::Action<Flood::MouseDragEvent^>^ evt)
{
    if (!_MouseDragDelegateInstance)
    {
        _MouseDragDelegateInstance = gcnew _MouseDragDelegate(this, &Flood::Mouse::_MouseDragRaise);
        auto _fptr = (void (*)(const ::MouseDragEvent&))Marshal::GetFunctionPointerForDelegate(_MouseDragDelegateInstance).ToPointer();
        ((::Mouse*)NativePtr)->onMouseDrag.Connect(_fptr);
    }
    _MouseDrag = static_cast<System::Action<Flood::MouseDragEvent^>^>(System::Delegate::Combine(_MouseDrag, evt));
}

void Flood::Mouse::MouseDrag::remove(System::Action<Flood::MouseDragEvent^>^ evt)
{
    _MouseDrag = static_cast<System::Action<Flood::MouseDragEvent^>^>(System::Delegate::Remove(_MouseDrag, evt));
}

void Flood::Mouse::MouseDrag::raise(Flood::MouseDragEvent^ _0)
{
    _MouseDrag(_0);
}

void Flood::Mouse::_MouseDragRaise(const ::MouseDragEvent& _0)
{
    MouseDrag::raise((&_0 == nullptr) ? nullptr : gcnew Flood::MouseDragEvent((::MouseDragEvent*)&_0));
}

void Flood::Mouse::MouseButtonPress::add(System::Action<Flood::MouseButtonEvent^>^ evt)
{
    if (!_MouseButtonPressDelegateInstance)
    {
        _MouseButtonPressDelegateInstance = gcnew _MouseButtonPressDelegate(this, &Flood::Mouse::_MouseButtonPressRaise);
        auto _fptr = (void (*)(const ::MouseButtonEvent&))Marshal::GetFunctionPointerForDelegate(_MouseButtonPressDelegateInstance).ToPointer();
        ((::Mouse*)NativePtr)->onMouseButtonPress.Connect(_fptr);
    }
    _MouseButtonPress = static_cast<System::Action<Flood::MouseButtonEvent^>^>(System::Delegate::Combine(_MouseButtonPress, evt));
}

void Flood::Mouse::MouseButtonPress::remove(System::Action<Flood::MouseButtonEvent^>^ evt)
{
    _MouseButtonPress = static_cast<System::Action<Flood::MouseButtonEvent^>^>(System::Delegate::Remove(_MouseButtonPress, evt));
}

void Flood::Mouse::MouseButtonPress::raise(Flood::MouseButtonEvent^ _0)
{
    _MouseButtonPress(_0);
}

void Flood::Mouse::_MouseButtonPressRaise(const ::MouseButtonEvent& _0)
{
    MouseButtonPress::raise((&_0 == nullptr) ? nullptr : gcnew Flood::MouseButtonEvent((::MouseButtonEvent*)&_0));
}

void Flood::Mouse::MouseButtonRelease::add(System::Action<Flood::MouseButtonEvent^>^ evt)
{
    if (!_MouseButtonReleaseDelegateInstance)
    {
        _MouseButtonReleaseDelegateInstance = gcnew _MouseButtonReleaseDelegate(this, &Flood::Mouse::_MouseButtonReleaseRaise);
        auto _fptr = (void (*)(const ::MouseButtonEvent&))Marshal::GetFunctionPointerForDelegate(_MouseButtonReleaseDelegateInstance).ToPointer();
        ((::Mouse*)NativePtr)->onMouseButtonRelease.Connect(_fptr);
    }
    _MouseButtonRelease = static_cast<System::Action<Flood::MouseButtonEvent^>^>(System::Delegate::Combine(_MouseButtonRelease, evt));
}

void Flood::Mouse::MouseButtonRelease::remove(System::Action<Flood::MouseButtonEvent^>^ evt)
{
    _MouseButtonRelease = static_cast<System::Action<Flood::MouseButtonEvent^>^>(System::Delegate::Remove(_MouseButtonRelease, evt));
}

void Flood::Mouse::MouseButtonRelease::raise(Flood::MouseButtonEvent^ _0)
{
    _MouseButtonRelease(_0);
}

void Flood::Mouse::_MouseButtonReleaseRaise(const ::MouseButtonEvent& _0)
{
    MouseButtonRelease::raise((&_0 == nullptr) ? nullptr : gcnew Flood::MouseButtonEvent((::MouseButtonEvent*)&_0));
}

void Flood::Mouse::MouseWheelMove::add(System::Action<Flood::MouseWheelEvent^>^ evt)
{
    if (!_MouseWheelMoveDelegateInstance)
    {
        _MouseWheelMoveDelegateInstance = gcnew _MouseWheelMoveDelegate(this, &Flood::Mouse::_MouseWheelMoveRaise);
        auto _fptr = (void (*)(const ::MouseWheelEvent&))Marshal::GetFunctionPointerForDelegate(_MouseWheelMoveDelegateInstance).ToPointer();
        ((::Mouse*)NativePtr)->onMouseWheelMove.Connect(_fptr);
    }
    _MouseWheelMove = static_cast<System::Action<Flood::MouseWheelEvent^>^>(System::Delegate::Combine(_MouseWheelMove, evt));
}

void Flood::Mouse::MouseWheelMove::remove(System::Action<Flood::MouseWheelEvent^>^ evt)
{
    _MouseWheelMove = static_cast<System::Action<Flood::MouseWheelEvent^>^>(System::Delegate::Remove(_MouseWheelMove, evt));
}

void Flood::Mouse::MouseWheelMove::raise(Flood::MouseWheelEvent^ _0)
{
    _MouseWheelMove(_0);
}

void Flood::Mouse::_MouseWheelMoveRaise(const ::MouseWheelEvent& _0)
{
    MouseWheelMove::raise((&_0 == nullptr) ? nullptr : gcnew Flood::MouseWheelEvent((::MouseWheelEvent*)&_0));
}

void Flood::Mouse::MouseEnter::add(System::Action^ evt)
{
    if (!_MouseEnterDelegateInstance)
    {
        _MouseEnterDelegateInstance = gcnew _MouseEnterDelegate(this, &Flood::Mouse::_MouseEnterRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_MouseEnterDelegateInstance).ToPointer();
        ((::Mouse*)NativePtr)->onMouseEnter.Connect(_fptr);
    }
    _MouseEnter = static_cast<System::Action^>(System::Delegate::Combine(_MouseEnter, evt));
}

void Flood::Mouse::MouseEnter::remove(System::Action^ evt)
{
    _MouseEnter = static_cast<System::Action^>(System::Delegate::Remove(_MouseEnter, evt));
}

void Flood::Mouse::MouseEnter::raise()
{
    _MouseEnter();
}

void Flood::Mouse::_MouseEnterRaise()
{
    MouseEnter::raise();
}

void Flood::Mouse::MouseExit::add(System::Action^ evt)
{
    if (!_MouseExitDelegateInstance)
    {
        _MouseExitDelegateInstance = gcnew _MouseExitDelegate(this, &Flood::Mouse::_MouseExitRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_MouseExitDelegateInstance).ToPointer();
        ((::Mouse*)NativePtr)->onMouseExit.Connect(_fptr);
    }
    _MouseExit = static_cast<System::Action^>(System::Delegate::Combine(_MouseExit, evt));
}

void Flood::Mouse::MouseExit::remove(System::Action^ evt)
{
    _MouseExit = static_cast<System::Action^>(System::Delegate::Remove(_MouseExit, evt));
}

void Flood::Mouse::MouseExit::raise()
{
    _MouseExit();
}

void Flood::Mouse::_MouseExitRaise()
{
    MouseExit::raise();
}

