/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ResourceManager.h"
#include "Archive.h"
#include "FileWatcher.h"
#include "ResourceLoader.h"
#include "Stream.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::ResourceEvent::ResourceEvent(::ResourceEvent* native)
{
    NativePtr = native;
}

Flood::ResourceEvent::ResourceEvent(System::IntPtr native)
{
    auto __native = (::ResourceEvent*)native.ToPointer();
    NativePtr = __native;
}

Flood::ResourceEvent::ResourceEvent()
{
    NativePtr = new ::ResourceEvent();
}

bool Flood::ResourceEvent::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ResourceEvent^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::ResourceEvent::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::ResourceEvent::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::ResourceEvent::__Instance::set(System::IntPtr object)
{
    NativePtr = (::ResourceEvent*)object.ToPointer();
}

Flood::Resource^ Flood::ResourceEvent::Resource::get()
{
    return (((::ResourceEvent*)NativePtr)->resource == nullptr) ? nullptr : gcnew Flood::Resource((::Resource*)((::ResourceEvent*)NativePtr)->resource);
}

void Flood::ResourceEvent::Resource::set(Flood::Resource^ value)
{
    ((::ResourceEvent*)NativePtr)->resource = (::Resource*)value->NativePtr;
}

Flood::Resource^ Flood::ResourceEvent::OldResource::get()
{
    return (((::ResourceEvent*)NativePtr)->oldResource == nullptr) ? nullptr : gcnew Flood::Resource((::Resource*)((::ResourceEvent*)NativePtr)->oldResource);
}

void Flood::ResourceEvent::OldResource::set(Flood::Resource^ value)
{
    ((::ResourceEvent*)NativePtr)->oldResource = (::Resource*)value->NativePtr;
}

Flood::ResourceHandle<Flood::Resource^> Flood::ResourceEvent::Handle::get()
{
    return Flood::ResourceHandle<Flood::Resource^>(((::ResourceEvent*)NativePtr)->handle.id);
}

void Flood::ResourceEvent::Handle::set(Flood::ResourceHandle<Flood::Resource^> value)
{
    ((::ResourceEvent*)NativePtr)->handle = (HandleId)value.Id;
}

generic<typename T>
Flood::ResourceHandle<T> Flood::ResourceManager::GetResource(System::String^ name)
{
    auto arg0 = StringMarshaller::marshalString(name);
    auto __ret = ((::ResourceManager*)NativePtr)->getResource(arg0);
    return Flood::ResourceHandle<T>(__ret.id);
}


generic<typename T>
Flood::ResourceHandle<T> Flood::ResourceManager::LoadResource(System::String^ name)
{
    auto arg0 = StringMarshaller::marshalString(name);
    auto __ret = ((::ResourceManager*)NativePtr)->loadResource(arg0);
    return Flood::ResourceHandle<T>(__ret.id);
}


generic<typename T>
Flood::ResourceHandle<T> Flood::ResourceManager::LoadResource(Flood::ResourceLoadOptions options)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = StringMarshaller::marshalUTF8String(options.Name);
    if (options.Stream != nullptr)
        _marshal0.stream = (::Stream*)options.Stream->NativePtr;
    if (options.Resource != nullptr)
        _marshal0.resource = (::Resource*)options.Resource->NativePtr;
    _marshal0.group = (::ResourceGroup)options.Group;
    _marshal0.isHighPriority = options.IsHighPriority;
    _marshal0.sendLoadEvent = options.SendLoadEvent;
    _marshal0.asynchronousLoad = options.AsynchronousLoad;
    _marshal0.keepStreamOpen = options.KeepStreamOpen;
    auto _marshal1 = ::ResourceLoadOption();
    _marshal1.key = options.Option.Key;
    _marshal1.value = options.Option.Value;
    _marshal0.option = _marshal1;
    auto arg0 = _marshal0;
    auto __ret = ((::ResourceManager*)NativePtr)->loadResource(arg0);
    return Flood::ResourceHandle<T>(__ret.id);
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

Flood::ResourceHandle<Flood::Resource^> Flood::ResourceManager::GetResource(System::String^ name)
{
    auto arg0 = StringMarshaller::marshalUTF8String(name);
    auto __ret = ((::ResourceManager*)NativePtr)->getResource(arg0);
    return Flood::ResourceHandle<Flood::Resource^>(__ret.id);
}

