/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ResourceLoader.h"
#include "Extension.h"
#include "Resource.h"

using namespace System;
using namespace System::Runtime::InteropServices;

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
    Name = clix::marshalString<clix::E_UTF8>(native->name);
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
    Name = clix::marshalString<clix::E_UTF8>(__native->name);
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
    auto _this0 = ::ResourceLoadOptions();
    _this0.name = clix::marshalString<clix::E_UTF8>((*this).Name);
    if ((*this).Resource != nullptr)
        _this0.resource = (::Resource*)(*this).Resource->NativePtr;
    _this0.group = (::ResourceGroup)(*this).Group;
    _this0.isHighPriority = (*this).IsHighPriority;
    _this0.sendLoadEvent = (*this).SendLoadEvent;
    _this0.asynchronousLoad = (*this).AsynchronousLoad;
    _this0.keepStreamOpen = (*this).KeepStreamOpen;
    auto _marshal0 = ::ResourceLoadOption();
    _marshal0.key = (*this).Option.Key;
    _marshal0.value = (*this).Option.Value;
    _this0.option = _marshal0;
    _this0.addOption(key, value);
    Name = clix::marshalString<clix::E_UTF8>(_this0.name);
    Resource = gcnew Flood::Resource((::Resource*)_this0.resource);
    Group = (Flood::ResourceGroup)_this0.group;
    IsHighPriority = _this0.isHighPriority;
    SendLoadEvent = _this0.sendLoadEvent;
    AsynchronousLoad = _this0.asynchronousLoad;
    KeepStreamOpen = _this0.keepStreamOpen;
    Option = Flood::ResourceLoadOption((::ResourceLoadOption*)&_this0.option);
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

bool Flood::ResourceStream::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ResourceStream^>(object);
    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ResourceStream::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::ResourceStream::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::ResourceStream::Instance::set(System::IntPtr object)
{
    NativePtr = (::ResourceStream*)object.ToPointer();
}

Flood::ResourceLoader^ Flood::ResourceStream::Loader::get()
{
    return gcnew Flood::ResourceLoader((::ResourceLoader*)((::ResourceStream*)NativePtr)->loader);
}

void Flood::ResourceStream::Loader::set(Flood::ResourceLoader^ value)
{
    ((::ResourceStream*)NativePtr)->loader = (::ResourceLoader*)value->NativePtr;
}

Flood::ResourceLoader::ResourceLoader(::ResourceLoader* native)
    : Flood::Extension(native)
{
}

Flood::ResourceLoader::ResourceLoader(System::IntPtr native)
    : Flood::Extension(native)
{
    auto __native = (::ResourceLoader*)native.ToPointer();
}

Flood::ResourceLoader::ResourceLoader()
    : Flood::Extension(nullptr)
{
}

Flood::Resource^ Flood::ResourceLoader::Prepare(Flood::ResourceLoadOptions _0)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = clix::marshalString<clix::E_UTF8>(_0.Name);
    if (_0.Resource != nullptr)
        _marshal0.resource = (::Resource*)_0.Resource->NativePtr;
    _marshal0.group = (::ResourceGroup)_0.Group;
    _marshal0.isHighPriority = _0.IsHighPriority;
    _marshal0.sendLoadEvent = _0.SendLoadEvent;
    _marshal0.asynchronousLoad = _0.AsynchronousLoad;
    _marshal0.keepStreamOpen = _0.KeepStreamOpen;
    auto _marshal1 = ::ResourceLoadOption();
    _marshal1.key = _0.Option.Key;
    _marshal1.value = _0.Option.Value;
    _marshal0.option = _marshal1;
    auto arg0 = _marshal0;
    auto ret = ((::ResourceLoader*)NativePtr)->prepare(arg0);
    return gcnew Flood::Resource((::Resource*)ret);
}

bool Flood::ResourceLoader::Decode(Flood::ResourceLoadOptions _0)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = clix::marshalString<clix::E_UTF8>(_0.Name);
    if (_0.Resource != nullptr)
        _marshal0.resource = (::Resource*)_0.Resource->NativePtr;
    _marshal0.group = (::ResourceGroup)_0.Group;
    _marshal0.isHighPriority = _0.IsHighPriority;
    _marshal0.sendLoadEvent = _0.SendLoadEvent;
    _marshal0.asynchronousLoad = _0.AsynchronousLoad;
    _marshal0.keepStreamOpen = _0.KeepStreamOpen;
    auto _marshal1 = ::ResourceLoadOption();
    _marshal1.key = _0.Option.Key;
    _marshal1.value = _0.Option.Value;
    _marshal0.option = _marshal1;
    auto arg0 = _marshal0;
    auto ret = ((::ResourceLoader*)NativePtr)->decode(arg0);
    return ret;
}

bool Flood::ResourceLoader::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ResourceLoader^>(object);
    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ResourceLoader::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ExtensionMetadata Flood::ResourceLoader::Metadata::get()
{
    auto ret = ((::ResourceLoader*)NativePtr)->getMetadata();
    return Flood::ExtensionMetadata((::ExtensionMetadata*)ret);
}

System::String^ Flood::ResourceLoader::Name::get()
{
    auto ret = ((::ResourceLoader*)NativePtr)->getName();
    return clix::marshalString<clix::E_UTF8>(ret);
}

Flood::ResourceGroup Flood::ResourceLoader::ResourceGroup::get()
{
    auto ret = ((::ResourceLoader*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

System::Collections::Generic::List<System::String^>^ Flood::ResourceLoader::Extensions::get()
{
    auto &ret = ((::ResourceLoader*)NativePtr)->getExtensions();
    auto _tmpret = gcnew System::Collections::Generic::List<System::String^>();
    for(auto _element : ret)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

System::Collections::Generic::List<System::String^>^ Flood::ResourceLoader::Extensions1::get()
{
    auto _tmpExtensions = gcnew System::Collections::Generic::List<System::String^>();
    for(auto _element : ((::ResourceLoader*)NativePtr)->extensions)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmpExtensions->Add(_marshalElement);
    }
    return _tmpExtensions;
}

void Flood::ResourceLoader::Extensions1::set(System::Collections::Generic::List<System::String^>^ value)
{
    auto _tmpvalue = std::vector<::String>();
    for each(System::String^ _element in value)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmpvalue.push_back(_marshalElement);
    }
    ((::ResourceLoader*)NativePtr)->extensions = _tmpvalue;
}

