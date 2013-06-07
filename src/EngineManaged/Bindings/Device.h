/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/Input/Device.h>
#include "ResourceHandle.h"

namespace Flood
{
    enum struct InputDeviceType;
    ref class InputDevice;
    ref class InputEvent;

    /// <summary>
    /// Different types of input devices.
    /// </summary>
    public enum struct InputDeviceType
    {
        Keyboard = 0,
        Mouse = 1,
        Joystick = 2
    };

    /// <summary>
    /// Base struct for input events.
    /// </summary>
    public ref class InputEvent
    {
    public:
        property ::InputEvent* NativePtr;

        InputEvent(::InputEvent* native);
        InputEvent(System::IntPtr native);
        InputEvent(Flood::InputDeviceType _0);
        property Flood::InputDeviceType DeviceType
        {
            Flood::InputDeviceType get();
            void set(Flood::InputDeviceType);
        }
    };

    /// <summary>
    /// Input device.
    /// </summary>
    public ref class InputDevice
    {
    public:
        property ::InputDevice* NativePtr;

        InputDevice(::InputDevice* native);
        InputDevice(System::IntPtr native);
        InputDevice();
        void ProcessEvent(Flood::InputEvent^ event);
        Flood::InputDeviceType GetType();
    };
}
