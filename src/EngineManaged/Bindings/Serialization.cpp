/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Serialization.h"
#include "Memory.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::ReflectionHandleContext::ReflectionHandleContext(::ReflectionHandleContext* native)
{
    NativePtr = native;
}

Flood::ReflectionHandleContext::ReflectionHandleContext(System::IntPtr native)
{
    auto __native = (::ReflectionHandleContext*)native.ToPointer();
    NativePtr = __native;
}

Flood::ReflectionHandleContext::ReflectionHandleContext()
{
    NativePtr = new ::ReflectionHandleContext();
}

bool Flood::ReflectionHandleContext::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ReflectionHandleContext^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ReflectionHandleContext::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::ReflectionHandleContext::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::ReflectionHandleContext::Instance::set(System::IntPtr object)
{
    NativePtr = (::ReflectionHandleContext*)object.ToPointer();
}
