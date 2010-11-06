/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/NativeFile.h"
#include "vapor/Timer.h"

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
		Debug
	};

	std::string toString( LogLevel::Enum );
};

//-----------------------------------//

/**
 * Convenience logging functions.
 */

namespace Log
{
	// Logs a debug message to the global logger.
	VAPOR_API void debug(const char* msg, ...);
	
	// Logs a debug message to the global logger.
	VAPOR_API void debug(const std::string& msg);
	
	// Logs an info message to the global logger.
	VAPOR_API void info(const char* msg, ...);
	
	// Logs a warning message to the global logger.
	VAPOR_API void warn(const char* msg, ...);
	
	// Logs an error message to the global logger.
	VAPOR_API void error(const char* msg, ...);
	
	// Shows a message box dialog.
	VAPOR_API void messageDialog(const std::string& msg, LogLevel::Enum = LogLevel::Warning);
}

//-----------------------------------//

/**
 * Logging class used to log relevant information to a stream.
 */

class VAPOR_API Logger : public NativeFile
{
public:
	
	Logger(const std::string& title, const std::string& filename);
	~Logger();
	
	// Logs an information to the stream.
	void info(const char* msg, ...);

	// Logs a warning to the stream.
	void warn(const char* msg, ...);

	// Logs an error to the stream.
	void error(const char* msg, ...);

	// Low-level logging implementation.
	void write(const LogLevel::Enum level, const char* msg, va_list args);

	// Gets the global engine logger.
	static Logger* getLogger() { return globalLogger; }

	// Gets the global engine logger.
	static SETTER(Logger, Logger*, globalLogger)

	// This controls if debug is output.
	static bool showDebug;

	// Global engine logger.
	static Logger* globalLogger;

protected:
		
	// Writes the header to the log.
	void writeHeader(const std::string& title);
	
	// Writes the footer to the log.
	void writeFooter();

	// Keeps track of elapsed time.
	Timer timer;

	// Keeps track of zebra coloring the table.
	bool even;

	// Mutex lock to synchronize access.
	THREAD(boost::mutex mutex;)
};

//-----------------------------------//

} // end namespace