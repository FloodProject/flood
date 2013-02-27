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
    NativePtr->init();
}

void Flood::Engine::Update()
{
    NativePtr->update();
}

void Flood::Engine::SetupLogger()
{
    NativePtr->setupLogger();
}

Flood::ResourceManager^ Flood::Engine::GetResourceManager()
{
    auto ret = NativePtr->getResourceManager();
    return gcnew Flood::ResourceManager((::ResourceManager*)ret);
}

Flood::InputManager^ Flood::Engine::GetInputManager()
{
    auto ret = NativePtr->getInputManager();
    return gcnew Flood::InputManager((::InputManager*)ret);
}

void Flood::Engine::SetInputManager(Flood::InputManager^ v)
{
    auto arg0 = v->NativePtr;
    NativePtr->setInputManager(arg0);
}

Flood::Log^ Flood::Engine::GetLogger()
{
    auto ret = NativePtr->getLogger();
    return gcnew Flood::Log((::Log*)ret);
}

void Flood::Engine::StepFrame()
{
    NativePtr->stepFrame();
}

Flood::Engine^ Flood::FloodEngine::GetEngine()
{
    auto ret = ::GetEngine();
    return gcnew Flood::Engine((::Engine*)ret);
}

