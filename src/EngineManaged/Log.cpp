/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Log.h"
#include "Memory.h"
#include "Concurrency.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Log::Log(::Log* native)
{
    NativePtr = native;
}

Flood::Log::Log()
{
}

void Flood::FloodLog::LogDestroy(Flood::Log^ _122)
{
}

void Flood::FloodLog::LogAddHandler(Flood::Log^ _123, System::Action<Flood::LogEntry>^ _124)
{
}

void Flood::FloodLog::LogRemoveHandler(Flood::Log^ _125, System::Action<Flood::LogEntry>^ _126)
{
}

void Flood::FloodLog::LogWrite(Flood::Log^ _127, Flood::LogEntry entry)
{
}

Flood::Log^ Flood::FloodLog::LogGetDefault()
{
    return nullptr;
}

void Flood::FloodLog::LogSetDefault(Flood::Log^ _128)
{
}

void Flood::FloodLog::LogInfo(System::String^ msg)
{
}

void Flood::FloodLog::LogWarn(System::String^ msg)
{
}

void Flood::FloodLog::LogError(System::String^ msg)
{
}

void Flood::FloodLog::LogDebug(System::String^ msg)
{
}

void Flood::FloodLog::LogAssert(System::String^ msg)
{
}


