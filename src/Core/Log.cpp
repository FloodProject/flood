/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "Log.h"
#include "Utilities.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

namespace vapor {

//-----------------------------------//

static const int BUF_MAX_SIZE = 256;
Logger* Logger::mainLogger = nullptr;

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
	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		if(log) log->write(LogLevel::Debug, msg, args);

	va_end(args);
}

//-----------------------------------//

void Log::info(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		if(log) log->write(LogLevel::Info, msg, args);
	
	va_end(args);
}

//-----------------------------------//

void Log::warn(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		if(log) log->write(LogLevel::Warning, msg, args);
	
	va_end(args);
}

//-----------------------------------//

void Log::error(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

		Logger* const log = Logger::getLogger();
		if(log) log->write(LogLevel::Error, msg, args);

	va_end(args);
}

//-----------------------------------//

Logger::Logger()
{
	add( new LogSinkConsole() );

	if( !getLogger() )
		setLogger(this);

	Log::info("Creating logger");
}

//-----------------------------------//

Logger::~Logger()
{
	for( uint i = 0; i < sinks.size(); i++ )
	{
		LogSink* sink = sinks[i];
		delete sink;
	}

	if( getLogger() == this )
		setLogger(nullptr);
}

//-----------------------------------//

void Logger::add( LogSink* sink )
{
	assert( sink != nullptr );
	sinks.push_back(sink);
}

//-----------------------------------//

