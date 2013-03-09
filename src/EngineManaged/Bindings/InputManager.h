/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Input/InputManager.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class InputManager;
    ref class InputDevice;
    ref class InputEvent;
    ref class Keyboard;
    ref class Mouse;
    ref class Joystick;
    ref class Window;

    /// <summary>
    /// Manages a set of input devices.
    /// </summary>
    public ref class InputManager
    {
    public:
        property ::InputManager* NativePtr;

        InputManager(::InputManager* native);
        InputManager(System::IntPtr native);
        InputManager();
        property Flood::Window^ Window;
        void CreateDefaultDevices();
        Flood::Window^ GetWindow();
        void SetWindow(Flood::Window^ window);
    };

    public ref class FloodInputManager
    {
    public:
        static void InputInitialize();
        static void InputDeinitialize();
        static Flood::InputManager^ GetInputManager();
    };
}
