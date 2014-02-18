/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "FileWatcher.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::FileWatchEvent::FileWatchEvent(::FileWatchEvent* native)
{
    __Action = (Flood::FileWatchEventKind)native->action;
    __WatchId = native->watchId;
    __Dir = clix::marshalString<clix::E_UTF8>(native->dir);
    __Filename = clix::marshalString<clix::E_UTF8>(native->filename);
    __Userdata = IntPtr(native->userdata);
}

Flood::FileWatchEvent::FileWatchEvent(System::IntPtr native)
{
    auto __native = (::FileWatchEvent*)native.ToPointer();
    __Action = (Flood::FileWatchEventKind)__native->action;
    __WatchId = __native->watchId;
    __Dir = clix::marshalString<clix::E_UTF8>(__native->dir);
    __Filename = clix::marshalString<clix::E_UTF8>(__native->filename);
    __Userdata = IntPtr(__native->userdata);
}

Flood::FileWatchEvent::FileWatchEvent(Flood::FileWatchEventKind _0, unsigned int _1, System::String^ dir, System::String^ file)
{
    ::FileWatchEvent _native((::FileWatchEventKind)_0, (::FileWatchId)(::uint32)(::uint32_t)_1, clix::marshalString<clix::E_UTF8>(dir), clix::marshalString<clix::E_UTF8>(file));
    this->Action = (Flood::FileWatchEventKind)_native.action;
    this->WatchId = _native.watchId;
    this->Dir = clix::marshalString<clix::E_UTF8>(_native.dir);
    this->Filename = clix::marshalString<clix::E_UTF8>(_native.filename);
    this->Userdata = IntPtr(_native.userdata);
}

Flood::FileWatchEventKind Flood::FileWatchEvent::Action::get()
{
    return __Action;
}

void Flood::FileWatchEvent::Action::set(Flood::FileWatchEventKind value)
{
    __Action = value;
}

unsigned int Flood::FileWatchEvent::WatchId::get()
{
    return __WatchId;
}

void Flood::FileWatchEvent::WatchId::set(unsigned int value)
{
    __WatchId = value;
}

System::String^ Flood::FileWatchEvent::Dir::get()
{
    return __Dir;
}

void Flood::FileWatchEvent::Dir::set(System::String^ value)
{
    __Dir = value;
}

System::String^ Flood::FileWatchEvent::Filename::get()
{
    return __Filename;
}

void Flood::FileWatchEvent::Filename::set(System::String^ value)
{
    __Filename = value;
}

System::IntPtr Flood::FileWatchEvent::Userdata::get()
{
    return __Userdata;
}

void Flood::FileWatchEvent::Userdata::set(System::IntPtr value)
{
    __Userdata = value;
}

Flood::FileWatcher::FileWatcher(::FileWatcher* native)
{
    NativePtr = native;
}

Flood::FileWatcher::FileWatcher(System::IntPtr native)
{
    auto __native = (::FileWatcher*)native.ToPointer();
    NativePtr = __native;
}

Flood::FileWatcher::FileWatcher()
{
}

unsigned int Flood::FileWatcher::AddWatch(System::String^ directory, System::IntPtr userdata)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(directory);
    auto arg1 = (void*)userdata.ToPointer();
    auto __ret = ((::FileWatcher*)NativePtr)->addWatch(arg0, arg1);
    return __ret;
}

void Flood::FileWatcher::RemoveWatch(System::String^ directory)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(directory);
    ((::FileWatcher*)NativePtr)->removeWatch(arg0);
}

void Flood::FileWatcher::RemoveWatch(unsigned int FileWatchId)
{
    auto arg0 = (::FileWatchId)(::uint32)(::uint32_t)FileWatchId;
    ((::FileWatcher*)NativePtr)->removeWatch(arg0);
}

void Flood::FileWatcher::Update()
{
    ((::FileWatcher*)NativePtr)->update();
}

bool Flood::FileWatcher::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<FileWatcher^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::FileWatcher::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::FileWatcher::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::FileWatcher::__Instance::set(System::IntPtr object)
{
    NativePtr = (::FileWatcher*)object.ToPointer();
}
void Flood::FileWatcher::FileWatchEvent::add(System::Action<Flood::FileWatchEvent>^ evt)
{
    if (!_FileWatchEventDelegateInstance)
    {
        _FileWatchEventDelegateInstance = gcnew _FileWatchEventDelegate(this, &Flood::FileWatcher::_FileWatchEventRaise);
        auto _fptr = (void (*)(const ::FileWatchEvent&))Marshal::GetFunctionPointerForDelegate(_FileWatchEventDelegateInstance).ToPointer();
        ((::FileWatcher*)NativePtr)->onFileWatchEvent.Connect(_fptr);
    }
    _FileWatchEvent = static_cast<System::Action<Flood::FileWatchEvent>^>(System::Delegate::Combine(_FileWatchEvent, evt));
}

void Flood::FileWatcher::FileWatchEvent::remove(System::Action<Flood::FileWatchEvent>^ evt)
{
    _FileWatchEvent = static_cast<System::Action<Flood::FileWatchEvent>^>(System::Delegate::Remove(_FileWatchEvent, evt));
}

void Flood::FileWatcher::FileWatchEvent::raise(Flood::FileWatchEvent _0)
{
    _FileWatchEvent(_0);
}

void Flood::FileWatcher::_FileWatchEventRaise(const ::FileWatchEvent& _0)
{
    FileWatchEvent::raise(Flood::FileWatchEvent((::FileWatchEvent*)&_0));
}