void Logger::write(LogLevel::Enum level, const char* msg, va_list args)
{
	THREAD(lock_guard<boost::mutex> lock(mutex);)
	LocaleSwitch c;

	std::string format = String::format(msg, args);
	
	if(last == format)
		return;
	
	last = format;

	LogEntry entry;
	entry.time = (float) timer.getElapsedTime();
	entry.message = format;
	entry.level = level;

	for( uint i = 0; i < sinks.size(); i++ )
	{
		LogSink* sink = sinks[i];
		sink->process(entry);
	}
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

/**
 * HTML markup. We declare it with an HTML5 doctype for extra coolness!
 */

static const char LOG_HTML[] =

	"<!DOCTYPE html>\n"
		"<head>\n"
			"\t<meta charset='utf-8'>\n"
			"\t<title>%s</title>\n"
			"\t<style type='text/css'>\n" "%s" "\t</style>\n"
		"</head>\n"
		"<body>\n"
			"\t<div id='container'>\n"
				"\t<img id='header' src='head.png' alt=''/>\n"
				"\t<table>\n"
					"\t\t<thead><tr><th></th>"
						"<th>Time</th>"
						"<th>Description</th>"
					"</tr></thead>\n";

//-----------------------------------//

/**
 * CSS styling. This is always a pain in the ass to get working.
 * Only tested in Mozilla Firefox 3.x and Google Chrome.
 */

static const char LOG_CSS[] =

	"\t\t\t* { font: 10px sans-serif; }\n"
	"\t\t\tbody { text-align: center; margin: 0 auto; }\n"
	"\t\t\t#header { margin-top: 10px; margin-bottom:-3px; }\n"
	"\t\t\ttr:nth-child(even) { background-color: #eee; }\n"
	"\t\t\ttable { margin: 0 auto; table-layout: automatic; width: 600px; text-align: left; border: 1px solid black; }\n"
	"\t\t\tthead th { font-weight: bold; font-size: 1.2em; border-bottom: 1px dotted black; }\n"
	"\t\t\ttable tr.even { background-color: #eee; }\n"
	"\t\t\ttable tr.warn { background-color: #ff9; }\n"
	"\t\t\ttable tr.error { background-color: #ff9; }\n"
	"\t\t\ttable td.info { background-repeat: no-repeat; width: 16px; height: 16px; background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABGdBTUEAAK/INwWK6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAKcSURBVDjLpZPLa9RXHMU/d0ysZEwmMQqZiTaP0agoaKGJUiwIxU0hUjtUQaIuXHSVbRVc+R8ICj5WvrCldJquhVqalIbOohuZxjDVxDSP0RgzyST9zdzvvffrQkh8tBs9yy9fPhw45xhV5X1U8+Yhc3U0LcEdVxdOVq20OA0ooQjhpnfhzuDZTx6++m9edfDFlZGMtXKxI6HJnrZGGtauAWAhcgwVnnB/enkGo/25859l3wIcvpzP2EhuHNpWF9/dWs/UnKW4EOGDkqhbQyqxjsKzMgM/P1ymhlO5C4ezK4DeS/c7RdzQoa3x1PaWenJjJZwT9rQ1gSp/js1jYoZdyfX8M1/mp7uFaTR8mrt29FEMQILr62jQ1I5kA8OF59jIItVA78dJertTiBNs1ZKfLNG+MUHX1oaURtIHEAOw3p/Y197MWHEJEUGCxwfHj8MTZIcnsGKxzrIURYzPLnJgbxvG2hMrKdjItjbV11CYKeG8R7ygIdB3sBMFhkem0RAAQ3Fuka7UZtRHrasOqhYNilOwrkrwnhCU/ON5/q04vHV48ThxOCuoAbxnBQB+am65QnO8FqMxNCjBe14mpHhxBBGCWBLxD3iyWMaYMLUKsO7WYH6Stk1xCAGccmR/Ozs/bKJuXS39R/YgIjgROloSDA39Deit1SZWotsjD8pfp5ONqZ6uTfyWn+T7X0f59t5fqDhUA4ry0fYtjJcWeZQvTBu4/VqRuk9/l9Fy5cbnX+6Od26s58HjWWaflwkusKGxjm1bmhkvLXHvh1+WMbWncgPfZN+qcvex6xnUXkzvSiYP7EvTvH4toDxdqDD4+ygT+cKMMbH+3MCZ7H9uAaDnqytpVX8cDScJlRY0YIwpAjcNcuePgXP/P6Z30QuoP4J7WbYhuQAAAABJRU5ErkJggg=='); }\n"
	"\t\t\ttable td.warn { background-repeat: no-repeat; width: 16px; height: 16px; background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABGdBTUEAAK/INwWK6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAIsSURBVDjLpVNLSJQBEP7+h6uu62vLVAJDW1KQTMrINQ1vPQzq1GOpa9EppGOHLh0kCEKL7JBEhVCHihAsESyJiE4FWShGRmauu7KYiv6Pma+DGoFrBQ7MzGFmPr5vmDFIYj1mr1WYfrHPovA9VVOqbC7e/1rS9ZlrAVDYHig5WB0oPtBI0TNrUiC5yhP9jeF4X8NPcWfopoY48XT39PjjXeF0vWkZqOjd7LJYrmGasHPCCJbHwhS9/F8M4s8baid764Xi0Ilfp5voorpJfn2wwx/r3l77TwZUvR+qajXVn8PnvocYfXYH6k2ioOaCpaIdf11ivDcayyiMVudsOYqFb60gARJYHG9DbqQFmSVNjaO3K2NpAeK90ZCqtgcrjkP9aUCXp0moetDFEeRXnYCKXhm+uTW0CkBFu4JlxzZkFlbASz4CQGQVBFeEwZm8geyiMuRVntzsL3oXV+YMkvjRsydC1U+lhwZsWXgHb+oWVAEzIwvzyVlk5igsi7DymmHlHsFQR50rjl+981Jy1Fw6Gu0ObTtnU+cgs28AKgDiy+Awpj5OACBAhZ/qh2HOo6i+NeA73jUAML4/qWux8mt6NjW1w599CS9xb0mSEqQBEDAtwqALUmBaG5FV3oYPnTHMjAwetlWksyByaukxQg2wQ9FlccaK/OXA3/uAEUDp3rNIDQ1ctSk6kHh1/jRFoaL4M4snEMeD73gQx4M4PsT1IZ5AfYH68tZY7zv/ApRMY9mnuVMvAAAAAElFTkSuQmCC'); }\n"
	"\t\t\ttable td.error { background-repeat: no-repeat; width: 16px; height: 16px; background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABGdBTUEAAK/INwWK6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAJPSURBVDjLpZPLS5RhFMYfv9QJlelTQZwRb2OKlKuINuHGLlBEBEOLxAu46oL0F0QQFdWizUCrWnjBaDHgThCMoiKkhUONTqmjmDp2GZ0UnWbmfc/ztrC+GbM2dXbv4ZzfeQ7vefKMMfifyP89IbevNNCYdkN2kawkCZKfSPZTOGTf6Y/m1uflKlC3LvsNTWArr9BT2LAf+W73dn5jHclIBFZyfYWU3or7T4K7AJmbl/yG7EtX1BQXNTVCYgtgbAEAYHlqYHlrsTEVQWr63RZFuqsfDAcdQPrGRR/JF5nKGm9xUxMyr0YBAEXXHgIANq/3ADQobD2J9fAkNiMTMSFb9z8ambMAQER3JC1XttkYGGZXoyZEGyTHRuBuPgBTUu7VSnUAgAUAWutOV2MjZGkehgYUA6O5A0AlkAyRnotiX3MLlFKduYCqAtuGXpyH0XQmOj+TIURt51OzURTYZdBKV2UBSsOIcRp/TVTT4ewK6idECAihtUKOArWcjq/B8tQ6UkUR31+OYXP4sTOdisivrkMyHodWejlXwcC38Fvs8dY5xaIId89VlJy7ACpCNCFCuOp8+BJ6A631gANQSg1mVmOxxGQYRW2nHMha4B5WA3chsv22T5/B13AIicWZmNZ6cMchTXUe81Okzz54pLi0uQWp+TmkZqMwxsBV74Or3od4OISPr0e3SHa3PX0f3HXKofNH/UIG9pZ5PeUth+CyS2EMkEqs4fPEOBJLsyske48/+xD8oxcAYPzs4QaS7RR2kbLTTOTQieczfzfTv8QPldGvTGoF6/8AAAAASUVORK5CYII='); }\n";

//-----------------------------------//

LogSinkHTML::LogSinkHTML(FileStream& stream)
	: stream(stream)
	, even(true)
	, fp(nullptr)
{
	if( !stream.open() )
		return;

	fp = (FILE*) stream.getFilePointer();
	assert( fp != nullptr );

	stream.setBuffering( false );
	
	fprintf(fp, LOG_HTML, "Log", LOG_CSS );
}

//-----------------------------------//

LogSinkHTML::~LogSinkHTML()
{
	if(!fp) return;
	fprintf(fp, "\t</table>\n" "\t</div>\n" "</body>\n" "</html>\n");
}

//-----------------------------------//

void LogSinkHTML::process(LogEntry& entry)
{
	if(!fp) return;

	std::string level = LogLevel::toString(entry.level);
	level = String::toLowerCase(level);

	fprintf(fp, "\t\t<tr class=\"%s,%s\">", level.c_str(), even ? "even" : "odd");
		fprintf(fp, "<td class=\"%s\"></td>", level.c_str());
		fprintf(fp, "<td>%.3fs</td>", entry.time);
		fprintf(fp, "<td>");
			fprintf(fp, "%s", entry.message.c_str());
		fprintf(fp, "</td>");
	fprintf(fp, "</tr>\n");

	fflush(fp);
	even = !even;
}

//-----------------------------------//

void LogSinkConsole::process(LogEntry& entry)
{
	std::string level = LogLevel::toString(entry.level);
	level = String::toLowerCase(level);

	// Show to standard output stream.
	printf("%s: %s\n", level.c_str(), entry.message.c_str());

#ifdef VAPOR_COMPILER_MSVC
	if(entry.level == LogLevel::Debug)
	{
		std::string msg = String::format("%s\n", entry.message.c_str());
		OutputDebugStringA( msg.c_str() );
	}
#endif
}

//-----------------------------------//

} // end namespace