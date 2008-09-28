/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// fuckEngine by triton
////
//// License: fuckGNU License
////
//// Changelog:
////	(19/08/08) Initial implementantion
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

using namespace std;

namespace vapor {
	namespace log {

// Severity level of the Log message
enum LogLevel
{
	Info,
	Warning,
	Error
};

// This class represents an HTML log file
class Log
{
private:
	FILE *fp;

	// Opens/closes a logfile
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

public:
	Log(const string &title, const string &filename);
	~Log();

	// Logging functions
	void info(const string &subsystem, const char* msg, ...);
	void warn(const string &subsystem, const char* msg, ...);
	void error(const string &subsystem, const char* msg, ...);
};

} } // end namespaces