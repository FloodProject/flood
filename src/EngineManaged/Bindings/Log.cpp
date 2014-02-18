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
    __Time = native->time;
    __Message = clix::marshalString<clix::E_UTF8>(native->message);
    __Level = (Flood::LogLevel)native->level;
}

Flood::LogEntry::LogEntry(System::IntPtr native)
{
    auto __native = (::LogEntry*)native.ToPointer();
    __Time = __native->time;
    __Message = clix::marshalString<clix::E_UTF8>(__native->message);
    __Level = (Flood::LogLevel)__native->level;
}

float Flood::LogEntry::Time::get()
{
    return __Time;
}

void Flood::LogEntry::Time::set(float value)
{
    __Time = value;
}

System::String^ Flood::LogEntry::Message::get()
{
    return __Message;
}

void Flood::LogEntry::Message::set(System::String^ value)
{
    __Message = value;
}

Flood::LogLevel Flood::LogEntry::Level::get()
{
    return __Level;
}

void Flood::LogEntry::Level::set(Flood::LogLevel value)
{
    __Level = value;
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
    return __Instance == obj->__Instance;
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
    auto __ret = ::LogGetDefault();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Log((::Log*)__ret);
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

System::IntPtr Flood::Log::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Log::__Instance::set(System::IntPtr object)
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

