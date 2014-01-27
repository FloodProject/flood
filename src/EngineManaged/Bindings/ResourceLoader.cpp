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
    Name = StringMarshaller::marshalString(native->name);
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
    Name = StringMarshaller::marshalString(__native->name);
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
    auto _this0 = ::ResourceLoadOptions();
    _this0.name = StringMarshaller::marshalUTF8String((*this).Name);
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
    Name = StringMarshaller::marshalString(_this0.name);
    Stream = gcnew Flood::Stream((::Stream*)_this0.stream);
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

Flood::Resource^ Flood::ResourceLoader::Prepare(Flood::ResourceLoadOptions _1)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = StringMarshaller::marshalUTF8String(_1.Name);
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
    auto __ret = ((::ResourceLoader*)NativePtr)->prepare(arg0);
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Resource((::Resource*)__ret);
}

bool Flood::ResourceLoader::Decode(Flood::ResourceLoadOptions _2)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = StringMarshaller::marshalUTF8String(_2.Name);
    if (_2.Stream != nullptr)
        _marshal0.stream = (::Stream*)_2.Stream->NativePtr;
    if (_2.Resource != nullptr)
        _marshal0.resource = (::Resource*)_2.Resource->NativePtr;
    _marshal0.group = (::ResourceGroup)_2.Group;
    _marshal0.isHighPriority = _2.IsHighPriority;
    _marshal0.sendLoadEvent = _2.SendLoadEvent;
    _marshal0.asynchronousLoad = _2.AsynchronousLoad;
    _marshal0.keepStreamOpen = _2.KeepStreamOpen;
    auto _marshal1 = ::ResourceLoadOption();
    _marshal1.key = _2.Option.Key;
    _marshal1.value = _2.Option.Value;
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
    return Instance == obj->Instance;
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
    return StringMarshaller::marshalString(__ret);
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
        auto _marshalElement = StringMarshaller::marshalString(_element);
        _tmp__ret->Add(_marshalElement);
    }
    return _tmp__ret;
}

System::Collections::Generic::List<System::String^>^ Flood::ResourceLoader::Extensions1::get()
{
    auto _tmpExtensions = gcnew System::Collections::Generic::List<System::String^>();
    for(auto _element : ((::ResourceLoader*)NativePtr)->extensions)
    {
        auto _marshalElement = StringMarshaller::marshalString(_element);
        _tmpExtensions->Add(_marshalElement);
    }
    return _tmpExtensions;
}

void Flood::ResourceLoader::Extensions1::set(System::Collections::Generic::List<System::String^>^ value)
{
    auto _tmpvalue = Vector<::String>();
    for each(System::String^ _element in value)
    {
        auto _marshalElement = StringMarshaller::marshalString(_element);
        _tmpvalue.Push(_marshalElement);
    }
    ((::ResourceLoader*)NativePtr)->extensions = _tmpvalue;
}

