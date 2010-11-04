/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vapor/Log.h"
#include "vapor/Utilities.h"
#include "LogFormat.h"

namespace vapor {

//-----------------------------------//

static const int BUF_MAX_SIZE = 256;

Logger* Logger::globalLogger = nullptr;
bool Logger::showDebug = true;

//-----------------------------------//

std::string LogLevel::toString( LogLevel::Enum level )
{
	switch(level)
	{
	case LogLevel::Info:
		return "Info";
	case LogLevel::Warning:
		return "Warn";
	case LogLevel::Error:
		return "Error";
	case LogLevel::Debug:
		return "Debug";
	default:
		assert( 0 && "This should not be reached" );
		return "";
	}
}

//-----------------------------------//

void Log::debug( const std::string& msg )
{
	Log::debug( "%s", msg.c_str() );
}

//-----------------------------------//

void Log::debug(const char* msg, ...)
{
	if( !Logger::showDebug )
		return;

	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		log->write(LogLevel::Debug, msg, args);

	va_end(args);
}

//-----------------------------------//

void Log::info(const char* msg, ...)
{
	if( !Logger::getLogger() )
		return;

	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		log->write(LogLevel::Info, msg, args);
	
	va_end(args);
}

//-----------------------------------//

void Log::warn(const char* msg, ...)
{
	if( !Logger::getLogger() )
		return;

	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		log->write(LogLevel::Warning, msg, args);
	
		//std::string buf = String::format(msg, args);
		//Log::debug( buf );

	va_end(args);
}

//-----------------------------------//

void Log::error(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		log->write(LogLevel::Error, msg, args);
	
		std::string buf = String::format(msg, args);
		Log::debug( "%s", buf.c_str() );

	va_end(args);
}

//-----------------------------------//

void Log::messageDialog(const std::string& msg, LogLevel::Enum level)
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
	Log* log = getLogger();
	log->write(level, "MessageBox", msg.c_str() );
#else
	#error "Missing message box implementation"
#endif
}

//-----------------------------------//

Logger::Logger(const std::string& title, const std::string& fn)
	: NativeFile(fn, FileMode::Write), even(true)
{
	if( !open() ) 
	{
		std::string msg = String::format("Could not open log file '%s'", fn.c_str() );
		Log::messageDialog(msg);
		return;
	}

	// Turn off file buffering.
	setBuffering( false );
	
	writeHeader( title );

	Log::info("Creating log file '%s'", fn.c_str());

	if( !getLogger() )
		setLogger(this);
}

//-----------------------------------//

Logger::~Logger()
{
	writeFooter();

	if( globalLogger == this )
		globalLogger = nullptr;
}

//-----------------------------------//

void Logger::write(const LogLevel::Enum level, const char* msg, va_list args)
{
	std::string lvl = String::toLowerCase( LogLevel::toString(level) );

	LocaleSaveRestore c;

	THREAD(boost::lock_guard<boost::mutex> lock(mutex);)

	fprintf(fp, "\t\t<tr class=\"%s,%s\">", lvl.c_str(), even ? "even" : "odd");
		fprintf(fp, "<td class=\"%s\"></td>", lvl.c_str());
		fprintf(fp, "<td>%.3fs</td>", timer.getElapsedTime()); // date time
		fprintf(fp, "<td>");
			vfprintf(fp, msg, args);
		fprintf(fp, "</td>");
	fprintf(fp, "</tr>\n");

	fflush(fp);
	even = !even;

	// Show to standard output stream.
	printf("%s: ", lvl.c_str());
	vprintf(msg, args);
	printf("\n");

#ifdef VAPOR_COMPILER_MSVC
	if(level == LogLevel::Debug)
	{
		std::string fmt = String::format(msg, args);
		fmt.append("\n");
		OutputDebugStringA( fmt.c_str() );
	}
#endif
}

//-----------------------------------//

void Logger::writeHeader(const std::string& title)
{
	fprintf(fp, LOG_HTML, title.c_str(), LOG_CSS );
}

//-----------------------------------//

void Logger::writeFooter()
{
	fprintf(fp, "\t</table>\n" "\t</div>\n" "</body>\n" "</html>\n");
}

//-----------------------------------//

void Logger::info(const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Info, msg, args);
	va_end(args);
}

//-----------------------------------//

void Logger::warn(const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Warning, msg, args);
	va_end(args);
}

//-----------------------------------//

void Logger::error(const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Error, msg, args);
	va_end(args);
}

//-----------------------------------//

} // end namespace