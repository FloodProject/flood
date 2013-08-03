/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Keyboard.h"
#include "Device.h"
#include "KeyboardEvents.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Keyboard::Keyboard(::Keyboard* native)
    : Flood::InputDevice(native)
{
}

Flood::Keyboard::Keyboard(System::IntPtr native)
    : Flood::InputDevice(native)
{
    auto __native = (::Keyboard*)native.ToPointer();
}

Flood::Keyboard::Keyboard()
    : Flood::InputDevice(nullptr)
{
    NativePtr = new ::Keyboard();
}

bool Flood::Keyboard::IsKeyPressed(Flood::Keys keyCode)
{
    auto arg0 = (::Keys)keyCode;
    auto ret = ((::Keyboard*)NativePtr)->isKeyPressed(arg0);
    return ret;
}

void Flood::Keyboard::ResetKeys()
{
    ((::Keyboard*)NativePtr)->resetKeys();
}

bool Flood::Keyboard::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Keyboard^>(object);
    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Keyboard::GetHashCode()
{
    return (int)NativePtr;
}

System::Collections::Generic::List<bool>^ Flood::Keyboard::KeyState::get()
{
    auto &ret = ((::Keyboard*)NativePtr)->getKeyState();
    auto _tmpret = gcnew System::Collections::Generic::List<bool>();
    for(auto _element : ret)
    {
        auto _marshalElement = _element;
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

void Flood::Keyboard::KeyPress::add(System::Action<Flood::KeyEvent^>^ evt)
{
    if (!_KeyPressDelegateInstance)
    {
        _KeyPressDelegateInstance = gcnew _KeyPressDelegate(this, &Flood::Keyboard::_KeyPressRaise);
        auto _fptr = (void (*)(const ::KeyEvent&))Marshal::GetFunctionPointerForDelegate(_KeyPressDelegateInstance).ToPointer();
        ((::Keyboard*)NativePtr)->onKeyPress.Connect(_fptr);
    }
    _KeyPress = static_cast<System::Action<Flood::KeyEvent^>^>(System::Delegate::Combine(_KeyPress, evt));
}

void Flood::Keyboard::KeyPress::remove(System::Action<Flood::KeyEvent^>^ evt)
{
    _KeyPress = static_cast<System::Action<Flood::KeyEvent^>^>(System::Delegate::Remove(_KeyPress, evt));
}

void Flood::Keyboard::KeyPress::raise(Flood::KeyEvent^ _0)
{
    _KeyPress(_0);
}

void Flood::Keyboard::_KeyPressRaise(const ::KeyEvent& _0)
{
    KeyPress::raise(gcnew Flood::KeyEvent((::KeyEvent*)&_0));
}

void Flood::Keyboard::KeyRelease::add(System::Action<Flood::KeyEvent^>^ evt)
{
    if (!_KeyReleaseDelegateInstance)
    {
        _KeyReleaseDelegateInstance = gcnew _KeyReleaseDelegate(this, &Flood::Keyboard::_KeyReleaseRaise);
        auto _fptr = (void (*)(const ::KeyEvent&))Marshal::GetFunctionPointerForDelegate(_KeyReleaseDelegateInstance).ToPointer();
        ((::Keyboard*)NativePtr)->onKeyRelease.Connect(_fptr);
    }
    _KeyRelease = static_cast<System::Action<Flood::KeyEvent^>^>(System::Delegate::Combine(_KeyRelease, evt));
}

void Flood::Keyboard::KeyRelease::remove(System::Action<Flood::KeyEvent^>^ evt)
{
    _KeyRelease = static_cast<System::Action<Flood::KeyEvent^>^>(System::Delegate::Remove(_KeyRelease, evt));
}

void Flood::Keyboard::KeyRelease::raise(Flood::KeyEvent^ _1)
{
    _KeyRelease(_1);
}

void Flood::Keyboard::_KeyReleaseRaise(const ::KeyEvent& _1)
{
    KeyRelease::raise(gcnew Flood::KeyEvent((::KeyEvent*)&_1));
}

void Flood::Keyboard::KeyText::add(System::Action<Flood::KeyEvent^>^ evt)
{
    if (!_KeyTextDelegateInstance)
    {
        _KeyTextDelegateInstance = gcnew _KeyTextDelegate(this, &Flood::Keyboard::_KeyTextRaise);
        auto _fptr = (void (*)(const ::KeyEvent&))Marshal::GetFunctionPointerForDelegate(_KeyTextDelegateInstance).ToPointer();
        ((::Keyboard*)NativePtr)->onKeyText.Connect(_fptr);
    }
    _KeyText = static_cast<System::Action<Flood::KeyEvent^>^>(System::Delegate::Combine(_KeyText, evt));
}

void Flood::Keyboard::KeyText::remove(System::Action<Flood::KeyEvent^>^ evt)
{
    _KeyText = static_cast<System::Action<Flood::KeyEvent^>^>(System::Delegate::Remove(_KeyText, evt));
}

void Flood::Keyboard::KeyText::raise(Flood::KeyEvent^ _2)
{
    _KeyText(_2);
}

void Flood::Keyboard::_KeyTextRaise(const ::KeyEvent& _2)
{
    KeyText::raise(gcnew Flood::KeyEvent((::KeyEvent*)&_2));
}

