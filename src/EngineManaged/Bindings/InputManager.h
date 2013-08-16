/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Input/InputManager.h>

namespace Flood
{
    ref class InputDevice;
    ref class InputEvent;
    ref class InputManager;
    ref class Joystick;
    ref class Keyboard;
    ref class Mouse;
    ref class Window;

    /// <summary>
    /// Manages a set of input devices.
    /// </summary>
    public ref class InputManager : ICppInstance
    {
    public:

        property ::InputManager* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        InputManager(::InputManager* native);
        InputManager(System::IntPtr native);
        InputManager();

        property System::Collections::Generic::List<Flood::InputDevice^>^ Devices
        {
            System::Collections::Generic::List<Flood::InputDevice^>^ get();
        }
        property Flood::Keyboard^ Keyboard
        {
            Flood::Keyboard^ get();
        }
        property Flood::Mouse^ Mouse
        {
            Flood::Mouse^ get();
        }
        property Flood::Window^ Window
        {
            Flood::Window^ get();
            void set(Flood::Window^);
        }
        void AddDevice(Flood::InputDevice^ device);

        void ProcessEvent(Flood::InputEvent^ event);

        void CreateDefaultDevices();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };

    public ref class FloodInputManager
    {
    public:
        static void InputInitialize();
        static void InputDeinitialize();
        static Flood::InputManager^ GetInputManager();
    };
}
