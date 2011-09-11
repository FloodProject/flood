/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Timer.h"
#include "Core/Utilities.h"

#include <cstdio>

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	
	#undef NOMINMAX
	#define NOMINMAX
	
	#include <Windows.h>	
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static const int BUF_MAX_SIZE = 256;

static Log* gs_Log;

Log* LogGetDefault() { return gs_Log; }
void LogSetDefault(Log* log) { gs_Log = log; }

//-----------------------------------//

static void LogConsoleHandler(LogEntry* entry)
{
	puts( entry->message.c_str() );
}

//-----------------------------------//

Log* LogCreate(Allocator* alloc)
{
	Log* log = Allocate(Log, alloc);
	
	log->timer = TimerCreate(alloc);
	log->mutex = MutexCreate(alloc);

	LogAddHandler(log, LogConsoleHandler);
	
	if( !gs_Log ) LogSetDefault(log);

	return log;
}

//-----------------------------------//

void LogDestroy(Log* log)
{
	if( !log ) return;
	if( gs_Log == log ) LogSetDefault(nullptr);
	
	TimerDestroy(log->timer);
	MutexDestroy(log->mutex);

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
	//MutexLock(log->mutex);
	log->handlers(entry);
	//MutexUnlock(log->mutex);
}

//-----------------------------------//

static void LogFormat(LogEntry* entry, /*Log* log,*/ LogLevel level, const char* msg, va_list args)
{
	String format = StringFormatArgs(msg, args);
	
	//entry->time = TimerGetElapsed(log->timer);
	entry->message = format;
	entry->level = level;
}

//-----------------------------------//

#ifdef COMPILER_MSVC
	#define PrintDebug OutputDebugStringA
#else
	#define PrintDebug printf
#endif

#ifdef BUILD_DEBUG

#define ASSERT_ON_LOG                                       \
		if(entry.level == LogLevel::Assert)                 \
		{                                                   \
			assert(false);                                  \
		}                                                   \

#else

#define ASSERT_ON_LOG

#endif

#define DEFINE_LOG_HELPER(Level)                            \
	void Log##Level(const char* msg, ...)                   \
	{                                                       \
		va_list args;                                       \
		va_start(args, msg);                                \
	                                                        \
		LogEntry entry;                                     \
		LogFormat(&entry, LogLevel::Level, msg, args);      \
		                                                    \
		Log* log = LogGetDefault();                         \
		if(log) LogWrite(log, &entry);                      \
		else puts(entry.message.c_str());                   \
                                                            \
		va_end(args);                                       \
                                                            \
		if(entry.level == LogLevel::Debug)                  \
		{                                                   \
			String debug = entry.message + "\n";            \
			PrintDebug( debug.c_str() );                    \
		}                                                   \
		ASSERT_ON_LOG                                       \
}                                                           \

DEFINE_LOG_HELPER(Info)
DEFINE_LOG_HELPER(Warn)
DEFINE_LOG_HELPER(Error)
DEFINE_LOG_HELPER(Debug)
DEFINE_LOG_HELPER(Assert)

//-----------------------------------//

NAMESPACE_CORE_END