/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include <cstdarg>
#include <ctime>

#include "vapor/Log.h"
#include "vapor/Platform.h"

#include <ctime>

#include "LogFormat.h"

const int BUF_MAX_SIZE = 256;

namespace vapor {
	namespace log {

//-----------------------------------//

Log* Log::engineLog = nullptr;
bool Log::showDebug = true;

//-----------------------------------//

void debug( const std::string& msg )
{
	log::debug( "%s", msg.c_str() );
}

//-----------------------------------//

void debug(const char* str, ...)
{
	if( !Log::showDebug ) return;

#ifdef VAPOR_DEBUG
	char fmt[BUF_MAX_SIZE];

	va_list args;
	va_start(args, str);

#ifdef VAPOR_COMPILER_MSVC
	char msg[BUF_MAX_SIZE];
	
	_snprintf_s( fmt, BUF_MAX_SIZE, _TRUNCATE, "%s\n", str );
	vsnprintf_s( msg, BUF_MAX_SIZE, _TRUNCATE, fmt, args );

	// TODO: i18n issues?
	OutputDebugStringA( msg );
#else
	snprintf( fmt, BUF_MAX_SIZE, "%s\n", str );
	vfprintf( stdout, fmt, args );
#endif

	va_end(args);
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

Log::Log(const std::string& title, const std::string& filename)
: even( true ), fp( nullptr )
{
	if( !open(filename) ) 
	{
		MessageDialog("Could not open log file '" + filename + "'");
		return;
	}
	
	start( title );

	info("log", "Creating log file '%s'", filename.c_str());

	if( Log::getLogger() == nullptr )
		Log::setLogger( this );
}

//-----------------------------------//

Log::~Log()
{
	if( !fp ) return;

	end();
	close();

	if( engineLog == this )
		engineLog = nullptr;
}

//-----------------------------------//

Log* Log::getLogger()
{
	return engineLog;
}

//-----------------------------------//

void Log::setLogger(Log* log)
{
	//delete engineLog;
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
	#elif defined(VAPOR_PLATFORM_LINUX)
		getLogger()->write(level, "MessageBox", msg.c_str() );
	#else
		#error "Missing message box implementation"
	#endif
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

	const char* s = nullptr;

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

		fprintf(fp, "<td>%.3fs</td>", timer.getElapsedTime()); // date time
		fprintf(fp, "<td>%s</td>", subsystem.c_str()); // subsystem

		fprintf(fp, "<td>");
			vfprintf(fp, msg, args);
		fprintf(fp, "</td>");

	fprintf(fp, "</tr>\n");

	fflush(fp);

	even = !even;

#ifdef VAPOR_DEBUG
	switch(level)
	{
	case LogLevel::Warning:
	case LogLevel::Error:
		char buf[512];
		int err = vsprintf(buf, msg, args);
		assert(err >= 0);
		debug("%s",buf);
		break;
	}
#endif
}

//-----------------------------------//

void Log::write(const LogLevel::Enum level, const std::string& subsystem, 
	const char* msg, ...)
{
	if(!Log::getLogger()) return;

	switch(level)
	{
	case LogLevel::Info:
		info(subsystem, msg);
		break;
	case LogLevel::Warning:
		warn(subsystem, msg);
		break;
	case LogLevel::Error:
		error(subsystem, msg);
		break;
	}
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
