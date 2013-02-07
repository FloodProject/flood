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
    // TODO: Struct marshaling
}

Flood::ResourceEvent::ResourceEvent(System::IntPtr native)
{
    // TODO: Struct marshaling
}

Flood::ResourceManager::ResourceManager(::ResourceManager* native)
{
    NativePtr = native;
}

Flood::ResourceManager::ResourceManager(System::IntPtr native)
{
    NativePtr = (::ResourceManager*)native.ToPointer();
}

Flood::ResourceManager::ResourceManager()
{
    NativePtr = new ::ResourceManager();
}

uint Flood::ResourceManager::GetResource(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    auto ret = NativePtr->getResource(arg0);
    return ret.id;
}

uint Flood::ResourceManager::LoadResource(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    auto ret = NativePtr->loadResource(arg0);
    return ret.id;
}

uint Flood::ResourceManager::LoadResource(Flood::ResourceLoadOptions options)
{
    auto _arg0 = (::ResourceLoadOptions*)&options;
    auto arg0 = *_arg0;
    auto ret = NativePtr->loadResource(arg0);
    return ret.id;
}

bool Flood::ResourceManager::FindResource(Flood::ResourceLoadOptions options)
{
    auto _arg0 = (::ResourceLoadOptions*)&options;
    auto arg0 = *_arg0;
    auto ret = NativePtr->findResource(arg0);
    return ret;
}

void Flood::ResourceManager::RemoveResource(Flood::Resource^ resource)
{
    auto arg0 = resource->NativePtr;
    NativePtr->removeResource(arg0);
}

void Flood::ResourceManager::RemoveResource(System::String^ name)
{
    auto arg0 = marshalString<E_UTF8>(name);
    NativePtr->removeResource(arg0);
}

void Flood::ResourceManager::RemoveUnusedResources()
{
    NativePtr->removeUnusedResources();
}

void Flood::ResourceManager::LoadQueuedResources()
{
    NativePtr->loadQueuedResources();
}

void Flood::ResourceManager::Update()
{
    NativePtr->update();
}

Flood::ResourceLoader^ Flood::ResourceManager::FindLoader(System::String^ extension)
{
    auto arg0 = marshalString<E_UTF8>(extension);
    auto ret = NativePtr->findLoader(arg0);
    return gcnew Flood::ResourceLoader((::ResourceLoader*)ret);
}

Flood::ResourceLoader^ Flood::ResourceManager::FindLoaderByClass(Flood::Class^ klass)
{
    auto arg0 = klass->NativePtr;
    auto ret = NativePtr->findLoaderByClass(arg0);
    return gcnew Flood::ResourceLoader((::ResourceLoader*)ret);
}

void Flood::ResourceManager::SetupResourceLoaders(Flood::Class^ klass)
{
    auto arg0 = klass->NativePtr;
    NativePtr->setupResourceLoaders(arg0);
}

bool Flood::ResourceManager::GetAsynchronousLoading()
{
    auto ret = NativePtr->getAsynchronousLoading();
    return ret;
}

void Flood::ResourceManager::SetAsynchronousLoading(bool v)
{
    NativePtr->setAsynchronousLoading(v);
}

Flood::ResourceManager^ Flood::FloodResourceManager::GetResourceManager()
{
    auto ret = ::GetResourceManager();
    return gcnew Flood::ResourceManager((::ResourceManager*)ret);
}

