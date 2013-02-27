/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "InputManager.h"
#include "Window.h"
#include "Vector.h"
#include "RenderTarget.h"

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

void Flood::InputManager::CreateDefaultDevices()
{
    NativePtr->createDefaultDevices();
}

Flood::Window^ Flood::InputManager::GetWindow()
{
    auto ret = NativePtr->getWindow();
    return gcnew Flood::Window((::Window*)ret);
}

void Flood::InputManager::SetWindow(Flood::Window^ window)
{
    auto arg0 = window->NativePtr;
    NativePtr->setWindow(arg0);
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

