/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Types.h>

namespace vapor {
	namespace log {

/**
 * Convenience functions to log in the main engine stream.
 */

void info(const string& subsystem, const char* msg, ...);
void warn(const string& subsystem, const char* msg, ...);
void error(const string& subsystem, const char* msg, ...);

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

/**
 * Logging class used to output useful logging and debugging information
 * to an output file format. Currently it outputs to XHTML.
 */

class Log
{
public:
	
	Log(const string& title, const string& filename);
	~Log();

	// Gets or sets the global engine logger
	static Log* getLogger();
	static void setLogger(Log* log);

	// Spawns a new message box dialog
	static void MessageDialog(const string& msg, const LogLevel::Enum level = LogLevel::Warning);

	// Logging functions
	void info(const string& subsystem, const char* msg, ...);
	void warn(const string& subsystem, const char* msg, ...);
	void error(const string& subsystem, const char* msg, ...);

	// Message logging functions
	void write(const LogLevel::Enum level, const string& subsystem, 
		const char* msg, va_list args);

protected:

	// Opens/closes a log file
	bool open(const string& filename);
	void close(void);
	
	// Writes JavaScript sorttable_v1.js
	void sorttable();

	// Write CSS styling to the log file
	void css();
	
	// Writes the boilerplate HTML tags
	void start(const string& title);
	void end();

	// Global engine logger
	static Log* engineLog;

	// File used to output logging information
	FILE* fp;

	// Used for zebra coloring the table
	bool even;
};

} } // end namespaces
