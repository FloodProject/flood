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
    ref class RenderDevice;
    ref class ResourceManager;
    ref class Scene;
    ref class ScriptManager;
    ref class Subsystem;
    ref class WindowManager;

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
        Engine();
        property Flood::ResourceManager^ ResourceManager
        {
            Flood::ResourceManager^ get();
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
