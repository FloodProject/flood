/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Log.h"
#include "Core/Memory.h"
#include "Core/Concurrency.h"
#include "Timer.h"
#include "Utilities.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

namespace vapor {

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

Log* LogCreate(MemoryAllocator* mem)
{
	Log* log = Allocate<Log>(mem);
	
	log->Timer = TimerCreate(mem);
	log->Mutex = MutexCreate(mem);
	log->Handlers.clear();

	LogAddHandler(log, LogConsoleHandler);
	
	if( !gs_Log ) LogSetDefault(log);

	return log;
}

//-----------------------------------//

void LogDestroy(Log* log, MemoryAllocator* mem)
{
	if( gs_Log == log ) LogSetDefault(nullptr);
	
	TimerDestroy(log->Timer, mem);
	MutexDestroy(log->Mutex, mem);

	Deallocate<Log>(mem, log);
}

//-----------------------------------//

void LogAddHandler(Log* log, LogFunction fn)
{
	log->Handlers.push_back(fn);
}

//-----------------------------------//

void LogWrite(Log* log, LogLevel::Enum level, const char* msg, va_list args)
{
	String format = StringFormatArgs(msg, args);
	
	LogEntry entry;
	entry.time = TimerGetElapsed(log->Timer);
	entry.message = format;
	entry.level = level;

	MutexLock(log->Mutex);

	for( size_t i = 0; i < log->Handlers.size(); i++ )
	{
		const LogFunction& fn = log->Handlers[i];
		fn(&entry);
	}

	MutexUnlock(log->Mutex);
}

//-----------------------------------//

#define DEFINE_LOG_HELPER(Level)                            \
	void Log##Level(const char* msg, ...)                   \
	{                                                       \
	va_list args;                                           \
	va_start(args, msg);                                    \
	                                                        \
	Log* log = LogGetDefault();                       \
	if(log) LogWrite(log, LogLevel::Level, msg, args);   \
	                                                        \
	va_end(args);                                           \
}                                                           \

DEFINE_LOG_HELPER(Info)
DEFINE_LOG_HELPER(Warn)
DEFINE_LOG_HELPER(Error)
DEFINE_LOG_HELPER(Debug)
DEFINE_LOG_HELPER(Assert)

//-----------------------------------//

} // end namespace