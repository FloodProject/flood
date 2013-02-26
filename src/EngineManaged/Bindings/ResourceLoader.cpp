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
    // TODO: Struct marshaling
}

Flood::ResourceLoadOption::ResourceLoadOption(System::IntPtr native)
{
    // TODO: Struct marshaling
}

Flood::ResourceLoadOptions::ResourceLoadOptions(::ResourceLoadOptions* native)
{
    // TODO: Struct marshaling
}

Flood::ResourceLoadOptions::ResourceLoadOptions(System::IntPtr native)
{
    // TODO: Struct marshaling
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
    NativePtr = (::ResourceStream*)native.ToPointer();
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

