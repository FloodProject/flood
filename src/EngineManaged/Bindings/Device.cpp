/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Device.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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

bool Flood::InputEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<InputEvent^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::InputEvent::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::InputEvent::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::InputEvent::Instance::set(System::IntPtr object)
{
    NativePtr = (::InputEvent*)object.ToPointer();
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

bool Flood::InputDevice::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<InputDevice^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::InputDevice::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::InputDevice::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::InputDevice::Instance::set(System::IntPtr object)
{
    NativePtr = (::InputDevice*)object.ToPointer();
}

Flood::InputDeviceType Flood::InputDevice::Type::get()
{
    auto __ret = ((::InputDevice*)NativePtr)->getType();
    return (Flood::InputDeviceType)__ret;
}

