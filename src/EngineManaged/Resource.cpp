/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Resource.h"
#include "ResourceLoader.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Resource::Resource(::Resource* native)
{
    NativePtr = native;
}

Flood::Class^ Flood::Resource::GetType()
{
    return nullptr;
}

Flood::Class^ Flood::Resource::GetStaticType()
{
    return nullptr;
}

System::String^ Flood::Resource::GetPath()
{
    return nullptr;
}

void Flood::Resource::SetPath(System::String^ v)
{
}

Flood::ResourceStatus Flood::Resource::GetStatus()
{
    return ResourceStatus::Error;
}

void Flood::Resource::SetStatus(Flood::ResourceStatus v)
{
}

bool Flood::Resource::IsLoaded()
{
    return false;
}

Flood::ResourceGroup Flood::Resource::GetResourceGroup()
{
    return ResourceGroup::General;
}

Flood::Enum^ Flood::FloodResource::ResourceStatusGetType()
{
    return nullptr;
}

Flood::Enum^ Flood::FloodResource::ResourceGroupGetType()
{
    return nullptr;
}

Flood::Class^ Flood::FloodResource::ResourceGetType()
{
    return nullptr;
}

Flood::ReferenceCounted^ Flood::FloodResource::ResourceHandleFind(unsigned int id)
{
    return nullptr;
}

void Flood::FloodResource::ResourceHandleDestroy(unsigned int id)
{
}

uint Flood::FloodResource::ResourceHandleCreate(Flood::Resource^ _129)
{
    return 0;
}


