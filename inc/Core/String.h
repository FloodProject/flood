/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <string>

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

typedef std::string String;
typedef std::wstring WideString;

// Compares a string in a case insensitive way.
API_CORE int StringCompareInsensitive(const String& s1, const String& s2);

// Formats the string according to the 'printf' style format string.
API_CORE String StringFormat(const char* s, ...);
API_CORE String StringFormatArgs(const char* s, va_list args);

// Splits a string (tokenizes) into sub-string separated by given elements.
API_CORE void StringSplit(const String& s, char delim, std::vector<String>& elems);

// Converts a wide string in UTF-16 to a UTF-8 string.
API_CORE String StringFromWideString(const WideString& ws);

// Converts an UTF-8 encoded string to a UTF-16 wide string.
API_CORE WideString StringToWideString(const String& s);

// Converts between caseness of the string.
API_CORE String StringToLowerCase(const String& s);
API_CORE String StringToUpperCase(const String& s);

// Replaces a match in the string by another string.
API_CORE void StringReplace(String& s, const String& from, const String& to);
	
// Strips the string of the given characters.
API_CORE String StringTrim(const String& s, const char* trim);

// Converts a float to a string.
API_CORE String StringFromFloat( float n, uint8 precision = 2 );

// Path utilities

typedef String Path;

// Gets the base part of the filename string.
API_CORE Path PathGetFileBase(const Path& path);

// Gets the extension part of the filename string.
API_CORE Path PathGetFileExtension(const Path& path);

// Gets the base path part of the path string.
API_CORE Path PathGetBase(const Path& path);

// Gets the file part of the path string.
API_CORE Path PathGetFile(const Path& path);

// Normalizes the path.
API_CORE Path PathNormalize(const Path& path);

// Gets the current directory.
API_CORE Path PathGetCurrentDir();

// Gets the path separator.
API_CORE Path PathGetSeparator();

//-----------------------------------//

EXTERN_END
	
// Converts a number to a string.
template< typename T >
vapor::String StringFromNumber(const T& t)
{
	std::ostringstream oss;
		
	oss << t;
	assert( !oss.fail() );
		
	return oss.str();
}

// Converts a string to a number.
template< typename T >
T StringToNumber(const vapor::String& s)
{
	std::stringstream ss(s);
		
	T num;
	ss >> num; 

	return num;
}

struct RawStringCompare
{
	bool operator()(char const* a, char const* b)
	{
		return std::strcmp(a, b) < 0;
	}
};

//-----------------------------------//

NAMESPACE_END