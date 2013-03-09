/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Log.h"
#include "Memory.h"
#include "ResourceHandle.h"
#include "Serialization.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::LogEntry::LogEntry(::LogEntry* native)
{
    Time = native->time;
    Message = marshalString<E_UTF8>(native->message);
    Level = (Flood::LogLevel)native->level;
}

Flood::LogEntry::LogEntry(System::IntPtr native)
{
    auto __native = (::LogEntry*)native.ToPointer();
    Time = __native->time;
    Message = marshalString<E_UTF8>(__native->message);
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

void Flood::Log::Destroy()
{
    auto arg0 = (::Log*)NativePtr;
    ::LogDestroy(arg0);
}

void Flood::Log::AddHandler(Flood::LogFunction^ _4)
{
    auto arg0 = (::Log*)NativePtr;
    auto arg1 = static_cast<::LogFunction>(System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_4).ToPointer());
    ::LogAddHandler(arg0, arg1);
}

void Flood::Log::RemoveHandler(Flood::LogFunction^ _6)
{
    auto arg0 = (::Log*)NativePtr;
    auto arg1 = static_cast<::LogFunction>(System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_6).ToPointer());
    ::LogRemoveHandler(arg0, arg1);
}

void Flood::Log::Write(Flood::LogEntry entry)
{
    auto arg0 = (::Log*)NativePtr;
    auto arg1 = ::LogEntry();
    arg1.time = entry.Time;
    arg1.message = marshalString<E_UTF8>(entry.Message);
    arg1.level = (::LogLevel)entry.Level;

    ::LogWrite(arg0, &arg1);
}

Flood::Log^ Flood::Log::GetDefault()
{
    auto ret = ::LogGetDefault();
    return gcnew Flood::Log((::Log*)ret);
}

void Flood::Log::SetDefault()
{
    auto arg0 = (::Log*)NativePtr;
    ::LogSetDefault(arg0);
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

