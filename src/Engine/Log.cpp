/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <cstdarg>
#include <ctime>

#include "vapor/Log.h"
#include "vapor/LogFormat.h"
#include "vapor/Platform.h"

namespace vapor {
	namespace log {

//-----------------------------------//

Log* Log::_log;

//-----------------------------------//

Log* Log::getLogger()
{
	return _log;
}

//-----------------------------------//

void Log::setLogger(Log* log)
{
	if(_log) delete _log;
	_log = log;
}

//-----------------------------------//

void info(const string& subsystem, const char* msg, ...)
{
	if(!Log::getLogger()) return;

	va_list args;
	
	va_start(args, msg);
		Log::getLogger()->write(LogLevel::Info, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void warn(const string &subsystem, const char* msg, ...)
{
	if(!Log::getLogger()) return;

	va_list args;
	
	va_start(args, msg);
		Log::getLogger()->write(LogLevel::Warning, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void error(const string& subsystem, const char* msg, ...)
{
	if(!Log::getLogger()) return;

	va_list args;
	
	va_start(args, msg);
		Log::getLogger()->write(LogLevel::Error, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

Log::Log(const string& title, const string& filename)
	: even(true)
{
	if(!open(filename)) {
		//error("vapor::log", "Could not open log file '%s'", filename.c_str());
		exit(EXIT_FAILURE);
	}
	
	start(title);

	info("log", "Creating log file '%s'", filename.c_str());
}

//-----------------------------------//

Log::~Log()
{
	end();
	close();
}

//-----------------------------------//

bool Log::open(const string& filename)
{
#ifdef VAPOR_PLATFORM_WINDOWS
	// disable Visual C++ fopen deprecation warning
	#pragma warning(disable : 4996)
#endif

	fp = fopen(filename.c_str(), "w+");
	if (!fp) return false;

	// turn off file buffering
	setbuf(fp, NULL);

	return true;
}

//-----------------------------------//

void Log::close()
{
	if (!fp) return;
	
	fclose(fp);
	fp = nullptr;
}

//-----------------------------------//

void Log::write(const LogLevel::Enum level, const string& subsystem, const char* msg, va_list args)
{
	char* s = nullptr;

	switch(level)
	{	
		case LogLevel::Info:	
			s = "info"; 
			break;
		case LogLevel::Warning:	
			s = "warn"; 
			break;
		case LogLevel::Error:	
			s = "error"; 
			break;
	}

	fprintf(fp, "\t\t<tr class=\"%s,%s\">", s, even ? "even" : "odd");

		fprintf(fp, "<td class=\"%s\"></td>", s);

		fprintf(fp, "<td>%d</td>", clock()); // date time
		fprintf(fp, "<td>%s</td>", subsystem.c_str()); // subsystem

		fprintf(fp, "<td>");
			vfprintf(fp, msg, args);
		fprintf(fp, "</td>");

	fprintf(fp, "</tr>\n");

	fflush(fp);

	even = !even;
}

//-----------------------------------//

void Log::start(const string& title)
{		
	fprintf(fp, LOG_HTML, title.c_str(), LOG_CSS, LOG_JS_TABLES);
}

//-----------------------------------//

void Log::end()
{
	fprintf(fp, "\t</table>\n" "\t</div>\n" "</body>\n" "</html>\n");
}

//-----------------------------------//

void Log::info(const string& subsystem, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Info, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void Log::warn(const string& subsystem, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Warning, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void Log::error(const string& subsystem, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Error, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

} } // end namespaces
