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
#include "vapor/Platform.h"

#include "LogFormat.h"

namespace vapor {
	namespace log {

//-----------------------------------//

Log* Log::engineLog;

//-----------------------------------//

Log::Log(const std::string& title, const std::string& filename)
	: even(true), fp(nullptr)
{
	if( !open(filename) ) 
	{
		MessageDialog("Could not open log file '" + filename + "'");
		return;
	}
	
	start( title );

	info("log", "Creating log file '%s'", filename.c_str());
}

//-----------------------------------//

Log::~Log()
{
	if( !fp ) return;

	end();
	close();
}

//-----------------------------------//

Log* Log::getLogger()
{
	return engineLog;
}

//-----------------------------------//

void Log::setLogger(Log* log)
{
	delete engineLog;
	engineLog = log;
}

//-----------------------------------//

void Log::MessageDialog(const std::string& msg, const LogLevel::Enum level)
{
	#ifdef VAPOR_PLATFORM_WINDOWS
		UINT style = MB_OK;

		switch(level)
		{		
		case LogLevel::Info:	
			style |= MB_ICONINFORMATION; 
			break;
		case LogLevel::Warning:	
			style |= MB_ICONWARNING; 
			break;
		case LogLevel::Error:	
			style |= MB_ICONERROR; 
			break;
		}

		MessageBoxA(nullptr, msg.c_str(), nullptr, style);
	#else
		#error "Missing message box implementation"
	#endif
}

//-----------------------------------//

void info(const std::string& subsystem, const char* msg, ...)
{
	if(!Log::getLogger()) return;

	va_list args;
	
	va_start(args, msg);
		Log::getLogger()->write(LogLevel::Info, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void warn(const std::string &subsystem, const char* msg, ...)
{
	if(!Log::getLogger()) return;

	va_list args;
	
	va_start(args, msg);
		Log::getLogger()->write(LogLevel::Warning, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void error(const std::string& subsystem, const char* msg, ...)
{
	if(!Log::getLogger()) return;

	va_list args;
	
	va_start(args, msg);
		Log::getLogger()->write(LogLevel::Error, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

bool Log::open(const std::string& filename)
{
	#ifdef VAPOR_COMPILER_MSVC
		// disable Visual C++ fopen deprecation warning
		#pragma warning(disable : 4996)
	#endif

	fp = fopen(filename.c_str(), "w+");
	
	if ( !fp ) return false;

	// turn off file buffering
	setbuf(fp, nullptr);

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

void Log::write(const LogLevel::Enum level, const std::string& subsystem, 
				const char* msg, va_list args)
{
	if (!fp) return;

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

void Log::start(const std::string& title)
{
	if (!fp) return;

	fprintf(fp, LOG_HTML, title.c_str(), LOG_CSS, LOG_JS_TABLES);
}

//-----------------------------------//

void Log::end()
{
	if (!fp) return;

	fprintf(fp, "\t</table>\n" "\t</div>\n" "</body>\n" "</html>\n");
}

//-----------------------------------//

void Log::info(const std::string& subsystem, const char* msg, ...)
{
	if (!fp) return;

	va_list args;

	va_start(args, msg);
		write(LogLevel::Info, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void Log::warn(const std::string& subsystem, const char* msg, ...)
{
	if (!fp) return;

	va_list args;

	va_start(args, msg);
		write(LogLevel::Warning, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void Log::error(const std::string& subsystem, const char* msg, ...)
{
	if (!fp) return;

	va_list args;

	va_start(args, msg);
		write(LogLevel::Error, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

} } // end namespaces
