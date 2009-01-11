/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include <cstdarg>

#include "vapor/log/Log.h"
#include "vapor/log/LogFormat.h"

namespace vapor {
	namespace log {

Log * _log = new Log("vaporEngine Log", "log.html");

Log::Log(const string &title, const string &filename)
{
	if(!open(filename))
		exit(EXIT_FAILURE);

	even = true;

	start(title);

	info("log", "Creating log file '%s'", filename.c_str());
}

Log::~Log()
{
	end();
	close();
}

bool Log::open(const string &filename)
{
	// disable Visual C++ fopen deprecation warning
	#pragma warning(disable : 4996)

	if (!(fp = fopen(filename.c_str(), "w+")))
		return false;

	// turn off file buffering
	setbuf(fp, NULL);

	return true;
}

void Log::close()
{
	if (!fp) return;
	
	fclose(fp);
	fp = NULL;
}

void Log::write(const LogLevel level, const string &subsystem, const char* msg, va_list args)
{
	fprintf(fp, "\t<tr class=\"%s\">", even ? "even" : "odd");

	switch(level) {	
		case Info:		fprintf(fp, "<td class=\"%s\"></td>", "info");	break;
		case Warning:	fprintf(fp, "<td class=\"%s\"></td>", "warn");	break;
		case Error:		fprintf(fp, "<td class=\"%s\"></td>", "error");	break;
	}

	fprintf(fp, "<td>%s</td>", ""); // date time
	fprintf(fp, "<td>%s</td>", subsystem.c_str()); // subsystem

	fprintf(fp, "<td>");
	vfprintf(fp, msg, args);
	fprintf(fp, "</td>");

	fprintf(fp, "</tr>\n");

	fflush(fp);

	even = !even;
}

void Log::start(const string &title)
{		
	fprintf(fp, LOG_HTML, title.c_str(), LOG_CSS, LOG_JS_TABLES);
}

void Log::end()
{
	fprintf(fp, "</table>\n" "</body>\n" "</html>\n");
}

void Log::info(const string &subsystem, const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	write(Info, subsystem, msg, args);
	va_end(args);
}

void Log::warn(const string &subsystem, const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	write(Warning, subsystem, msg, args);
	va_end(args);
}

void Log::error(const string &subsystem, const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	write(Error, subsystem, msg, args);
	va_end(args);
}

Log* Log::getLogger()
{
	return _log;
}

} } // end namespaces
