/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "ResourceLoader.h"
#include "Extension.h"
#include "Resource.h"
#include "ResourceHandle.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::ResourceLoadOption::ResourceLoadOption(::ResourceLoadOption* native)
{
    Key = native->key;
    Value = native->value;
}

Flood::ResourceLoadOption::ResourceLoadOption(System::IntPtr native)
{
    auto __native = (::ResourceLoadOption*)native.ToPointer();
    Key = __native->key;
    Value = __native->value;
}

Flood::ResourceLoadOptions::ResourceLoadOptions(::ResourceLoadOptions* native)
{
    Name = marshalString<E_UTF8>(native->name);
    Stream = gcnew Flood::Stream((::Stream*)native->stream);
    Resource = gcnew Flood::Resource((::Resource*)native->resource);
    Group = (Flood::ResourceGroup)native->group;
    IsHighPriority = native->isHighPriority;
    SendLoadEvent = native->sendLoadEvent;
    AsynchronousLoad = native->asynchronousLoad;
    KeepStreamOpen = native->keepStreamOpen;
    Option = Flood::ResourceLoadOption((::ResourceLoadOption*)&native->option);
}

Flood::ResourceLoadOptions::ResourceLoadOptions(System::IntPtr native)
{
    auto __native = (::ResourceLoadOptions*)native.ToPointer();
    Name = marshalString<E_UTF8>(__native->name);
    Stream = gcnew Flood::Stream((::Stream*)__native->stream);
    Resource = gcnew Flood::Resource((::Resource*)__native->resource);
    Group = (Flood::ResourceGroup)__native->group;
    IsHighPriority = __native->isHighPriority;
    SendLoadEvent = __native->sendLoadEvent;
    AsynchronousLoad = __native->asynchronousLoad;
    KeepStreamOpen = __native->keepStreamOpen;
    Option = Flood::ResourceLoadOption((::ResourceLoadOption*)&__native->option);
}

void Flood::ResourceLoadOptions::AddOption(int key, int value)
{
    auto this0 = (::ResourceLoadOptions*) 0;
    this0->addOption(key, value);
}

Flood::ResourceStream::ResourceStream(::ResourceStream* native)
{
    NativePtr = native;
}

Flood::ResourceStream::ResourceStream(System::IntPtr native)
{
    auto __native = (::ResourceStream*)native.ToPointer();
    NativePtr = __native;
}

Flood::ResourceStream::ResourceStream()
{
}

int Flood::ResourceStream::Decode(System::IntPtr buffer, unsigned int size)
{
    auto arg0 = (uint8*)buffer.ToPointer();
    auto arg1 = (size_t)size;
    auto ret = ((::ResourceStream*)NativePtr)->decode(arg0, arg1);
    return ret;
}

void Flood::ResourceStream::Reset()
{
    ((::ResourceStream*)NativePtr)->reset();
}

Flood::ResourceLoader::ResourceLoader(::ResourceLoader* native)
    : Extension(native)
{
}

Flood::ResourceLoader::ResourceLoader(System::IntPtr native)
    : Extension(native)
{
    auto __native = (::ResourceLoader*)native.ToPointer();
}

Flood::ResourceLoader::ResourceLoader()
    : Extension(nullptr)
{
}

Flood::ExtensionMetadata Flood::ResourceLoader::GetMetadata()
{
    auto ret = ((::ResourceLoader*)NativePtr)->getMetadata();
    return Flood::ExtensionMetadata((::ExtensionMetadata*)ret);
}

Flood::Resource^ Flood::ResourceLoader::Prepare(Flood::ResourceLoadOptions _161)
{
    auto arg0 = ::ResourceLoadOptions();
    arg0.name = marshalString<E_UTF8>(_161.Name);
    arg0.stream = (::Stream*)_161.Stream->NativePtr;
    arg0.resource = (::Resource*)_161.Resource->NativePtr;
    arg0.group = (::ResourceGroup)_161.Group;
    arg0.isHighPriority = _161.IsHighPriority;
    arg0.sendLoadEvent = _161.SendLoadEvent;
    arg0.asynchronousLoad = _161.AsynchronousLoad;
    arg0.keepStreamOpen = _161.KeepStreamOpen;
    arg0.option = ::ResourceLoadOption();

    auto ret = ((::ResourceLoader*)NativePtr)->prepare(arg0);
    return gcnew Flood::Resource((::Resource*)ret);
}

bool Flood::ResourceLoader::Decode(Flood::ResourceLoadOptions _162)
{
    auto arg0 = ::ResourceLoadOptions();
    arg0.name = marshalString<E_UTF8>(_162.Name);
    arg0.stream = (::Stream*)_162.Stream->NativePtr;
    arg0.resource = (::Resource*)_162.Resource->NativePtr;
    arg0.group = (::ResourceGroup)_162.Group;
    arg0.isHighPriority = _162.IsHighPriority;
    arg0.sendLoadEvent = _162.SendLoadEvent;
    arg0.asynchronousLoad = _162.AsynchronousLoad;
    arg0.keepStreamOpen = _162.KeepStreamOpen;
    arg0.option = ::ResourceLoadOption();

    auto ret = ((::ResourceLoader*)NativePtr)->decode(arg0);
    return ret;
}

System::String^ Flood::ResourceLoader::GetName()
{
    auto ret = ((::ResourceLoader*)NativePtr)->getName();
    return marshalString<E_UTF8>(ret);
}

Flood::ResourceGroup Flood::ResourceLoader::GetResourceGroup()
{
    auto ret = ((::ResourceLoader*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

