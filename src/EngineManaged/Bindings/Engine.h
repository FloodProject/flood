/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Engine.h>

namespace Flood
{
    ref class Engine;
    ref class Scene;
    ref class Subsystem;
    ref class RenderDevice;
    ref class AudioDevice;
    ref class PhysicsManager;
    ref class PageManager;
    ref class InputManager;
    ref class ScriptManager;
    ref class TaskPool;
    ref class Log;
    ref class Stream;
    ref class ResourceManager;
    ref class RefPtr;










    /// <summary>
    /// Main engine class. This is a utility class that instantiates all the
    /// other engine classes and provides suitable methods to get/set them. It
    /// is also responsible to set the default logger for all of the engine
    /// logging needs. If you need extra flexibility in your app, you can create
    /// a class like this one that instantiates everything and sets it all up.
    /// </summary>
    public ref class Engine
    {
        property ::Engine* NativePtr;

    public:
        Engine(::Engine* native);
        Engine();
        void Init();
        void Update();
        void AddSubsystem(Flood::Subsystem^ subsystem);
        void SetupLogger();
        Flood::RenderDevice^ GetRenderDevice();
        Flood::AudioDevice^ GetAudioDevice();
        Flood::TaskPool^ GetTaskPool();
        Flood::ScriptManager^ GetScriptManager();
        Flood::ResourceManager^ GetResourceManager();
        Flood::PhysicsManager^ GetPhysicsManager();
        void SetPhysicsManager(Flood::PhysicsManager^ v);
        Flood::InputManager^ GetInputManager();
        void SetInputManager(Flood::InputManager^ v);
        Flood::Log^ GetLogger();
        void StepFrame();
    };

    public ref class FloodEngine
    {
    public:
        static Flood::Engine^ GetEngine ();
    };
}

