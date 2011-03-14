/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"

BEGIN_NAMESPACE_EXTERN

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
	std::vector<LogFunction> Handlers;
};

CORE_API Log* LogCreate(MemoryAllocator*);
CORE_API void LogDestroy(Log*, MemoryAllocator*);
CORE_API void LogAddHandler(Log*, LogFunction);
CORE_API void LogRemoveHandler(LogFunction);
CORE_API void LogWrite(Log*, LogLevel::Enum level, const char* msg, va_list args);

CORE_API Log* LogGetDefault();
CORE_API void LogSetDefault(Log*);

CORE_API void LogInfo(const char* msg, ...);
CORE_API void LogWarn(const char* msg, ...);
CORE_API void LogError(const char* msg, ...);
CORE_API void LogDebug(const char* msg, ...);
CORE_API void LogAssert(const char* msg, ...);

//-----------------------------------//

END_NAMESPACE_EXTERN