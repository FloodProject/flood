/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ResourceManager.h"
#include "Concurrency.h"
#include "ResourceLoader.h"
#include "Resource.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::ResourceManager::ResourceManager(::ResourceManager* native)
{
    NativePtr = native;
}

Flood::ResourceManager::ResourceManager()
{
}

uint Flood::ResourceManager::GetResource(System::String^ name)
{
    return 0;
}

uint Flood::ResourceManager::LoadResource(System::String^ name)
{
    return 0;
}

uint Flood::ResourceManager::LoadResource(Flood::ResourceLoadOptions options)
{
    return 0;
}

bool Flood::ResourceManager::FindResource(Flood::ResourceLoadOptions options)
{
    return false;
}

void Flood::ResourceManager::RemoveResource(Flood::Resource^ resource)
{
}

void Flood::ResourceManager::RemoveResource(System::String^ name)
{
}

void Flood::ResourceManager::RemoveUnusedResources()
{
}

void Flood::ResourceManager::LoadQueuedResources()
{
}

void Flood::ResourceManager::Update()
{
}

System::Collections::Generic::Dictionary<System::String^, uint>^ Flood::ResourceManager::GetResources()
{
    return nullptr;
}

Flood::ResourceLoader^ Flood::ResourceManager::FindLoader(System::String^ extension)
{
    return nullptr;
}

Flood::ResourceLoader^ Flood::ResourceManager::FindLoaderByClass(Flood::Class^ klass)
{
    return nullptr;
}

void Flood::ResourceManager::SetupResourceLoaders(Flood::Class^ klass)
{
}

System::Collections::Generic::Dictionary<System::String^, Flood::ResourceLoader^>^ Flood::ResourceManager::GetResourceLoaders()
{
    return nullptr;
}

bool Flood::ResourceManager::GetAsynchronousLoading()
{
    return false;
}

void Flood::ResourceManager::SetAsynchronousLoading(bool v)
{
}

Flood::HandleManager^ Flood::ResourceManager::GetHandleManager()
{
    return nullptr;
}

Flood::TaskPool^ Flood::ResourceManager::GetTaskPool()
{
    return nullptr;
}

void Flood::ResourceManager::SetTaskPool(Flood::TaskPool^ v)
{
}

Flood::Archive^ Flood::ResourceManager::GetArchive()
{
    return nullptr;
}

void Flood::ResourceManager::SetArchive(Flood::Archive^ archive)
{
}

Flood::ResourceManager^ Flood::FloodResourceManager::GetResourceManager()
{
    return nullptr;
}


