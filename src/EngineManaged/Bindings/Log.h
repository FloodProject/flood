/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Log.h>

namespace Flood
{
    enum struct LogLevel;
    ref class Log;
    ref class Allocator;
    value struct LogEntry;
    ref class Timer;
    ref class Mutex;
    ref class Mutex;
    ref class Event1;

    public enum struct LogLevel
    {
        Info = 0,
        Warn = 1,
        Error = 2,
        Debug = 3,
        Assert = 4
    };

    delegate void LogFunction(Flood::LogEntry);

    public value struct LogEntry
    {
    };



    public ref class Log
    {
        property ::Log* NativePtr;

    public:
        Log(::Log* native);
        Log();
    };

    public ref class FloodLog
    {
    public:
        static void LogDestroy (Flood::Log^ _122);
        static void LogAddHandler (Flood::Log^ _123, System::Action<Flood::LogEntry>^ _124);
        static void LogRemoveHandler (Flood::Log^ _125, System::Action<Flood::LogEntry>^ _126);
        static void LogWrite (Flood::Log^ _127, Flood::LogEntry entry);
        static Flood::Log^ LogGetDefault ();
        static void LogSetDefault (Flood::Log^ _128);
        static void LogInfo (System::String^ msg);
        static void LogWarn (System::String^ msg);
        static void LogError (System::String^ msg);
        static void LogDebug (System::String^ msg);
        static void LogAssert (System::String^ msg);
    };
}

