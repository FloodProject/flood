/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"
#include "Event.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct Timer;
struct Mutex;
struct MemoryAllocator;

namespace LogLevel
{
	enum Enum { Info, Warn, Error, Debug, Assert };
};

struct LogEntry
{
	float time;
	String message;
	LogLevel::Enum level;
};

typedef void (*LogFunction)(LogEntry*);

struct Log
{
	Timer* Timer;
	Mutex* Mutex;
	Event1<LogEntry*> Handlers;
};

API_CORE Log* LogCreate(MemoryAllocator*);
API_CORE void LogDestroy(Log*, MemoryAllocator*);
API_CORE void LogAddHandler(Log*, LogFunction);
API_CORE void LogRemoveHandler(Log* log, LogFunction);
API_CORE void LogWrite(Log*, LogLevel::Enum level, const char* msg, va_list args);

API_CORE Log* LogGetDefault();
API_CORE void LogSetDefault(Log*);

API_CORE void LogInfo(const char* msg, ...);
API_CORE void LogWarn(const char* msg, ...);
API_CORE void LogError(const char* msg, ...);
API_CORE void LogDebug(const char* msg, ...);
API_CORE void LogAssert(const char* msg, ...);

//-----------------------------------//

NAMESPACE_EXTERN_END