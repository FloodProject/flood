/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Device.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::InputEvent::InputEvent(::InputEvent* native)
{
    NativePtr = native;
}

Flood::InputEvent::InputEvent(System::IntPtr native)
{
    auto __native = (::InputEvent*)native.ToPointer();
    NativePtr = __native;
}

Flood::InputEvent::InputEvent(Flood::InputDeviceType _0)
{
    auto arg0 = (::InputDeviceType)_0;
    NativePtr = new ::InputEvent(arg0);
}

Flood::InputDeviceType Flood::InputEvent::DeviceType::get()
{
    return (Flood::InputDeviceType)((::InputEvent*)NativePtr)->deviceType;
}

void Flood::InputEvent::DeviceType::set(Flood::InputDeviceType value)
{
    ((::InputEvent*)NativePtr)->deviceType = (::InputDeviceType)value;
}

Flood::InputDevice::InputDevice(::InputDevice* native)
{
    NativePtr = native;
}

Flood::InputDevice::InputDevice(System::IntPtr native)
{
    auto __native = (::InputDevice*)native.ToPointer();
    NativePtr = __native;
}

Flood::InputDevice::InputDevice()
{
}

void Flood::InputDevice::ProcessEvent(Flood::InputEvent^ event)
{
    auto &arg0 = *(::InputEvent*)event->NativePtr;
    ((::InputDevice*)NativePtr)->processEvent(arg0);
}

Flood::InputDeviceType Flood::InputDevice::GetType()
{
    auto ret = ((::InputDevice*)NativePtr)->getType();
    return (Flood::InputDeviceType)ret;
}

