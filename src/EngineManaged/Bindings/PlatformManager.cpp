/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "PlatformManager.h"
#include "InputManager.h"
#include "WindowManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::PlatformManager::PlatformManager(::PlatformManager* native)
    : Flood::Subsystem((::Subsystem*)native)
{
}

Flood::PlatformManager::PlatformManager(System::IntPtr native)
    : Flood::Subsystem(native)
{
    auto __native = (::PlatformManager*)native.ToPointer();
}

void Flood::PlatformManager::Init()
{
    ((::PlatformManager*)NativePtr)->init();
}

void Flood::PlatformManager::Update()
{
    ((::PlatformManager*)NativePtr)->update();
}

Flood::PlatformManager::PlatformManager()
    : Flood::Subsystem((::Subsystem*)nullptr)
{
}

bool Flood::PlatformManager::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<PlatformManager^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::PlatformManager::GetHashCode()
{
    return (int)NativePtr;
}

Flood::WindowManager^ Flood::PlatformManager::WindowManager::get()
{
    auto __ret = ((::PlatformManager*)NativePtr)->getWindowManager();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::WindowManager((::WindowManager*)__ret);
}

Flood::InputManager^ Flood::PlatformManager::InputManager::get()
{
    auto __ret = ((::PlatformManager*)NativePtr)->getInputManager();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::InputManager((::InputManager*)__ret);
}

