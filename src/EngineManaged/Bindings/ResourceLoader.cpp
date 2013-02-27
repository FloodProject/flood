/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "ResourceLoader.h"
#include "Reflection.h"
#include "Memory.h"
#include "Serialization.h"
#include "Stream.h"
#include "Resource.h"
#include "Extension.h"

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
    auto ret = NativePtr->decode(arg0, arg1);
    return ret;
}

void Flood::ResourceStream::Reset()
{
    NativePtr->reset();
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

Flood::Class^ Flood::ResourceLoader::GetType()
{
    auto ret = NativePtr->getType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::Class^ Flood::ResourceLoader::GetStaticType()
{
    auto ret = NativePtr->getStaticType();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::ExtensionMetadata Flood::ResourceLoader::GetMetadata()
{
    auto ret = NativePtr->getMetadata();
    return Flood::ExtensionMetadata((::ExtensionMetadata*)ret);
}

Flood::Resource^ Flood::ResourceLoader::Prepare(Flood::ResourceLoadOptions _173)
{
    auto _arg0 = (::ResourceLoadOptions*)&_173;
    auto arg0 = *_arg0;
    auto ret = NativePtr->prepare(arg0);
    return gcnew Flood::Resource((::Resource*)ret);
}

bool Flood::ResourceLoader::Decode(Flood::ResourceLoadOptions _174)
{
    auto _arg0 = (::ResourceLoadOptions*)&_174;
    auto arg0 = *_arg0;
    auto ret = NativePtr->decode(arg0);
    return ret;
}

System::String^ Flood::ResourceLoader::GetName()
{
    auto ret = NativePtr->getName();
    return marshalString<E_UTF8>(ret);
}

Flood::Class^ Flood::ResourceLoader::GetResourceClass()
{
    auto ret = NativePtr->getResourceClass();
    return gcnew Flood::Class((::Class*)ret);
}

Flood::ResourceGroup Flood::ResourceLoader::GetResourceGroup()
{
    auto ret = NativePtr->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

