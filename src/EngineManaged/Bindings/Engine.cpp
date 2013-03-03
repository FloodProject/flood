/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Engine.h"
#include "Log.h"
#include "Stream.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "WindowManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

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

Flood::ResourceManager^ Flood::Engine::GetResourceManager()
{
    auto ret = ((::Engine*)NativePtr)->getResourceManager();
    return gcnew Flood::ResourceManager((::ResourceManager*)ret);
}

Flood::InputManager^ Flood::Engine::GetInputManager()
{
    auto ret = ((::Engine*)NativePtr)->getInputManager();
    return gcnew Flood::InputManager((::InputManager*)ret);
}

void Flood::Engine::SetInputManager(Flood::InputManager^ v)
{
    auto arg0 = (::InputManager*)v->NativePtr;
    ((::Engine*)NativePtr)->setInputManager(arg0);
}

Flood::WindowManager^ Flood::Engine::GetWindowManager()
{
    auto ret = ((::Engine*)NativePtr)->getWindowManager();
    return gcnew Flood::WindowManager((::WindowManager*)ret);
}

void Flood::Engine::SetWindowManager(Flood::WindowManager^ v)
{
    auto arg0 = (::WindowManager*)v->NativePtr;
    ((::Engine*)NativePtr)->setWindowManager(arg0);
}

Flood::Log^ Flood::Engine::GetLogger()
{
    auto ret = ((::Engine*)NativePtr)->getLogger();
    return gcnew Flood::Log((::Log*)ret);
}

void Flood::Engine::StepFrame()
{
    ((::Engine*)NativePtr)->stepFrame();
}

Flood::Engine^ Flood::FloodEngine::GetEngine()
{
    auto ret = ::GetEngine();
    return gcnew Flood::Engine((::Engine*)ret);
}

