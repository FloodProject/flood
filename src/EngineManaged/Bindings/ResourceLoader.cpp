/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ResourceLoader.h"
#include "Extension.h"
#include "Resource.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::ResourceLoadOption::ResourceLoadOption(::ResourceLoadOption* native)
{
    __Key = native->key;
    __Value = native->value;
}

Flood::ResourceLoadOption::ResourceLoadOption(System::IntPtr native)
{
    auto __native = (::ResourceLoadOption*)native.ToPointer();
    __Key = __native->key;
    __Value = __native->value;
}

int Flood::ResourceLoadOption::Key::get()
{
    return __Key;
}

void Flood::ResourceLoadOption::Key::set(int value)
{
    __Key = value;
}

int Flood::ResourceLoadOption::Value::get()
{
    return __Value;
}

void Flood::ResourceLoadOption::Value::set(int value)
{
    __Value = value;
}

Flood::ResourceLoadOptions::ResourceLoadOptions(::ResourceLoadOptions* native)
{
    __Name = clix::marshalString<clix::E_UTF8>(native->name);
    __Stream = gcnew Flood::Stream((::Stream*)native->stream);
    __Resource = gcnew Flood::Resource((::Resource*)native->resource);
    __Group = (Flood::ResourceGroup)native->group;
    __IsHighPriority = native->isHighPriority;
    __SendLoadEvent = native->sendLoadEvent;
    __AsynchronousLoad = native->asynchronousLoad;
    __KeepStreamOpen = native->keepStreamOpen;
    Option = Flood::ResourceLoadOption((::ResourceLoadOption*)&native->option);
}

Flood::ResourceLoadOptions::ResourceLoadOptions(System::IntPtr native)
{
    auto __native = (::ResourceLoadOptions*)native.ToPointer();
    __Name = clix::marshalString<clix::E_UTF8>(__native->name);
    __Stream = gcnew Flood::Stream((::Stream*)__native->stream);
    __Resource = gcnew Flood::Resource((::Resource*)__native->resource);
    __Group = (Flood::ResourceGroup)__native->group;
    __IsHighPriority = __native->isHighPriority;
    __SendLoadEvent = __native->sendLoadEvent;
    __AsynchronousLoad = __native->asynchronousLoad;
    __KeepStreamOpen = __native->keepStreamOpen;
    Option = Flood::ResourceLoadOption((::ResourceLoadOption*)&__native->option);
}

void Flood::ResourceLoadOptions::AddOption(int key, int value)
{
    auto _this0 = ::ResourceLoadOptions();
    _this0.name = clix::marshalString<clix::E_UTF8>((*this).Name);
    if ((*this).Stream != nullptr)
        _this0.stream = (::Stream*)(*this).Stream->NativePtr;
    if ((*this).Resource != nullptr)
        _this0.resource = (::Resource*)(*this).Resource->NativePtr;
    _this0.group = (::ResourceGroup)(*this).Group;
    _this0.isHighPriority = (*this).IsHighPriority;
    _this0.sendLoadEvent = (*this).SendLoadEvent;
    _this0.asynchronousLoad = (*this).AsynchronousLoad;
    _this0.keepStreamOpen = (*this).KeepStreamOpen;
    auto _this0_marshal0 = ::ResourceLoadOption();
    _this0_marshal0.key = (*this).Option.Key;
    _this0_marshal0.value = (*this).Option.Value;
    _this0.option = _this0_marshal0;
    _this0.addOption(key, value);
    __Name = clix::marshalString<clix::E_UTF8>(_this0.name);
    __Stream = gcnew Flood::Stream((::Stream*)_this0.stream);
    __Resource = gcnew Flood::Resource((::Resource*)_this0.resource);
    __Group = (Flood::ResourceGroup)_this0.group;
    __IsHighPriority = _this0.isHighPriority;
    __SendLoadEvent = _this0.sendLoadEvent;
    __AsynchronousLoad = _this0.asynchronousLoad;
    __KeepStreamOpen = _this0.keepStreamOpen;
    Option = Flood::ResourceLoadOption((::ResourceLoadOption*)&_this0.option);
}

System::String^ Flood::ResourceLoadOptions::Name::get()
{
    return __Name;
}

void Flood::ResourceLoadOptions::Name::set(System::String^ value)
{
    __Name = value;
}

Flood::Stream^ Flood::ResourceLoadOptions::Stream::get()
{
    return __Stream;
}

void Flood::ResourceLoadOptions::Stream::set(Flood::Stream^ value)
{
    __Stream = value;
}

Flood::Resource^ Flood::ResourceLoadOptions::Resource::get()
{
    return __Resource;
}

void Flood::ResourceLoadOptions::Resource::set(Flood::Resource^ value)
{
    __Resource = value;
}

Flood::ResourceGroup Flood::ResourceLoadOptions::Group::get()
{
    return __Group;
}

void Flood::ResourceLoadOptions::Group::set(Flood::ResourceGroup value)
{
    __Group = value;
}

bool Flood::ResourceLoadOptions::IsHighPriority::get()
{
    return __IsHighPriority;
}

