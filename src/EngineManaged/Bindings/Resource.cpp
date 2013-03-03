/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Resource.h"
#include "Reflection.h"
#include "Memory.h"
#include "Serialization.h"
#include "ResourceLoader.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Resource::Resource(::Resource* native)
{
    NativePtr = native;
}

Flood::Resource::Resource(System::IntPtr native)
{
    auto __native = (::Resource*)native.ToPointer();
    NativePtr = __native;
}

Flood::Class^ Flood::Resource::GetType()
{
    auto ret = ((::Resource*)NativePtr)->getType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::Class^ Flood::Resource::GetStaticType()
{
    auto ret = ((::Resource*)NativePtr)->getStaticType();
    return gcnew Flood::Class((::Class*)ret);
}

System::String^ Flood::Resource::GetPath()
{
    auto ret = &((::Resource*)NativePtr)->getPath();
    return marshalString<E_UTF8>(*ret);
}

void Flood::Resource::SetPath(System::String^ v)
{
    auto arg0 = marshalString<E_UTF8>(v);
    ((::Resource*)NativePtr)->setPath(arg0);
}

Flood::ResourceStatus Flood::Resource::GetStatus()
{
    auto ret = ((::Resource*)NativePtr)->getStatus();
    return (Flood::ResourceStatus)ret;
}

void Flood::Resource::SetStatus(Flood::ResourceStatus v)
{
    auto arg0 = (::ResourceStatus)v;
    ((::Resource*)NativePtr)->setStatus(arg0);
}

bool Flood::Resource::IsLoaded()
{
    auto ret = ((::Resource*)NativePtr)->isLoaded();
    return ret;
}

Flood::ResourceGroup Flood::Resource::GetResourceGroup()
{
    auto ret = ((::Resource*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

void Flood::Resource::HandleDestroy(unsigned int id)
{
    auto arg0 = (HandleId)(uint32)id;
    ::ResourceHandleDestroy(arg0);
}

uint Flood::Resource::HandleCreate()
{
    auto arg0 = (::Resource*)NativePtr;
    auto ret = ::ResourceHandleCreate(arg0);
    return ret.id;
}

