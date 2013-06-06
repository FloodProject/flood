/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Engine.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class AudioDevice;
    ref class Engine;
    ref class InputManager;
    ref class Log;
    ref class PageManager;
    ref class PhysicsManager;
    ref class RenderDevice;
    ref class ResourceManager;
    ref class Scene;
    ref class ScriptManager;
    ref class Subsystem;
    ref class WindowManager;

    /// <summary>
    /// Main engine class. This is a utility class that instantiates all the other
    /// engine classes and provides suitable methods to get/set them. It is also
    /// responsible to set the default logger for all of the engine logging needs.
    /// If you need extra flexibility in your app, you can create a class like this
    /// one that instantiates everything and sets it all up.
    /// </summary>
    public ref class Engine
    {
    public:
        property ::Engine* NativePtr;

        Engine(::Engine* native);
        Engine(System::IntPtr native);
        Engine();
        void Init();
        void Update();
        void SetupLogger();
        Flood::ResourceManager^ GetResourceManager();
        Flood::InputManager^ GetInputManager();
        void SetInputManager(Flood::InputManager^ v);
        Flood::WindowManager^ GetWindowManager();
        void SetWindowManager(Flood::WindowManager^ v);
        Flood::Log^ GetLogger();
        void StepFrame();
    };

    public ref class FloodEngine
    {
    public:
        static Flood::Engine^ GetEngine();
    };
}
