/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Log.h>
#include "StringConverter.h"

namespace Flood
{
    enum struct LogLevel;
    ref class Allocator;
    ref class Log;
    value struct LogEntry;
}

namespace Flood
{
    public enum struct LogLevel
    {
        Info = 0,
        Warn = 1,
        Error = 2,
        Debug = 3,
        Assert = 4
    };

    public delegate void LogFunction(Flood::LogEntry _2);

    public value struct LogEntry
    {
    public:

        LogEntry(::LogEntry* native);
        LogEntry(System::IntPtr native);
        float Time;
        System::String^ Message;
        Flood::LogLevel Level;
    };

    public ref class Log : ICppInstance
    {
    public:

        property ::Log* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Log(::Log* native);
        Log(System::IntPtr native);
        Log();

    private:
        delegate void _HandlersDelegate(::LogEntry* _0);
        _HandlersDelegate^ _HandlersDelegateInstance;
        void _HandlersRaise(::LogEntry* _0);
        System::Action<Flood::LogEntry>^ _Handlers;
    public:
        event System::Action<Flood::LogEntry>^ Handlers
        {
            void add(System::Action<Flood::LogEntry>^ evt);
            void remove(System::Action<Flood::LogEntry>^ evt);
            void raise(Flood::LogEntry _0);
        }
        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

        void Destroy();

        void AddHandler(Flood::LogFunction^ _1);

        void RemoveHandler(Flood::LogFunction^ _1);

        void Write(Flood::LogEntry entry);

        void SetDefault();

        static Flood::Log^ GetDefault();

        static void Info(System::String^ msg);

        static void Warn(System::String^ msg);

        static void Error(System::String^ msg);

        static void Debug(System::String^ msg);

        static void Assert(System::String^ msg);
    };
}