Flood::ResourceHandle<Flood::Resource^> Flood::ResourceManager::LoadResource(System::String^ name)
{
    auto arg0 = StringMarshaller::marshalUTF8String(name);
    auto __ret = ((::ResourceManager*)NativePtr)->loadResource(arg0);
    return Flood::ResourceHandle<Flood::Resource^>(__ret.id);
}

Flood::ResourceHandle<Flood::Resource^> Flood::ResourceManager::LoadResource(Flood::ResourceLoadOptions options)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = StringMarshaller::marshalUTF8String(options.Name);
    if (options.Stream != nullptr)
        _marshal0.stream = (::Stream*)options.Stream->NativePtr;
    if (options.Resource != nullptr)
        _marshal0.resource = (::Resource*)options.Resource->NativePtr;
    _marshal0.group = (::ResourceGroup)options.Group;
    _marshal0.isHighPriority = options.IsHighPriority;
    _marshal0.sendLoadEvent = options.SendLoadEvent;
    _marshal0.asynchronousLoad = options.AsynchronousLoad;
    _marshal0.keepStreamOpen = options.KeepStreamOpen;
    auto _marshal1 = ::ResourceLoadOption();
    _marshal1.key = options.Option.Key;
    _marshal1.value = options.Option.Value;
    _marshal0.option = _marshal1;
    auto arg0 = _marshal0;
    auto __ret = ((::ResourceManager*)NativePtr)->loadResource(arg0);
    return Flood::ResourceHandle<Flood::Resource^>(__ret.id);
}

bool Flood::ResourceManager::FindResource(Flood::ResourceLoadOptions options)
{
    auto _marshal0 = ::ResourceLoadOptions();
    _marshal0.name = StringMarshaller::marshalUTF8String(options.Name);
    if (options.Stream != nullptr)
        _marshal0.stream = (::Stream*)options.Stream->NativePtr;
    if (options.Resource != nullptr)
        _marshal0.resource = (::Resource*)options.Resource->NativePtr;
    _marshal0.group = (::ResourceGroup)options.Group;
    _marshal0.isHighPriority = options.IsHighPriority;
    _marshal0.sendLoadEvent = options.SendLoadEvent;
    _marshal0.asynchronousLoad = options.AsynchronousLoad;
    _marshal0.keepStreamOpen = options.KeepStreamOpen;
    auto _marshal1 = ::ResourceLoadOption();
    _marshal1.key = options.Option.Key;
    _marshal1.value = options.Option.Value;
    _marshal0.option = _marshal1;
    auto arg0 = _marshal0;
    auto __ret = ((::ResourceManager*)NativePtr)->findResource(arg0);
    return __ret;
}

void Flood::ResourceManager::RemoveResource(Flood::Resource^ resource)
{
    auto arg0 = (::Resource*)resource->NativePtr;
    ((::ResourceManager*)NativePtr)->removeResource(arg0);
}

void Flood::ResourceManager::RemoveResource(System::String^ name)
{
    auto arg0 = StringMarshaller::marshalString(name);
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
    auto arg0 = StringMarshaller::marshalString(extension);
    auto __ret = ((::ResourceManager*)NativePtr)->findLoader(arg0);
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::ResourceLoader((::ResourceLoader*)__ret);
}

bool Flood::ResourceManager::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<ResourceManager^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::ResourceManager::GetHashCode()
{
    return (int)NativePtr;
}

Flood::ResourceManager^ Flood::ResourceManager::GetResourceManager()
{
    auto __ret = ::GetResourceManager();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::ResourceManager((::ResourceManager*)__ret);
}

System::IntPtr Flood::ResourceManager::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::ResourceManager::__Instance::set(System::IntPtr object)
{
    NativePtr = (::ResourceManager*)object.ToPointer();
}

System::Collections::Generic::Dictionary<System::String^, Flood::ResourceHandle<Flood::Resource^>>^ Flood::ResourceManager::Resources::get()
{
    auto &__ret = ((::ResourceManager*)NativePtr)->getResources();
    auto _tmp__ret = gcnew System::Collections::Generic::Dictionary<System::String^, Flood::ResourceHandle<Flood::Resource^>>();
    for(auto _it = __ret.Begin(); _it != __ret.End(); ++_it)
    {
        auto& _key = _it->first;
        auto& _val = _it->second;
        auto _marshalKey = StringMarshaller::marshalString(_key);
        auto _marshalValue = Flood::ResourceHandle<Flood::Resource^>(_val.id);
        _tmp__ret->Add(_marshalKey, _marshalValue);
    }
    return (System::Collections::Generic::Dictionary<System::String^, Flood::ResourceHandle<Flood::Resource^>>^)(_tmp__ret);
}

