/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"
#include "Core/Event.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct Timer;
struct Mutex;
struct Allocator;

enum_class LogLevel
{
	Info, Warn, Error, Debug, Assert
};

struct LogEntry
{
	float time;
	String message;
	LogLevel level;
};

typedef void (*LogFunction)(LogEntry*);

struct Log
{
	Timer* timer;
	Mutex* mutex;
	Event1<LogEntry*> handlers;
};

API_CORE Log* LogCreate(Allocator*);
API_CORE void LogDestroy(Log*);
API_CORE void LogAddHandler(Log*, LogFunction);
API_CORE void LogRemoveHandler(Log*, LogFunction);
API_CORE void LogWrite(Log*, LogEntry* entry);

API_CORE Log* LogGetDefault();
API_CORE void LogSetDefault(Log*);

API_CORE void LogInfo(const char* msg, ...);
API_CORE void LogWarn(const char* msg, ...);
API_CORE void LogError(const char* msg, ...);
API_CORE void LogDebug(const char* msg, ...);
API_CORE void LogAssert(const char* msg, ...);

//-----------------------------------//

NAMESPACE_EXTERN_END