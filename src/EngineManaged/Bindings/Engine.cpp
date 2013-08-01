/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine.h"
#include "InputManager.h"
#include "Log.h"
#include "ResourceManager.h"
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

Flood::Engine::Engine()
{
    NativePtr = new ::Engine();
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
    return Instance == safe_cast<ICppInstance^>(object)->Instance;
}

int Flood::Engine::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Engine::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Engine::Instance::set(System::IntPtr object)
{
    NativePtr = (::Engine*)object.ToPointer();
}

Flood::ResourceManager^ Flood::Engine::ResourceManager::get()
{
    auto ret = ((::Engine*)NativePtr)->getResourceManager();
    return gcnew Flood::ResourceManager((::ResourceManager*)ret);
}

Flood::InputManager^ Flood::Engine::InputManager::get()
{
    auto ret = ((::Engine*)NativePtr)->getInputManager();
    return gcnew Flood::InputManager((::InputManager*)ret);
}

void Flood::Engine::InputManager::set(Flood::InputManager^ value)
{
    auto v = value;
    auto arg0 = (::InputManager*)v->NativePtr;
    ((::Engine*)NativePtr)->setInputManager(arg0);
}

Flood::WindowManager^ Flood::Engine::WindowManager::get()
{
    auto ret = ((::Engine*)NativePtr)->getWindowManager();
    return gcnew Flood::WindowManager((::WindowManager*)ret);
}

void Flood::Engine::WindowManager::set(Flood::WindowManager^ value)
{
    auto v = value;
    auto arg0 = (::WindowManager*)v->NativePtr;
    ((::Engine*)NativePtr)->setWindowManager(arg0);
}

Flood::Log^ Flood::Engine::Logger::get()
{
    auto ret = ((::Engine*)NativePtr)->getLogger();
    return gcnew Flood::Log((::Log*)ret);
}

Flood::Engine^ Flood::FloodEngine::GetEngine()
{
    auto ret = ::GetEngine();
    return gcnew Flood::Engine((::Engine*)ret);
}

