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

#include <cstdarg>

#include "vapor/Log.h"
#include "vapor/LogExternalData.h"

namespace vapor {
	namespace log {

Log::Log(const string &title, const string &filename)
{
	open(filename);
	start(title);
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
	if(!fp)	return;

	fprintf(fp, "\t<tr>");

	fprintf(fp, "<td>%s</td>", ""); // date time
	fprintf(fp, "<td>%s</td>", subsystem.c_str()); // subsystem

	fprintf(fp, "<td>");
	vfprintf(fp, msg, args);
	fprintf(fp, "</td>");

	fprintf(fp, "</tr>\n");

	fflush(fp);
}

void Log::css()
{

	fprintf(fp, "\t<style>");

		fwrite(cs, sizeof(cs[0]), sizeof(cs)/sizeof(cs[0])-1, fp);

	fprintf(fp, "\t</style>\n");
}

void Log::sorttable()
{
	fprintf(fp, "\t<script type=\"text/javascript\">\n");

		fwrite(st, sizeof(st[0]), sizeof(st)/sizeof(st[0])-1, fp);
	
	fprintf(fp, "\t</script>\n");
}

void Log::start(const string &title)
{
	fprintf(fp,	"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"\n"
				"\t\"http://www.w3.org/TR/html4/strict.dtd\">\n");
	
	fprintf(fp, "<html>\n");

	fprintf(fp, "<head>\n");
		
		fprintf(fp, "\t<title>%s</title>\n", title.c_str());
	
		css();
		
		//sorttable();

	fprintf(fp, "</head>\n");

	fprintf(fp, "<body>\n");

	fprintf(fp, "<table>\n");
}

void Log::end()
{
	fprintf(fp, "</table>\n");

	fprintf(fp, "</body>\n");
	
	fprintf(fp, "</html>\n");
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

} } // end namespaces