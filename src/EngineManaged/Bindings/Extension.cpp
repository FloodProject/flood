/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Extension.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::ExtensionMetadata::ExtensionMetadata(::ExtensionMetadata* native)
{
    __Name = clix::marshalString<clix::E_UTF8>(native->name);
    __Description = clix::marshalString<clix::E_UTF8>(native->description);
    __Priority = native->priority;
}

Flood::ExtensionMetadata::ExtensionMetadata(System::IntPtr native)
{
    auto __native = (::ExtensionMetadata*)native.ToPointer();
    __Name = clix::marshalString<clix::E_UTF8>(__native->name);
    __Description = clix::marshalString<clix::E_UTF8>(__native->description);
    __Priority = __native->priority;
}

System::String^ Flood::ExtensionMetadata::Name::get()
{
    return __Name;
}

void Flood::ExtensionMetadata::Name::set(System::String^ value)
{
    __Name = value;
}

System::String^ Flood::ExtensionMetadata::Description::get()
{
    return __Description;
}

void Flood::ExtensionMetadata::Description::set(System::String^ value)
{
    __Description = value;
}

int Flood::ExtensionMetadata::Priority::get()
{
    return __Priority;
}

void Flood::ExtensionMetadata::Priority::set(int value)
{
    __Priority = value;
}

Flood::Extension::Extension(::Extension* native)
{
    NativePtr = native;
}

Flood::Extension::Extension(System::IntPtr native)
{
    auto __native = (::Extension*)native.ToPointer();
    NativePtr = __native;
}

Flood::Extension::Extension()
{
}

void Flood::Extension::OnInit()
{
    ((::Extension*)NativePtr)->onInit();
}

void Flood::Extension::OnCleanup()
{
    ((::Extension*)NativePtr)->onCleanup();
}

bool Flood::Extension::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Extension^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Extension::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Extension::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Extension::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Extension*)object.ToPointer();
}

Flood::ExtensionMetadata Flood::Extension::Metadata::get()
{
    auto __ret = ((::Extension*)NativePtr)->getMetadata();
    return Flood::ExtensionMetadata((::ExtensionMetadata*)__ret);
}

