/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Input/Device.h>

namespace Flood
{
    enum struct InputDeviceType;
    ref class InputDevice;
    ref class InputEvent;
}

namespace Flood
{
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
    public ref class InputEvent : ICppInstance
    {
    public:

        property ::InputEvent* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        InputEvent(::InputEvent* native);
        InputEvent(System::IntPtr native);
        InputEvent(Flood::InputDeviceType _0);

        property Flood::InputDeviceType DeviceType
        {
            Flood::InputDeviceType get();
            void set(Flood::InputDeviceType);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    /// <summary>
    /// Input device.
    /// </summary>
    public ref class InputDevice : ICppInstance
    {
    public:

        property ::InputDevice* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        InputDevice(::InputDevice* native);
        InputDevice(System::IntPtr native);
        InputDevice();

        property Flood::InputDeviceType Type
        {
            Flood::InputDeviceType get();
        }

        virtual void ProcessEvent(Flood::InputEvent^ event);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
