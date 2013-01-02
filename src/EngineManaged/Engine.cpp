/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine.h"
#include "Concurrency.h"
#include "Log.h"
#include "Stream.h"
#include "ResourceManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Engine::Engine(::Engine* native)
{
    NativePtr = native;
}

Flood::Engine::Engine()
{
}

void Flood::Engine::Init()
{
}

void Flood::Engine::Update()
{
}

void Flood::Engine::AddSubsystem(Flood::Subsystem^ subsystem)
{
}

void Flood::Engine::SetupLogger()
{
}

Flood::RenderDevice^ Flood::Engine::GetRenderDevice()
{
    return nullptr;
}

Flood::AudioDevice^ Flood::Engine::GetAudioDevice()
{
    return nullptr;
}

Flood::TaskPool^ Flood::Engine::GetTaskPool()
{
    return nullptr;
}

Flood::ScriptManager^ Flood::Engine::GetScriptManager()
{
    return nullptr;
}

Flood::ResourceManager^ Flood::Engine::GetResourceManager()
{
    return nullptr;
}

Flood::PhysicsManager^ Flood::Engine::GetPhysicsManager()
{
    return nullptr;
}

void Flood::Engine::SetPhysicsManager(Flood::PhysicsManager^ v)
{
}

Flood::InputManager^ Flood::Engine::GetInputManager()
{
    return nullptr;
}

void Flood::Engine::SetInputManager(Flood::InputManager^ v)
{
}

Flood::Log^ Flood::Engine::GetLogger()
{
    return nullptr;
}

void Flood::Engine::StepFrame()
{
}

Flood::Engine^ Flood::FloodEngine::GetEngine()
{
    return nullptr;
}


