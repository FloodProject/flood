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
 * Convenience functions to log in the main engine stream.
 */

namespace Log
{
	VAPOR_API void debug(const char* msg, ...);
	VAPOR_API void debug(const std::string& msg);
	VAPOR_API void info(const char* msg, ...);
	VAPOR_API void warn(const char* msg, ...);
	VAPOR_API void error(const char* msg, ...);
}
		
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
		Error
	};

	std::string toString( LogLevel::Enum );
};

//-----------------------------------//

/**
 * Logging class used to output useful logging and debugging information
 * to an output file format. Currently it outputs to HTML.
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

	// Spawns a new message box dialog.
	static void createMessageDialog(const std::string& msg, 
		const LogLevel::Enum level = LogLevel::Warning);

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
	THREAD(boost::mutex lock;)

public:

	// Gets the global engine logger.
	static Logger* getLogger() { return engineLog; }

	// Gets the global engine logger.
	static SETTER(Logger, Logger*, engineLog)

	// This controls if debug is output.
	static bool showDebug;

	// Global engine logger.
	static Logger* engineLog;
};

//-----------------------------------//

} // end namespace