/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include <vapor/Platform.h>

namespace vapor {
	namespace log {

/**
 * Severity level of the log message
 */ 
enum LogLevel
{
	Info,
	Warning,
	Error
};

/**
 * Logging class used to output useful logging and debugging information
 * to an output file format. Currently it outputs to XHTML.
 */ 
class Log
{

public:
	
	Log(const string &title, const string &filename);
	~Log();

	// Logging functions
	void info(const string &subsystem, const char* msg, ...);
	void warn(const string &subsystem, const char* msg, ...);
	void error(const string &subsystem, const char* msg, ...);

	// Gets the global engine logger
	static Log* getLogger();

private:
	
	FILE *fp;

	// used for zebra coloring the table
	bool even;

	// Opens/closes a log file
	bool open(const string &filename);
	void close(void);
	
	// Writes JavaScript sorttable_v1.js
	void sorttable();

	// Write CSS styling to the log file
	void css();
	
	// Writes the boilerplate HTML tags
	void start(const string &title);
	void end();

	// Message logging functions
	void write(const LogLevel level, const string &subsystem, const char* msg, va_list args);
};

} } // end namespaces