System::Collections::Generic::Dictionary<System::String^, Flood::ResourceLoader^>^ Flood::ResourceManager::ResourceLoaders::get()
{
    auto &__ret = ((::ResourceManager*)NativePtr)->getResourceLoaders();
    auto _tmp__ret = gcnew System::Collections::Generic::Dictionary<System::String^, Flood::ResourceLoader^>();
    for(auto _it = __ret.Begin(); _it != __ret.End(); ++_it)
    {
        auto& _key = _it->first;
        auto& _val = _it->second;
        auto _marshalKey = StringMarshaller::marshalString(_key);
        auto _marshalValue = (_val.get() == nullptr) ? nullptr : gcnew Flood::ResourceLoader((::ResourceLoader*)_val.get());
        _tmp__ret->Add(_marshalKey, _marshalValue);
    }
    return (System::Collections::Generic::Dictionary<System::String^, Flood::ResourceLoader^>^)(_tmp__ret);
}

bool Flood::ResourceManager::AsynchronousLoading::get()
{
    auto __ret = ((::ResourceManager*)NativePtr)->getAsynchronousLoading();
    return __ret;
}

void Flood::ResourceManager::AsynchronousLoading::set(bool v)
{
    ((::ResourceManager*)NativePtr)->setAsynchronousLoading(v);
}

Flood::Archive^ Flood::ResourceManager::Archive::get()
{
    auto __ret = ((::ResourceManager*)NativePtr)->getArchive();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Archive((::Archive*)__ret);
}

void Flood::ResourceManager::Archive::set(Flood::Archive^ archive)
{
    auto arg0 = (::Archive*)archive->NativePtr;
    ((::ResourceManager*)NativePtr)->setArchive(arg0);
}

void Flood::ResourceManager::ResourcePrepared::add(System::Action<Flood::ResourceEvent^>^ evt)
{
    if (!_ResourcePreparedDelegateInstance)
    {
        _ResourcePreparedDelegateInstance = gcnew _ResourcePreparedDelegate(this, &Flood::ResourceManager::_ResourcePreparedRaise);
        auto _fptr = (void (*)(const ::ResourceEvent&))Marshal::GetFunctionPointerForDelegate(_ResourcePreparedDelegateInstance).ToPointer();
        ((::ResourceManager*)NativePtr)->onResourcePrepared.Connect(_fptr);
    }
    _ResourcePrepared = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Combine(_ResourcePrepared, evt));
}

void Flood::ResourceManager::ResourcePrepared::remove(System::Action<Flood::ResourceEvent^>^ evt)
{
    _ResourcePrepared = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Remove(_ResourcePrepared, evt));
}

void Flood::ResourceManager::ResourcePrepared::raise(Flood::ResourceEvent^ _0)
{
    _ResourcePrepared(_0);
}

void Flood::ResourceManager::_ResourcePreparedRaise(const ::ResourceEvent& _0)
{
    ResourcePrepared::raise((&_0 == nullptr) ? nullptr : gcnew Flood::ResourceEvent((::ResourceEvent*)&_0));
}

void Flood::ResourceManager::ResourceLoaded::add(System::Action<Flood::ResourceEvent^>^ evt)
{
    if (!_ResourceLoadedDelegateInstance)
    {
        _ResourceLoadedDelegateInstance = gcnew _ResourceLoadedDelegate(this, &Flood::ResourceManager::_ResourceLoadedRaise);
        auto _fptr = (void (*)(const ::ResourceEvent&))Marshal::GetFunctionPointerForDelegate(_ResourceLoadedDelegateInstance).ToPointer();
        ((::ResourceManager*)NativePtr)->onResourceLoaded.Connect(_fptr);
    }
    _ResourceLoaded = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Combine(_ResourceLoaded, evt));
}

void Flood::ResourceManager::ResourceLoaded::remove(System::Action<Flood::ResourceEvent^>^ evt)
{
    _ResourceLoaded = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Remove(_ResourceLoaded, evt));
}

void Flood::ResourceManager::ResourceLoaded::raise(Flood::ResourceEvent^ _0)
{
    _ResourceLoaded(_0);
}

void Flood::ResourceManager::_ResourceLoadedRaise(const ::ResourceEvent& _0)
{
    ResourceLoaded::raise((&_0 == nullptr) ? nullptr : gcnew Flood::ResourceEvent((::ResourceEvent*)&_0));
}

