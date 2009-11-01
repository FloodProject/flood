/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Types.h"
#include "vapor/Timer.h"
#include "vapor/NativeFile.h"

namespace vapor {
	namespace log {
		
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
 * to an output file format. Currently it outputs to XHTML.
 */

class VAPOR_API Log : private boost::noncopyable
{
public:
	
	Log(const std::string& title, const std::string& filename);
	~Log();

	// Gets or sets the global engine logger.
	static Log* getLogger();
	static void setLogger(Log* log);

	// Spawns a new message box dialog.
	static void MessageDialog(const std::string& msg, 
		const LogLevel::Enum level = LogLevel::Warning);
	
	// Logging methods for each message category.
	void info(const std::string& subsystem, const char* msg, ...);
	void warn(const std::string& subsystem, const char* msg, ...);
	void error(const std::string& subsystem, const char* msg, ...);

	// Low-level logging implementation.
	void write(const LogLevel::Enum level, const std::string& subsystem, 
		const char* msg, va_list args);
		
	void write(const LogLevel::Enum, const std::string&, const char*, ...);

	static bool showDebug;

protected:

	// Opens/closes a log file.
	bool open(const std::string& filename);
	void close(void);
	
	// Writes JavaScript sorttable_v1.js.
	void sorttable();

	// Write CSS styling to the log file.
	void css();
	
	// Writes the boilerplate HTML tags.
	void start(const std::string& title);
	void end();

	// Global engine logger.
	static Log* engineLog;

	// File used to output logging information.
	FILE* fp;

	// Timer used for data/time control
	Timer timer;

	// Used for zebra coloring the table.
	bool even;
};

//-----------------------------------//

} } // end namespaces
