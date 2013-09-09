/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Engine.h>

namespace Flood
{
    ref class AudioDevice;
    ref class Engine;
    ref class InputManager;
    ref class Log;
    ref class PageManager;
    ref class PhysicsManager;
    ref class PlatformManager;
    ref class RenderDevice;
    ref class ResourceManager;
    ref class ScriptManager;
    ref class Stream;
    ref class Subsystem;
    ref class WindowManager;

    /// <summary>
    /// Main engine class. This is a utility class that instantiates all the other
    /// engine classes and provides suitable methods to get/set them. It is also
    /// responsible to set the default logger for all of the engine logging needs.
    /// If you need extra flexibility in your app, you can create a class like this
    /// one that instantiates everything and sets it all up.
    /// </summary>
    public ref class Engine : ICppInstance
    {
    public:

        property ::Engine* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Engine(::Engine* native);
        Engine(System::IntPtr native);
        Engine(Flood::PlatformManager^ platform);

        property Flood::PlatformManager^ PlatformManager
        {
            Flood::PlatformManager^ get();
            void set(Flood::PlatformManager^);
        }
        property Flood::InputManager^ InputManager
        {
            Flood::InputManager^ get();
            void set(Flood::InputManager^);
        }
        property Flood::WindowManager^ WindowManager
        {
            Flood::WindowManager^ get();
            void set(Flood::WindowManager^);
        }
        property Flood::RenderDevice^ RenderDevice
        {
            Flood::RenderDevice^ get();
        }
        property Flood::ResourceManager^ ResourceManager
        {
            Flood::ResourceManager^ get();
        }
        property Flood::Log^ Logger
        {
            Flood::Log^ get();
        }
        void Init();

        void Update();

        void SetupLogger();

        void StepFrame();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };

    public ref class FloodEngine
    {
    public:
        static Flood::Engine^ GetEngine();
    };
}
