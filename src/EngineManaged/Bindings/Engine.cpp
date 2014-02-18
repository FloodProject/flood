/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine.h"
#include "InputManager.h"
#include "Log.h"
#include "PlatformManager.h"
#include "RenderDevice.h"
#include "ResourceManager.h"
#include "Stream.h"
#include "Subsystem.h"
#include "WindowManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Engine::Engine(::Engine* native)
{
    NativePtr = native;
}

Flood::Engine::Engine(System::IntPtr native)
{
    auto __native = (::Engine*)native.ToPointer();
    NativePtr = __native;
}

Flood::Engine::Engine(Flood::PlatformManager^ platform)
{
    auto arg0 = (::PlatformManager*)platform->NativePtr;
    NativePtr = new ::Engine(arg0);
}

void Flood::Engine::Init()
{
    ((::Engine*)NativePtr)->init();
}

void Flood::Engine::Update()
{
    ((::Engine*)NativePtr)->update();
}

void Flood::Engine::SetupLogger()
{
    ((::Engine*)NativePtr)->setupLogger();
}

void Flood::Engine::StepFrame()
{
    ((::Engine*)NativePtr)->stepFrame();
}

bool Flood::Engine::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Engine^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Engine::GetHashCode()
{
    return (int)NativePtr;
}

Flood::Engine^ Flood::Engine::GetEngine()
{
    auto __ret = ::GetEngine();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Engine((::Engine*)__ret);
}

System::IntPtr Flood::Engine::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Engine::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Engine*)object.ToPointer();
}

Flood::PlatformManager^ Flood::Engine::PlatformManager::get()
{
    auto __ret = ((::Engine*)NativePtr)->getPlatformManager();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::PlatformManager((::PlatformManager*)__ret);
}

void Flood::Engine::PlatformManager::set(Flood::PlatformManager^ value)
{
    auto v = value;
    auto arg0 = (::PlatformManager*)v->NativePtr;
    ((::Engine*)NativePtr)->setPlatformManager(arg0);
}

Flood::InputManager^ Flood::Engine::InputManager::get()
{
    auto __ret = ((::Engine*)NativePtr)->getInputManager();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::InputManager((::InputManager*)__ret);
}

void Flood::Engine::InputManager::set(Flood::InputManager^ value)
{
    auto v = value;
    auto arg0 = (::InputManager*)v->NativePtr;
    ((::Engine*)NativePtr)->setInputManager(arg0);
}

Flood::WindowManager^ Flood::Engine::WindowManager::get()
{
    auto __ret = ((::Engine*)NativePtr)->getWindowManager();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::WindowManager((::WindowManager*)__ret);
}

void Flood::Engine::WindowManager::set(Flood::WindowManager^ value)
{
    auto v = value;
    auto arg0 = (::WindowManager*)v->NativePtr;
    ((::Engine*)NativePtr)->setWindowManager(arg0);
}

Flood::RenderDevice^ Flood::Engine::RenderDevice::get()
{
    auto __ret = ((::Engine*)NativePtr)->getRenderDevice();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::RenderDevice((::RenderDevice*)__ret);
}

Flood::ResourceManager^ Flood::Engine::ResourceManager::get()
{
    auto __ret = ((::Engine*)NativePtr)->getResourceManager();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::ResourceManager((::ResourceManager*)__ret);
}

Flood::Log^ Flood::Engine::Logger::get()
{
    auto __ret = ((::Engine*)NativePtr)->getLogger();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Log((::Log*)__ret);
}

