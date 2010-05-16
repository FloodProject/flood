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

namespace vapor { namespace log {
		
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
 * Use this to issue debug messages.
 * Doesn't do anything on release builds.
 */

VAPOR_API void debug(const char* msg, ...);
VAPOR_API void debug( const std::string& msg );

//-----------------------------------//

/**
 * Convenience functions to log in the main engine stream.
 */

VAPOR_API void info(const std::string& subsystem, const char* msg, ...);
VAPOR_API void warn(const std::string& subsystem, const char* msg, ...);
VAPOR_API void error(const std::string& subsystem, const char* msg, ...);

//-----------------------------------//

/**
 * Logging class used to output useful logging and debugging information
 * to an output file format. Currently it outputs to HTML.
 */

class VAPOR_API Log : public NativeFile
{
public:
	
	Log(const std::string& title, const std::string& filename);
	~Log();
	
	// Logging methods for each message category.
	void info(const std::string& subsystem, const char* msg, ...);
	void warn(const std::string& subsystem, const char* msg, ...);
	void error(const std::string& subsystem, const char* msg, ...);

	// Low-level logging implementation.
	void write(const LogLevel::Enum level, const std::string& subsystem, 
		const char* msg, va_list args);

	// Gets/sets the global engine logger.
	IMPLEMENT_STATIC_ACESSOR(Logger, Log*, engineLog)

	// Spawns a new message box dialog.
	static void MessageDialog(const std::string& msg, 
		const LogLevel::Enum level = LogLevel::Warning);

	//-----------------------------------//

	// This controls if debug is output.
	static bool showDebug;

	// Global engine logger.
	static Log* engineLog;

protected:
		
	// Writes the header and footer to the log.
	void start(const std::string& title);
	void end();

	// Writes some code to allow table sorting.
	void sorttable();

	// Write CSS styling to the log.
	void css();

	// Timer used for data/time control
	Timer timer;

	// Used for zebra coloring the table.
	bool even;

#ifdef VAPOR_THREADING
	boost::mutex mut;
#endif
};

//-----------------------------------//

} } // end namespaces