/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Utilities.h"

#include <sstream>
#include <algorithm>
#include <cassert>
#include <cstdio>

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#undef  NOMINMAX
	#define NOMINMAX
	#include <Windows.h>

    #include <direct.h>
    #define my_getcwd _getcwd
	#define my_stricmp _stricmp
#else
	#include <unistd.h>
    #define my_getcwd getcwd
	#define my_stricmp stricmp
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

// Check for mode details about endianness-related tricks and issues:
// http://www.gamedev.net/reference/articles/article2091.asp

bool SystemIsLittleEndian()
{
	byte endianTest[2] = { 1, 0 };
	short x;
	x = *(short *) endianTest;

	return x == 1; // Little Endian
}

//-----------------------------------//

int32 SystemSwapEndian(int32 i)
{
	uint8 b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i>>8 )  & 255;
	b3 = ( i>>16 ) & 255;
	b4 = ( i>>24 ) & 255;

	return ((int32)b1 << 24) + ((int32)b2 << 16) + ((int32)b3 << 8) + b4;
}

//-----------------------------------//

void SystemSleep( int64 time )
{
#ifdef PLATFORM_WINDOWS
	::Sleep( static_cast<DWORD>(time) );
#else
	timespec param;
	param.tv_nsec = time*1000000000.0;
	nanosleep(&param, NULL);
#endif
}

//-----------------------------------//

int StringCompareInsensitive(const String& s1, const String& s2)
{
#ifdef PLATFORM_WINDOWS
	return my_stricmp(s1.c_str(), s2.c_str());
#else
	return strcasecmp(s1.c_str(), s2.c_str());
#endif
}

//-----------------------------------//

String StringFromFloat( float n, byte precision )
{
	return StringFormat("%#.*f", precision, n );
}

//-----------------------------------//

#ifdef PLATFORM_WINDOWS

String StringFromWideString(const std::wstring &wstr)
{
    // Convert a Unicode string to an ASCII string
    String strTo;
    char *szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo,
		(int)wstr.length(), nullptr, nullptr);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

#endif

//-----------------------------------//

#ifdef PLATFORM_WINDOWS

std::wstring StringToWideString(const String &str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo,
		(int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}

#endif

//-----------------------------------//

String StringFormat(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	String formatted = StringFormatArgs(str, args);

	va_end(args);

	return formatted;
}

//-----------------------------------//

String StringFormatArgs(const char* str, va_list args)
{
	const int BUF_MAX_SIZE = 16384;
	char buf[BUF_MAX_SIZE];

	#ifdef COMPILER_MSVC
		int n = vsnprintf_s( buf, BUF_MAX_SIZE, _TRUNCATE, str, args );
	#else
		int n = vsnprintf( buf, BUF_MAX_SIZE, str, args );
	#endif

	assert( n >= 0 ); // check for output error
	assert( n < BUF_MAX_SIZE ); // check for truncation

	return String(buf);
}

//-----------------------------------//

void StringSplit(const String& s, char delim, std::vector<String>& elems)
{
    std::stringstream ss(s);
    String item;
    
	while(std::getline(ss, item, delim)) 
        elems.push_back(item);
}

//-----------------------------------//

String StringTrim(const String& s, const char* trim)
{
	String::size_type first = s.find_first_not_of(trim);
	
	if( first == String::npos ) return "";

	return s.substr(first, s.find_last_not_of(trim) - first + 1);
}

//-----------------------------------//

String StringToLowerCase(const String& str)
{
	String data(str);
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	
	return data;
}

//-----------------------------------//

String StringToUpperCase(const String& str)
{
	String data(str);
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
	
	return data;
}

//-----------------------------------//

void StringReplace(String& source, const String& from, const String& to)
{
	String::size_type pos = 0;

	while(true)
	{
		pos = source.find(from, pos);
		
		if( pos == String::npos )
			return;
		
		source.replace( pos++, from.size(), to );
	}
}

//-----------------------------------//

Path PathGetFileBase(const Path& path)
{
	Path filePath = PathGetFile(path);

	// Check if it has a file extension.
	size_t ch = filePath.find_last_of(".");

	if( ch == String::npos ) return "";

	// Return the file extension.
	return filePath.substr( 0, ch );
}

//-----------------------------------//

Path PathGetFileExtension(const Path& path)
{
	// Check if it has a file extension.
	size_t ch = path.find_last_of(".");

	if( ch == String::npos ) 
		return "";

	// Return the file extension.
	return path.substr( ++ch );
}

//-----------------------------------//

Path PathGetBase(const Path& path)
{
	size_t ch = path.find_last_of("/");

	if( ch == String::npos )
		ch = path.find_last_of("\\");

	if( ch == String::npos )
		return path;

	// Return the file extension.
	return path.substr( 0, ++ch );
}

//-----------------------------------//

Path PathGetFile(const Path& path)
{
	size_t ch = path.find_last_of("/");

	if( ch == String::npos )
		ch = path.find_last_of("\\");

	if( ch == String::npos )
		return path;

	// Return the file extension.
	return path.substr( ++ch );
}

//-----------------------------------//

Path PathNormalize(const Path& path)
{
	Path norm = path;

	StringReplace(norm, "\\", "/");
	StringReplace(norm, "//", "/");
	StringReplace(norm, "../", "");
	StringReplace(norm, "./", "");
	StringReplace(norm, "/\\", "/");
	StringReplace(norm, "\\/", "/");

	return norm;
}

//-----------------------------------//

Path PathGetCurrentDir()
{
	char buf[256];
	my_getcwd(buf, ARRAY_SIZE(buf));
	return String(buf);
}

//-----------------------------------//

Path PathGetSeparator()
{
#ifdef PLATFORM_WINDOWS
	return "\\";
#else
	return "/";
#endif
}

//-----------------------------------//

Path PathCombine(Path base, Path extra)
{
	base = StringTrim(base, "\\");
	base = StringTrim(base, "/");

	extra = StringTrim(extra, "\\");
	extra = StringTrim(extra, "/");

	const Path& sep = PathGetSeparator();

	StringReplace(base, "\\", sep);
	StringReplace(base, "/", sep);

	StringReplace(extra, "\\", sep);
	StringReplace(extra, "/", sep);

	return base + PathGetSeparator() + extra;
}

//-----------------------------------//

NAMESPACE_CORE_END