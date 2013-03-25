/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "InputManager.h"
#include "Device.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ResourceHandle.h"
#include "Window.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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

System::Collections::Generic::List<Flood::InputDevice^>^ Flood::InputManager::GetDevices()
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

Flood::Keyboard^ Flood::InputManager::GetKeyboard()
{
    auto ret = ((::InputManager*)NativePtr)->getKeyboard();
    return gcnew Flood::Keyboard((::Keyboard*)ret);
}

Flood::Mouse^ Flood::InputManager::GetMouse()
{
    auto ret = ((::InputManager*)NativePtr)->getMouse();
    return gcnew Flood::Mouse((::Mouse*)ret);
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

Flood::Window^ Flood::InputManager::GetWindow()
{
    auto ret = ((::InputManager*)NativePtr)->getWindow();
    return gcnew Flood::Window((::Window*)ret);
}

void Flood::InputManager::SetWindow(Flood::Window^ window)
{
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