void Flood::ResourceLoadOptions::IsHighPriority::set(bool value)
{
    __IsHighPriority = value;
}

bool Flood::ResourceLoadOptions::SendLoadEvent::get()
{
    return __SendLoadEvent;
}

void Flood::ResourceLoadOptions::SendLoadEvent::set(bool value)
{
    __SendLoadEvent = value;
}

bool Flood::ResourceLoadOptions::AsynchronousLoad::get()
{
    return __AsynchronousLoad;
}

void Flood::ResourceLoadOptions::AsynchronousLoad::set(bool value)
{
    __AsynchronousLoad = value;
}

bool Flood::ResourceLoadOptions::KeepStreamOpen::get()
{
    return __KeepStreamOpen;
}

void Flood::ResourceLoadOptions::KeepStreamOpen::set(bool value)
{
    __KeepStreamOpen = value;
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
    auto arg0 = (::uint8*)buffer.ToPointer();
    auto arg1 = (::size_t)size;
    auto __ret = ((::ResourceStream*)NativePtr)->decode(arg0, arg1);
    return __ret;
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
    return __Instance == obj->__Instance;
}

int Flood::ResourceStream::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::ResourceStream::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::ResourceStream::__Instance::set(System::IntPtr object)
{
    NativePtr = (::ResourceStream*)object.ToPointer();
}

Flood::Stream^ Flood::ResourceStream::Stream::get()
{
    return gcnew Flood::Stream((::Stream*)((::ResourceStream*)NativePtr)->stream);
}

void Flood::ResourceStream::Stream::set(Flood::Stream^ value)
{
    ((::ResourceStream*)NativePtr)->stream = (::Stream*)value->NativePtr;
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
    : Flood::Extension((::Extension*)native)
{
}

Flood::ResourceLoader::ResourceLoader(System::IntPtr native)
    : Flood::Extension(native)
{
    auto __native = (::ResourceLoader*)native.ToPointer();
}

Flood::ResourceLoader::ResourceLoader()
    : Flood::Extension((::Extension*)nullptr)
{
}

Flood::Resource^ Flood::ResourceLoader::Prepare(Flood::ResourceLoadOptions _0)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = clix::marshalString<clix::E_UTF8>(_0.Name);
    if (_0.Stream != nullptr)
        _marshal0.stream = (::Stream*)_0.Stream->NativePtr;
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
    auto __ret = ((::ResourceLoader*)NativePtr)->prepare(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Resource((::Resource*)__ret);
}

bool Flood::ResourceLoader::Decode(Flood::ResourceLoadOptions _1)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = clix::marshalString<clix::E_UTF8>(_1.Name);
    if (_1.Stream != nullptr)
        _marshal0.stream = (::Stream*)_1.Stream->NativePtr;
    if (_1.Resource != nullptr)
        _marshal0.resource = (::Resource*)_1.Resource->NativePtr;
    _marshal0.group = (::ResourceGroup)_1.Group;
    _marshal0.isHighPriority = _1.IsHighPriority;
    _marshal0.sendLoadEvent = _1.SendLoadEvent;
    _marshal0.asynchronousLoad = _1.AsynchronousLoad;
    _marshal0.keepStreamOpen = _1.KeepStreamOpen;
    auto _marshal1 = ::ResourceLoadOption();
    _marshal1.key = _1.Option.Key;
    _marshal1.value = _1.Option.Value;
    _marshal0.option = _marshal1;
    auto arg0 = _marshal0;
    auto __ret = ((::ResourceLoader*)NativePtr)->decode(arg0);
    return __ret;
}

bool Flood::ResourceLoader::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ResourceLoader^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::ResourceLoader::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ExtensionMetadata Flood::ResourceLoader::Metadata::get()
{
    auto __ret = ((::ResourceLoader*)NativePtr)->getMetadata();
    return Flood::ExtensionMetadata((::ExtensionMetadata*)__ret);
}

System::String^ Flood::ResourceLoader::Name::get()
{
    auto __ret = ((::ResourceLoader*)NativePtr)->getName();
    return clix::marshalString<clix::E_UTF8>(__ret);
}

Flood::ResourceGroup Flood::ResourceLoader::ResourceGroup::get()
{
    auto __ret = ((::ResourceLoader*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)__ret;
}

System::Collections::Generic::List<System::String^>^ Flood::ResourceLoader::Extensions::get()
{
    auto &__ret = ((::ResourceLoader*)NativePtr)->getExtensions();
    auto _tmp__ret = gcnew System::Collections::Generic::List<System::String^>();
    for(auto _element : __ret)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmp__ret->Add(_marshalElement);
    }
    return _tmp__ret;
}

System::Collections::Generic::List<System::String^>^ Flood::ResourceLoader::Extensions1::get()
{
    auto _tmp__Extensions = gcnew System::Collections::Generic::List<System::String^>();
    for(auto _element : ((::ResourceLoader*)NativePtr)->extensions)
    {
        auto _marshalElement = clix::marshalString<clix::E_UTF8>(_element);
        _tmp__Extensions->Add(_marshalElement);
    }
    return _tmp__Extensions;
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

