/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "ResourceManager.h"
#include "ResourceLoader.h"
#include "Stream.h"
#include "Memory.h"
#include "Resource.h"
#include "Reflection.h"
#include "Serialization.h"
#include "Extension.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::ResourceEvent::ResourceEvent(::ResourceEvent* native)
{
    Resource = gcnew Flood::Resource((::Resource*)native->resource);
    OldResource = gcnew Flood::Resource((::Resource*)native->oldResource);
    Handle = native->handle.id;
}

Flood::ResourceEvent::ResourceEvent(System::IntPtr native)
{
    auto __native = (::ResourceEvent*)native.ToPointer();
    Resource = gcnew Flood::Resource((::Resource*)__native->resource);
    OldResource = gcnew Flood::Resource((::Resource*)__native->oldResource);
    Handle = __native->handle.id;
}

Flood::ResourceManager::ResourceManager(::ResourceManager* native)
{
    NativePtr = native;
}

Flood::ResourceManager::ResourceManager(System::IntPtr native)
{
    auto __native = (::ResourceManager*)native.ToPointer();
    NativePtr = __native;
}

Flood::ResourceManager::ResourceManager()
{
    NativePtr = new ::ResourceManager();
}

uint Flood::ResourceManager::GetResource(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    auto ret = ((::ResourceManager*)NativePtr)->getResource(arg0);
    return ret.id;
}

uint Flood::ResourceManager::LoadResource(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    auto ret = ((::ResourceManager*)NativePtr)->loadResource(arg0);
    return ret.id;
}

uint Flood::ResourceManager::LoadResource(Flood::ResourceLoadOptions options)
{
    auto arg0 = ::ResourceLoadOptions();
    arg0.name = marshalString<E_UTF8>(options.Name);
    arg0.stream = (::Stream*)options.Stream->NativePtr;
    arg0.resource = (::Resource*)options.Resource->NativePtr;
    arg0.group = (::ResourceGroup)options.Group;
    arg0.isHighPriority = options.IsHighPriority;
    arg0.sendLoadEvent = options.SendLoadEvent;
    arg0.asynchronousLoad = options.AsynchronousLoad;
    arg0.keepStreamOpen = options.KeepStreamOpen;
    arg0.option = ::ResourceLoadOption();

    auto ret = ((::ResourceManager*)NativePtr)->loadResource(arg0);
    return ret.id;
}

bool Flood::ResourceManager::FindResource(Flood::ResourceLoadOptions options)
{
    auto arg0 = ::ResourceLoadOptions();
    arg0.name = marshalString<E_UTF8>(options.Name);
    arg0.stream = (::Stream*)options.Stream->NativePtr;
    arg0.resource = (::Resource*)options.Resource->NativePtr;
    arg0.group = (::ResourceGroup)options.Group;
    arg0.isHighPriority = options.IsHighPriority;
    arg0.sendLoadEvent = options.SendLoadEvent;
    arg0.asynchronousLoad = options.AsynchronousLoad;
    arg0.keepStreamOpen = options.KeepStreamOpen;
    arg0.option = ::ResourceLoadOption();

    auto ret = ((::ResourceManager*)NativePtr)->findResource(arg0);
    return ret;
}

void Flood::ResourceManager::RemoveResource(Flood::Resource^ resource)
{
    auto arg0 = (::Resource*)resource->NativePtr;
    ((::ResourceManager*)NativePtr)->removeResource(arg0);
}

void Flood::ResourceManager::RemoveResource(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    ((::ResourceManager*)NativePtr)->removeResource(arg0);
}

void Flood::ResourceManager::RemoveUnusedResources()
{
    ((::ResourceManager*)NativePtr)->removeUnusedResources();
}

void Flood::ResourceManager::LoadQueuedResources()
{
    ((::ResourceManager*)NativePtr)->loadQueuedResources();
}

void Flood::ResourceManager::Update()
{
    ((::ResourceManager*)NativePtr)->update();
}

Flood::ResourceLoader^ Flood::ResourceManager::FindLoader(System::String^ extension)
{
    auto arg0 = marshalString<E_UTF8>(extension);
    auto ret = ((::ResourceManager*)NativePtr)->findLoader(arg0);
    return gcnew Flood::ResourceLoader((::ResourceLoader*)ret);
}

Flood::ResourceLoader^ Flood::ResourceManager::FindLoaderByClass(Flood::Class^ klass)
{
    auto arg0 = (::Class*)klass->NativePtr;
    auto ret = ((::ResourceManager*)NativePtr)->findLoaderByClass(arg0);
    return gcnew Flood::ResourceLoader((::ResourceLoader*)ret);
}

void Flood::ResourceManager::SetupResourceLoaders(Flood::Class^ klass)
{
    auto arg0 = (::Class*)klass->NativePtr;
    ((::ResourceManager*)NativePtr)->setupResourceLoaders(arg0);
}

bool Flood::ResourceManager::GetAsynchronousLoading()
{
    auto ret = ((::ResourceManager*)NativePtr)->getAsynchronousLoading();
    return ret;
}

void Flood::ResourceManager::SetAsynchronousLoading(bool v)
{
    ((::ResourceManager*)NativePtr)->setAsynchronousLoading(v);
}

Flood::ResourceManager^ Flood::FloodResourceManager::GetResourceManager()
{
    auto ret = ::GetResourceManager();
    return gcnew Flood::ResourceManager((::ResourceManager*)ret);
}