void Flood::ResourceManager::ResourceRemoved::add(System::Action<Flood::ResourceEvent^>^ evt)
{
    if (!_ResourceRemovedDelegateInstance)
    {
        _ResourceRemovedDelegateInstance = gcnew _ResourceRemovedDelegate(this, &Flood::ResourceManager::_ResourceRemovedRaise);
        auto _fptr = (void (*)(const ::ResourceEvent&))Marshal::GetFunctionPointerForDelegate(_ResourceRemovedDelegateInstance).ToPointer();
        ((::ResourceManager*)NativePtr)->onResourceRemoved.Connect(_fptr);
    }
    _ResourceRemoved = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Combine(_ResourceRemoved, evt));
}

void Flood::ResourceManager::ResourceRemoved::remove(System::Action<Flood::ResourceEvent^>^ evt)
{
    _ResourceRemoved = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Remove(_ResourceRemoved, evt));
}

void Flood::ResourceManager::ResourceRemoved::raise(Flood::ResourceEvent^ _0)
{
    _ResourceRemoved(_0);
}

void Flood::ResourceManager::_ResourceRemovedRaise(const ::ResourceEvent& _0)
{
    ResourceRemoved::raise((&_0 == nullptr) ? nullptr : gcnew Flood::ResourceEvent((::ResourceEvent*)&_0));
}

void Flood::ResourceManager::ResourceReloaded::add(System::Action<Flood::ResourceEvent^>^ evt)
{
    if (!_ResourceReloadedDelegateInstance)
    {
        _ResourceReloadedDelegateInstance = gcnew _ResourceReloadedDelegate(this, &Flood::ResourceManager::_ResourceReloadedRaise);
        auto _fptr = (void (*)(const ::ResourceEvent&))Marshal::GetFunctionPointerForDelegate(_ResourceReloadedDelegateInstance).ToPointer();
        ((::ResourceManager*)NativePtr)->onResourceReloaded.Connect(_fptr);
    }
    _ResourceReloaded = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Combine(_ResourceReloaded, evt));
}

void Flood::ResourceManager::ResourceReloaded::remove(System::Action<Flood::ResourceEvent^>^ evt)
{
    _ResourceReloaded = static_cast<System::Action<Flood::ResourceEvent^>^>(System::Delegate::Remove(_ResourceReloaded, evt));
}

void Flood::ResourceManager::ResourceReloaded::raise(Flood::ResourceEvent^ _0)
{
    _ResourceReloaded(_0);
}

void Flood::ResourceManager::_ResourceReloadedRaise(const ::ResourceEvent& _0)
{
    ResourceReloaded::raise((&_0 == nullptr) ? nullptr : gcnew Flood::ResourceEvent((::ResourceEvent*)&_0));
}

void Flood::ResourceManager::ResourceLoaderRegistered::add(System::Action<Flood::ResourceLoader^>^ evt)
{
    if (!_ResourceLoaderRegisteredDelegateInstance)
    {
        _ResourceLoaderRegisteredDelegateInstance = gcnew _ResourceLoaderRegisteredDelegate(this, &Flood::ResourceManager::_ResourceLoaderRegisteredRaise);
        auto _fptr = (void (*)(const ::ResourceLoader&))Marshal::GetFunctionPointerForDelegate(_ResourceLoaderRegisteredDelegateInstance).ToPointer();
        ((::ResourceManager*)NativePtr)->onResourceLoaderRegistered.Connect(_fptr);
    }
    _ResourceLoaderRegistered = static_cast<System::Action<Flood::ResourceLoader^>^>(System::Delegate::Combine(_ResourceLoaderRegistered, evt));
}

void Flood::ResourceManager::ResourceLoaderRegistered::remove(System::Action<Flood::ResourceLoader^>^ evt)
{
    _ResourceLoaderRegistered = static_cast<System::Action<Flood::ResourceLoader^>^>(System::Delegate::Remove(_ResourceLoaderRegistered, evt));
}

void Flood::ResourceManager::ResourceLoaderRegistered::raise(Flood::ResourceLoader^ _0)
{
    _ResourceLoaderRegistered(_0);
}

void Flood::ResourceManager::_ResourceLoaderRegisteredRaise(const ::ResourceLoader& _0)
{
    ResourceLoaderRegistered::raise((&_0 == nullptr) ? nullptr : gcnew Flood::ResourceLoader((::ResourceLoader*)&_0));
}

