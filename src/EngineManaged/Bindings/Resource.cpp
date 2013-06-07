/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Resource.h"
#include "ResourceHandle.h"
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

System::String^ Flood::Resource::GetPath()
{
    auto &ret = ((::Resource*)NativePtr)->getPath();
    return clix::marshalString<clix::E_UTF8>(ret);
}

void Flood::Resource::SetPath(System::String^ v)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(v);
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

Flood::ResourceHandle<Flood::Resource^> Flood::Resource::HandleCreate()
{
    auto arg0 = (::Resource*)NativePtr;
    auto ret = ::ResourceHandleCreate(arg0);
    return Flood::ResourceHandle<Flood::Resource^>(ret.id);
}

void Flood::Resource::HandleDestroy(unsigned int id)
{
    auto arg0 = (HandleId)(uint32)id;
    ::ResourceHandleDestroy(arg0);
}

System::String^ Flood::Resource::Path::get()
{
    return clix::marshalString<clix::E_UTF8>(((::Resource*)NativePtr)->path);
}

void Flood::Resource::Path::set(System::String^ value)
{
    ((::Resource*)NativePtr)->path = clix::marshalString<clix::E_UTF8>(value);
}

Flood::ResourceStatus Flood::Resource::Status::get()
{
    return (Flood::ResourceStatus)((::Resource*)NativePtr)->status;
}

void Flood::Resource::Status::set(Flood::ResourceStatus value)
{
    ((::Resource*)NativePtr)->status = (::ResourceStatus)value;
}

