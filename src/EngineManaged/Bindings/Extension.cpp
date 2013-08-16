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
    Name = clix::marshalString<clix::E_UTF8>(native->name);
    Description = clix::marshalString<clix::E_UTF8>(native->description);
    Priority = native->priority;
}

Flood::ExtensionMetadata::ExtensionMetadata(System::IntPtr native)
{
    auto __native = (::ExtensionMetadata*)native.ToPointer();
    Name = clix::marshalString<clix::E_UTF8>(__native->name);
    Description = clix::marshalString<clix::E_UTF8>(__native->description);
    Priority = __native->priority;
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
    return Instance == obj->Instance;
}

int Flood::Extension::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::Extension::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Extension::Instance::set(System::IntPtr object)
{
    NativePtr = (::Extension*)object.ToPointer();
}

Flood::ExtensionMetadata Flood::Extension::Metadata::get()
{
    auto ret = ((::Extension*)NativePtr)->getMetadata();
    return Flood::ExtensionMetadata((::ExtensionMetadata*)ret);
}

