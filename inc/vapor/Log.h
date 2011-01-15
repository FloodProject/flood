/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "FileStream.h"
#include "Timer.h"
#include "Thread.h"

namespace vapor {

//-----------------------------------//

/**
 * Severity level of the log message.
 */

namespace LogLevel 
{
	enum Enum
	{
		Info,
		Warning,
		Error,
		Debug,
		Assert
	};

	std::string toString( LogLevel::Enum );
};

//-----------------------------------//

struct LogEntry
{
	// Time of the message.
	float time;

	// Formatted message.
	std::string message;

	// Level of the message.
	LogLevel::Enum level;
};

//-----------------------------------//

class LogSink
{
	DECLARE_UNCOPYABLE(LogSink)

public:

	LogSink() {}
	virtual ~LogSink() {}

	// Writes a log entry into the sink.
	virtual void process(LogEntry& entry) = 0;
};

//-----------------------------------//

/**
 * Logging class used to log relevant information to a stream.
 */

class CORE_API Logger
{
public:
	
	Logger();
	~Logger();
	
	// Logs an information message.
	void info(const char* msg, ...);

	// Logs a warning message.
	void warn(const char* msg, ...);

	// Logs an error message.
	void error(const char* msg, ...);

	// Writes a message of the given level.
	void write(LogLevel::Enum level, const char* msg, va_list args);

	// Adds a new sink to the logger.
	void add( LogSink* sink );

protected:

	// Keeps track of elapsed time.
	Timer timer;

	// Keeps track of last message.
	std::string last;

	// Mutex lock to synchronize access.
	THREAD(boost::mutex mutex;)

	// Log sinks.
	std::vector<LogSink*> sinks;
};

// Gets the main log instance.
Logger* GetLogger();

//-----------------------------------//

/**
 * Convenience logging functions.
 */

namespace Log
{
	// Logs an assert message to the global logger.
	CORE_API void _assert(const char* msg, ...);

	// Logs a debug message to the global logger.
	CORE_API void debug(const std::string& msg);

	// Logs a debug message to the global logger.
	CORE_API void debug(const char* msg, ...);
	
	// Logs an info message to the global logger.
	CORE_API void info(const char* msg, ...);
	
	// Logs a warning message to the global logger.
	CORE_API void warn(const char* msg, ...);
	
	// Logs an error message to the global logger.
	CORE_API void error(const char* msg, ...);
}

#define assert(expr)     \
	(void)( (!!(expr)) || \
	(Log::_assert("%s (%s:%d)", #expr, __FUNCTION__, __LINE__), 0) )

//-----------------------------------//

class LogSinkConsole : public LogSink
{
public:

	void process(LogEntry& entry);
};

//-----------------------------------//

class LogSinkHTML : public LogSink
{
public:

	LogSinkHTML(FileStream& stream);
	~LogSinkHTML();

	void process(LogEntry& entry);

protected:

	Stream& stream;
	FILE* fp;
	bool even;
};

//-----------------------------------//

} // end namespace