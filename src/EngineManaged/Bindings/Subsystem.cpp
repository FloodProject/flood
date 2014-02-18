/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Subsystem.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Subsystem::Subsystem(::Subsystem* native)
{
    NativePtr = native;
}

Flood::Subsystem::Subsystem(System::IntPtr native)
{
    auto __native = (::Subsystem*)native.ToPointer();
    NativePtr = __native;
}

void Flood::Subsystem::Update()
{
    ((::Subsystem*)NativePtr)->update();
}

bool Flood::Subsystem::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Subsystem^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Subsystem::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Subsystem::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Subsystem::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Subsystem*)object.ToPointer();
}
