/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Utilities.h"

#include <sstream>
#include <algorithm>
#include <cassert>

#ifdef VAPOR_PLATFORM_WINDOWS

	#define WIN32_LEAN_AND_MEAN
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

namespace vapor {

//-----------------------------------//

// Check for mode details about endianness-related tricks and issues:
// http://www.gamedev.net/reference/articles/article2091.asp

bool System::isLittleEndian()
{
	byte endianTest[2] = { 1, 0 };
	short x;
	x = *(short *) endianTest;

	return x == 1; // Little Endian
}

//-----------------------------------//

long System::swapEndian(long i)
{
	unsigned char b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i>>8 )  & 255;
	b3 = ( i>>16 ) & 255;
	b4 = ( i>>24 ) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

//-----------------------------------//

int StringCompareInsensitive(const String& s1, const String& s2)
{
#ifdef VAPOR_PLATFORM_WINDOWS
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

#ifdef VAPOR_PLATFORM_WINDOWS

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

#ifdef VAPOR_PLATFORM_WINDOWS

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

	#ifdef VAPOR_COMPILER_MSVC
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
	return path.substr( 0, ch );
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
	// Check if it has a file extension.
	size_t ch = path.find_last_of("/");

	if( ch == String::npos ) 
		return path;

	// Return the file extension.
	return path.substr( 0, ch );
}

//-----------------------------------//

Path PathGetFile(const Path& path)
{
	// Check if it has a file extension.
	size_t ch = path.find_last_of("/");

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

	return norm;
}

//-----------------------------------//

Path PathGetCurrentDir()
{
	char buf[256];
	my_getcwd(buf, VAPOR_ARRAY_SIZE(buf));
	return String(buf);
}

//-----------------------------------//

Path PathGetSeparator()
{
#ifdef VAPOR_PLATFORM_WINDOWS
	return "\\";
#else
	return "/";
#endif
}

//-----------------------------------//

} // end namespace