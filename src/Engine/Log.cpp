/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Log.h"
#include "vapor/Utilities.h"
#include "LogFormat.h"

namespace vapor {

//-----------------------------------//

static const int BUF_MAX_SIZE = 256;

Log* Log::engineLog = nullptr;
bool Log::showDebug = true;

//-----------------------------------//

std::string LogLevel::toString( LogLevel::Enum level )
{
	switch(level)
	{
	case LogLevel::Info:
		return "Info";
	case LogLevel::Warning:
		return "Warning";
	case LogLevel::Error:
		return "Error";
	default:
		assert( 0 && "This should not be reached" );
		return "(undefined)";
	}
}

//-----------------------------------//

void debug( const std::string& msg )
{
	debug( "%s", msg.c_str() );
}

//-----------------------------------//

void debug(const char* str, ...)
{
	if( !Log::showDebug )
		return;

	va_list args;
	va_start(args, str);

	std::string msg = String::format(str, args);
	msg.append("\n");

#ifdef VAPOR_COMPILER_MSVC
	// TODO: i18n
	OutputDebugStringA( msg.c_str() );
#else
	printf( "%s", msg.c_str() );
#endif

	va_end(args);
}

//-----------------------------------//

void info(const std::string& subsystem, const char* msg, ...)
{
	if( !Log::getLogger() )
		return;

	va_list args;
	va_start(args, msg);

		Log* const log = Log::getLogger();
		log->write(LogLevel::Info, subsystem, msg, args);
	
	va_end(args);
}

//-----------------------------------//

void warn(const std::string &subsystem, const char* msg, ...)
{
	if( !Log::getLogger() )
		return;

	va_list args;
	va_start(args, msg);

		Log* const log = Log::getLogger();
		log->write(LogLevel::Warning, subsystem, msg, args);
	
		std::string buf = String::format(msg, args);
		debug( "%s", buf.c_str() );

	va_end(args);
}

//-----------------------------------//

void error(const std::string& subsystem, const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

		Log* const log = Log::getLogger();
		log->write(LogLevel::Error, subsystem, msg, args);
	
		std::string buf = String::format(msg, args);
		debug( "%s", buf.c_str() );

	va_end(args);
}

//-----------------------------------//

Log::Log(const std::string& title, const std::string& fn)
	: NativeFile(fn, AccessMode::Write), even(true)
{
	if( !open() ) 
	{
		std::string msg = String::format("Could not open log file '%s'", fn.c_str() );
		createMessageDialog(msg);
		return;
	}

	// Turn off file buffering.
	setBuffering( false );
	
	writeHeader( title );

	info("log", "Creating log file '%s'", fn.c_str());

	if( !getLogger() == nullptr )
		setLogger( this );
}

//-----------------------------------//

Log::~Log()
{
	writeFooter();

	if( engineLog == this )
		engineLog = nullptr;
}

//-----------------------------------//

void Log::createMessageDialog(const std::string& msg, const LogLevel::Enum level)
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

void Log::write(const LogLevel::Enum level, const std::string& subsystem, 
				const char* msg, va_list args)
{
	assert( fp != nullptr );

	const char* s = String::toLowerCase( LogLevel::toString(level) ).c_str();

	LocaleSaveRestore c;

#ifdef VAPOR_THREADING
	boost::lock_guard<boost::mutex> lock(mut);
#endif

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
}

//-----------------------------------//

void Log::writeHeader(const std::string& title)
{
	assert( fp != nullptr );

	fprintf(fp, LOG_HTML, title.c_str(), LOG_CSS );
}

//-----------------------------------//

void Log::writeFooter()
{
	assert( fp != nullptr );

	fprintf(fp, "\t</table>\n" "\t</div>\n" "</body>\n" "</html>\n");
}

//-----------------------------------//

void Log::info(const std::string& subsystem, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Info, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void Log::warn(const std::string& subsystem, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Warning, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

void Log::error(const std::string& subsystem, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
		write(LogLevel::Error, subsystem, msg, args);
	va_end(args);
}

//-----------------------------------//

} // end namespace