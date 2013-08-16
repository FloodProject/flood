/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "InputManager.h"
#include "Device.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Window.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::InputManager::InputManager(::InputManager* native)
{
    NativePtr = native;
}

Flood::InputManager::InputManager(System::IntPtr native)
{
    auto __native = (::InputManager*)native.ToPointer();
    NativePtr = __native;
}

Flood::InputManager::InputManager()
{
    NativePtr = new ::InputManager();
}

void Flood::InputManager::AddDevice(Flood::InputDevice^ device)
{
    auto arg0 = (::InputDevice*)device->NativePtr;
    ((::InputManager*)NativePtr)->addDevice(arg0);
}

void Flood::InputManager::ProcessEvent(Flood::InputEvent^ event)
{
    auto &arg0 = *(::InputEvent*)event->NativePtr;
    ((::InputManager*)NativePtr)->processEvent(arg0);
}

void Flood::InputManager::CreateDefaultDevices()
{
    ((::InputManager*)NativePtr)->createDefaultDevices();
}

bool Flood::InputManager::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<InputManager^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::InputManager::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::InputManager::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::InputManager::Instance::set(System::IntPtr object)
{
    NativePtr = (::InputManager*)object.ToPointer();
}

System::Collections::Generic::List<Flood::InputDevice^>^ Flood::InputManager::Devices::get()
{
    auto &ret = ((::InputManager*)NativePtr)->getDevices();
    auto _tmpret = gcnew System::Collections::Generic::List<Flood::InputDevice^>();
    for(auto _element : ret)
    {
        auto _marshalElement = gcnew Flood::InputDevice((::InputDevice*)_element);
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

Flood::Keyboard^ Flood::InputManager::Keyboard::get()
{
    auto ret = ((::InputManager*)NativePtr)->getKeyboard();
    return gcnew Flood::Keyboard((::Keyboard*)ret);
}

Flood::Mouse^ Flood::InputManager::Mouse::get()
{
    auto ret = ((::InputManager*)NativePtr)->getMouse();
    return gcnew Flood::Mouse((::Mouse*)ret);
}

Flood::Window^ Flood::InputManager::Window::get()
{
    auto ret = ((::InputManager*)NativePtr)->getWindow();
    return gcnew Flood::Window((::Window*)ret);
}

void Flood::InputManager::Window::set(Flood::Window^ value)
{
    auto window = value;
    auto arg0 = (::Window*)window->NativePtr;
    ((::InputManager*)NativePtr)->setWindow(arg0);
}

void Flood::FloodInputManager::InputInitialize()
{
    ::InputInitialize();
}

void Flood::FloodInputManager::InputDeinitialize()
{
    ::InputDeinitialize();
}

Flood::InputManager^ Flood::FloodInputManager::GetInputManager()
{
    auto ret = ::GetInputManager();
    return gcnew Flood::InputManager((::InputManager*)ret);
}

