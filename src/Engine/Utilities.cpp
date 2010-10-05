/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Utilities.h"

#ifdef VAPOR_COMPILER_MSVC
	#define __stricmp _stricmp
#else
	#define __stricmp stricmp
#endif

namespace vapor {

//-----------------------------------//

// Check for mode details about endianness-related tricks and issues:
// http://www.gamedev.net/reference/articles/article2091.asp

bool System::isLittleEndian()
{
	byte EndianTest[2] = { 1, 0 };
	short x;
	x = *(short *) EndianTest;

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

int String::compareInsensitive(const std::string& s1, const std::string& s2)
{
	return __stricmp(s1.c_str(), s2.c_str());
}

//-----------------------------------//

void String::fromFloat( char* str, float n, byte precision )
{
	sprintf( str, "%#.*f", precision, n );
}

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS

std::string String::fromWideString(const std::wstring &wstr)
{
    // Convert a Unicode string to an ASCII string
    std::string strTo;
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

std::wstring String::toWideString(const std::string &str)
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

std::string String::format(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	std::string fmt = format(str, args);

	va_end(args);

	return fmt;
}

//-----------------------------------//

std::string String::format(const char* str, va_list args)
{
	const int BUF_MAX_SIZE = 16384;
	char buf[BUF_MAX_SIZE];

	#ifdef VAPOR_COMPILER_MSVC
		int n = vsnprintf_s( buf, BUF_MAX_SIZE, _TRUNCATE, str, args );
	#else
		int n = vsnprintf( &fmt[0], fmt.size(), str, args );
	#endif

	assert( n >= 0 ); // check for output error
	assert( n < BUF_MAX_SIZE ); // check for truncation

	return std::string(buf);
}

//-----------------------------------//

std::vector<std::string>& String::split(const std::string& s, char delim, 
								std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    
	while(std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }
    
	return elems;
}

//-----------------------------------//

std::vector<std::string> String::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
	return String::split(s, delim, elems);
}

//-----------------------------------//

std::string String::toLowerCase(const std::string& str)
{
	std::string data(str);
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	
	return data;
}

//-----------------------------------//

std::string String::toUpperCase(const std::string& str)
{
	std::string data(str);
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
	
	return data;
}

//-----------------------------------//

} // end namespace