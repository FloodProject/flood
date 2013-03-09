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
    ref class Engine;
    ref class Scene;
    ref class Stream;
    ref class Subsystem;
    ref class ResourceManager;
    ref class RenderDevice;
    ref class AudioDevice;
    ref class PhysicsManager;
    ref class PageManager;
    ref class InputManager;
    ref class ScriptManager;
    ref class WindowManager;
    ref class Log;
    ref class RefPtr;

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
        property Flood::Log^ Log;
        property Flood::Stream^ Stream;
        property Flood::InputManager^ InputManager;
        property Flood::ResourceManager^ ResourceManager;
        property Flood::WindowManager^ WindowManager;
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
