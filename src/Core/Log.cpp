/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/String.h"
#include "Core/Timer.h"
#include "Core/Utilities.h"

#include <cstdio>

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#ifndef NOMINMAX
	#define NOMINMAX
	#endif
	#include <Windows.h>
#endif

#ifdef COMPILER_MSVC
	#define PrintDebug OutputDebugStringA
#else
	#define PrintDebug printf
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Log* gs_Log;

Log* LogGetDefault() { return gs_Log; }
void LogSetDefault(Log* log) { gs_Log = log; }

//-----------------------------------//

static void LogConsoleHandler(LogEntry* entry)
{
	puts( entry->message.c_str() );
}

//-----------------------------------//

Log::Log()
	: timer(nullptr)
{
	Allocator* alloc = AllocatorGetThis();

	timer = TimerCreate(alloc);

	LogAddHandler(this, LogConsoleHandler);

	if( !gs_Log ) LogSetDefault(this);
}

//-----------------------------------//

Log::~Log()
{
	if( gs_Log == this ) LogSetDefault(nullptr);

	TimerDestroy(timer);
}

//-----------------------------------//

Log* LogCreate(Allocator* alloc)
{
	Log* log = Allocate(alloc, Log);
	return log;
}

//-----------------------------------//

void LogDestroy(Log* log)
{
	if( !log ) return;
	Deallocate(log);
}

//-----------------------------------//

void LogAddHandler(Log* log, LogFunction fn)
{
	log->handlers.Connect(fn);
}

//-----------------------------------//

void LogRemoveHandler(Log* log, LogFunction fn)
{
	log->handlers.Disconnect(fn);
}

//-----------------------------------//

void LogWrite(Log* log, LogEntry* entry)
{
	log->mutex.lock();
	log->handlers(entry);
	log->mutex.unlock();
}

//-----------------------------------//

static const int BUF_MAX_SIZE = 1024;
static thread_local char g_LogBuffer[BUF_MAX_SIZE];

static void LogProcess(Log* log, const char* msg, va_list args, LogLevel level)
{
	LogEntry entry;
	entry.message = StringFormatArgs(msg, args);
	entry.level = level;

	if(log)
		LogWrite(log, &entry);
	else
		puts(entry.message.c_str());

	switch(level)
	{
	case LogLevel::Debug:
	{
		StringFormat(g_LogBuffer, "%s\n", entry.message.c_str());
		PrintDebug(g_LogBuffer);
		break;
	}
	case LogLevel::Assert:
		assert(false);
		break;
	case LogLevel::Info:
	case LogLevel::Warn:
	case LogLevel::Error:
		unreachable("Invalid log level");
	};
}

#define DEFINE_LOG_HELPER(Level) \
	void Log##Level(const char* msg, ...) \
	{ \
		va_list args; \
		va_start(args, msg); \
		LogProcess(LogGetDefault(), msg, args, LogLevel::Level); \
		va_end(args); \
	}

DEFINE_LOG_HELPER(Info)
DEFINE_LOG_HELPER(Warn)
DEFINE_LOG_HELPER(Error)
DEFINE_LOG_HELPER(Debug)
DEFINE_LOG_HELPER(Assert)

//-----------------------------------//

NAMESPACE_CORE_END