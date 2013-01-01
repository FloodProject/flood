/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ResourceLoader.h"
#include "Stream.h"
#include "Extension.h"
#include "Resource.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::ResourceStream::ResourceStream(::ResourceStream* native)
{
    NativePtr = native;
}

Flood::ResourceStream::ResourceStream()
{
}

int Flood::ResourceStream::Decode(unsigned char buffer, unsigned int size)
{
    return 0;
}

void Flood::ResourceStream::Reset()
{
}

Flood::ResourceLoader::ResourceLoader(::ResourceLoader* native)
{
    NativePtr = native;
}

Flood::ResourceLoader::ResourceLoader()
{
}

Flood::Class^ Flood::ResourceLoader::GetType()
{
    return nullptr;
}

Flood::Class^ Flood::ResourceLoader::GetStaticType()
{
    return nullptr;
}

Flood::ExtensionMetadata Flood::ResourceLoader::GetMetadata()
{
    return ExtensionMetadata();
}

Flood::Resource^ Flood::ResourceLoader::Prepare(Flood::ResourceLoadOptions)
{
    return nullptr;
}

bool Flood::ResourceLoader::Decode(Flood::ResourceLoadOptions)
{
    return false;
}

System::String^ Flood::ResourceLoader::GetName()
{
    return nullptr;
}

Flood::Class^ Flood::ResourceLoader::GetResourceClass()
{
    return nullptr;
}

Flood::ResourceGroup Flood::ResourceLoader::GetResourceGroup()
{
    return ResourceGroup::General;
}

System::Collections::Generic::List<System::String^>^ Flood::ResourceLoader::GetExtensions()
{
    return nullptr;
}

void Flood::ResourceLoadOptions::AddOption(int key, int value)
{
}

Flood::Class^ Flood::FloodResourceLoader::ResourceLoaderGetType()
{
    return nullptr;
}


