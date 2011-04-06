/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Concurrency.h"
#include "Core/Timer.h"
#include "Core/Utilities.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

NAMESPACE_BEGIN

//-----------------------------------//

static const int BUF_MAX_SIZE = 256;

static Log* gs_Log;

Log* LogGetDefault() { return gs_Log; }
void LogSetDefault(Log* log) { gs_Log = log; }

//-----------------------------------//

static void LogConsoleHandler(LogEntry* entry)
{
	puts( entry->message.c_str() );

#ifdef VAPOR_COMPILER_MSVC
	if(entry->level == LogLevel::Debug)
		OutputDebugStringA( entry->message.c_str() );
#endif
}

//-----------------------------------//

Log* LogCreate(Allocator* alloc)
{
	Log* log = Allocate<Log>(alloc);
	
	log->Timer = TimerCreate(alloc);
	log->Mutex = MutexCreate(alloc);

	LogAddHandler(log, LogConsoleHandler);
	
	if( !gs_Log ) LogSetDefault(log);

	return log;
}

//-----------------------------------//

void LogDestroy(Log* log, Allocator* alloc)
{
	if( !log ) return;
	if( gs_Log == log ) LogSetDefault(nullptr);
	
	TimerDestroy(log->Timer, alloc);
	MutexDestroy(log->Mutex, alloc);

	Deallocate<Log>(alloc, log);
}

//-----------------------------------//

void LogAddHandler(Log* log, LogFunction fn)
{
	log->Handlers.Connect(fn);
}

void LogRemoveHandler(Log* log, LogFunction fn)
{
	log->Handlers.Disconnect(fn);
}

//-----------------------------------//

void LogWrite(Log* log, LogEntry* entry)
{
	MutexLock(log->Mutex);
	log->Handlers(entry);
	MutexUnlock(log->Mutex);
}

//-----------------------------------//

static void LogFormat(LogEntry& entry, Log* log, LogLevel::Enum level, const char* msg, va_list args)
{
	String format = StringFormatArgs(msg, args);
	
	entry.time = TimerGetElapsed(log->Timer);
	entry.message = format;
	entry.level = level;
}

//-----------------------------------//

#define DEFINE_LOG_HELPER(Level)                            \
	void Log##Level(const char* msg, ...)                   \
	{                                                       \
		va_list args;                                       \
		va_start(args, msg);                                \
	                                                        \
		Log* log = LogGetDefault();                         \
		if(!log) return;                                    \
		                                                    \
		LogEntry entry;                                     \
		LogFormat(entry, log, LogLevel::Level, msg, args);  \
		LogWrite(log, &entry);                              \
	                                                        \
		va_end(args);                                       \
}                                                           \

DEFINE_LOG_HELPER(Info)
DEFINE_LOG_HELPER(Warn)
DEFINE_LOG_HELPER(Error)
DEFINE_LOG_HELPER(Debug)
DEFINE_LOG_HELPER(Assert)

//-----------------------------------//

NAMESPACE_END