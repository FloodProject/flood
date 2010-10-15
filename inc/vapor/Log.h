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
 * Use this to issue debug messages.
 * Doesn't do anything on release builds.
 */

namespace Log
{
	VAPOR_API void debug(const char* msg, ...);
	VAPOR_API void debug(const std::string& msg);
}

//-----------------------------------//

/**
 * Convenience functions to log in the main engine stream.
 */

namespace Log
{
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

	// Gets an error string
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
	
	// Logging methods for each message category.
	void info(const char* msg, ...);
	void warn(const char* msg, ...);
	void error(const char* msg, ...);

	// Low-level logging implementation.
	void write(const LogLevel::Enum level, 
		const char* msg, va_list args);

	// Gets/sets the global engine logger.
	STATIC_ACESSOR(Logger, Logger*, engineLog)

	// Spawns a new message box dialog.
	static void createMessageDialog(const std::string& msg, 
		const LogLevel::Enum level = LogLevel::Warning);

	//-----------------------------------//

	// This controls if debug is output.
	static bool showDebug;

	// Global engine logger.
	static Logger* engineLog;

protected:
		
	// Writes the header and footer to the log.
	void writeHeader(const std::string& title);
	void writeFooter();

	// Timer used for data/time control
	Timer timer;

	// Used for zebra coloring the table.
	bool even;

#ifdef VAPOR_THREADING
	boost::mutex mut;
#endif
};

//-----------------------------------//

} // end namespace