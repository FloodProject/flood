/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Log.h"
#include "Memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::LogEntry::LogEntry(::LogEntry* native)
{
    Time = native->time;
    Message = clix::marshalString<clix::E_UTF8>(native->message);
    Level = (Flood::LogLevel)native->level;
}

Flood::LogEntry::LogEntry(System::IntPtr native)
{
    auto __native = (::LogEntry*)native.ToPointer();
    Time = __native->time;
    Message = clix::marshalString<clix::E_UTF8>(__native->message);
    Level = (Flood::LogLevel)__native->level;
}

Flood::Log::Log(::Log* native)
{
    NativePtr = native;
}

Flood::Log::Log(System::IntPtr native)
{
    auto __native = (::Log*)native.ToPointer();
    NativePtr = __native;
}

Flood::Log::Log()
{
    NativePtr = new ::Log();
}

bool Flood::Log::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Log^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::Log::GetHashCode()
{
    return (int)NativePtr;
}

void Flood::Log::Destroy()
{
    auto arg0 = (::Log*)NativePtr;
    ::LogDestroy(arg0);
}

void Flood::Log::AddHandler(Flood::LogFunction^ _1)
{
    auto arg0 = (::Log*)NativePtr;
    auto arg1 = static_cast<::LogFunction>(System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_1).ToPointer());
    ::LogAddHandler(arg0, arg1);
}

void Flood::Log::RemoveHandler(Flood::LogFunction^ _1)
{
    auto arg0 = (::Log*)NativePtr;
    auto arg1 = static_cast<::LogFunction>(System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_1).ToPointer());
    ::LogRemoveHandler(arg0, arg1);
}

void Flood::Log::Write(Flood::LogEntry entry)
{
    auto arg0 = (::Log*)NativePtr;
    auto _marshal1 = ::LogEntry();
    _marshal1.time = entry.Time;
    _marshal1.message = clix::marshalString<clix::E_UTF8>(entry.Message);
    _marshal1.level = (::LogLevel)entry.Level;
    auto arg1 = _marshal1;
    ::LogWrite(arg0, &arg1);
}

void Flood::Log::SetDefault()
{
    auto arg0 = (::Log*)NativePtr;
    ::LogSetDefault(arg0);
}

Flood::Log^ Flood::Log::GetDefault()
{
    auto ret = ::LogGetDefault();
    return gcnew Flood::Log((::Log*)ret);
}

void Flood::Log::Info(System::String^ msg)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(msg);
    auto arg0 = _arg0.c_str();
    ::LogInfo(arg0);
}

void Flood::Log::Warn(System::String^ msg)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(msg);
    auto arg0 = _arg0.c_str();
    ::LogWarn(arg0);
}

void Flood::Log::Error(System::String^ msg)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(msg);
    auto arg0 = _arg0.c_str();
    ::LogError(arg0);
}

void Flood::Log::Debug(System::String^ msg)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(msg);
    auto arg0 = _arg0.c_str();
    ::LogDebug(arg0);
}

void Flood::Log::Assert(System::String^ msg)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(msg);
    auto arg0 = _arg0.c_str();
    ::LogAssert(arg0);
}

System::IntPtr Flood::Log::Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Log::Instance::set(System::IntPtr object)
{
    NativePtr = (::Log*)object.ToPointer();
}
void Flood::Log::Handlers::add(System::Action<Flood::LogEntry>^ evt)
{
    if (!_HandlersDelegateInstance)
    {
        _HandlersDelegateInstance = gcnew _HandlersDelegate(this, &Flood::Log::_HandlersRaise);
        auto _fptr = (void (*)(::LogEntry*))Marshal::GetFunctionPointerForDelegate(_HandlersDelegateInstance).ToPointer();
        ((::Log*)NativePtr)->handlers.Connect(_fptr);
    }
    _Handlers = static_cast<System::Action<Flood::LogEntry>^>(System::Delegate::Combine(_Handlers, evt));
}

void Flood::Log::Handlers::remove(System::Action<Flood::LogEntry>^ evt)
{
    _Handlers = static_cast<System::Action<Flood::LogEntry>^>(System::Delegate::Remove(_Handlers, evt));
}

void Flood::Log::Handlers::raise(Flood::LogEntry _0)
{
    _Handlers(_0);
}

void Flood::Log::_HandlersRaise(::LogEntry* _0)
{
    Handlers::raise(Flood::LogEntry((::LogEntry*)_0));
}

