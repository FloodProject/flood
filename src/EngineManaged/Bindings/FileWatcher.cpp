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
    Action = (Flood::FileWatchEventKind)native->action;
    WatchId = native->watchId;
    Dir = StringMarshaller::marshalString(native->dir);
    Filename = StringMarshaller::marshalString(native->filename);
    Userdata = IntPtr(native->userdata);
}

Flood::FileWatchEvent::FileWatchEvent(System::IntPtr native)
{
    auto __native = (::FileWatchEvent*)native.ToPointer();
    Action = (Flood::FileWatchEventKind)__native->action;
    WatchId = __native->watchId;
    Dir = StringMarshaller::marshalString(__native->dir);
    Filename = StringMarshaller::marshalString(__native->filename);
    Userdata = IntPtr(__native->userdata);
}

Flood::FileWatchEvent::FileWatchEvent(Flood::FileWatchEventKind _0, unsigned int _1, System::String^ dir, System::String^ file)
{
    ::FileWatchEvent _native((::FileWatchEventKind)_0, (::FileWatchId)(::uint32)(::uint32_t)_1, StringMarshaller::marshalUTF8String(dir), StringMarshaller::marshalUTF8String(file));
    this->Action = (Flood::FileWatchEventKind)_native.action;
    this->WatchId = _native.watchId;
    this->Dir = StringMarshaller::marshalString(_native.dir);
    this->Filename = StringMarshaller::marshalString(_native.filename);
    this->Userdata = IntPtr(_native.userdata);
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
    auto arg0 = StringMarshaller::marshalUTF8String(directory);
    auto arg1 = (void*)userdata.ToPointer();
    auto __ret = ((::FileWatcher*)NativePtr)->addWatch(arg0, arg1);
    return __ret;
}

void Flood::FileWatcher::RemoveWatch(System::String^ directory)
{
    auto arg0 = StringMarshaller::marshalUTF8String(directory);
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
    return Instance == obj->Instance;
}

int Flood::FileWatcher::GetHashCode()
{
    return (int)NativePtr;
}

System::IntPtr Flood::FileWatcher::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::FileWatcher::Instance::set(System::IntPtr object)
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

