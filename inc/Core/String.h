/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <string>

BEGIN_NAMESPACE_EXTERN

//-----------------------------------//

typedef std::string String;
typedef std::wstring WideString;

// Compares a string in a case insensitive way.
CORE_API int StringCompareInsensitive(const String& s1, const String& s2);

// Formats the string according to the 'printf' style format string.
CORE_API String StringFormat(const char* s, ...);
CORE_API String StringFormatArgs(const char* s, va_list args);

// Splits a string (tokenizes) into sub-string separated by given elements.
CORE_API void StringSplit(const String& s, char delim, std::vector<String>& elems);

// Converts a wide string in UTF-16 to a UTF-8 string.
CORE_API String StringFromWideString(const WideString& ws);

// Converts an UTF-8 encoded string to a UTF-16 wide string.
CORE_API WideString StringToWideString(const String& s);

// Converts between caseness of the string.
CORE_API String StringToLowerCase(const String& s);
CORE_API String StringToUpperCase(const String& s);

// Replaces a match in the string by another string.
CORE_API void StringReplace(String& s, const String& from, const String& to);
	
// Converts a float to a string.
CORE_API String StringFromFloat( float n, uint8 precision = 2 );

typedef String Path;

// Gets the base part of the filename string.
CORE_API Path PathGetFileBase(const Path& path);

// Gets the extension part of the filename string.
CORE_API Path PathGetFileExtension(const Path& path);

// Gets the base path part of the path string.
CORE_API Path PathGetBase(const Path& path);

// Gets the file part of the path string.
CORE_API Path PathGetFile(const Path& path);

// Normalizes the path.
CORE_API Path PathNormalize(const Path& path);

// Gets the current directory.
CORE_API Path PathGetCurrentDir();

// Gets the path separator.
CORE_API Path PathGetSeparator();

//-----------------------------------//

END_NAMESPACE_EXTERN
	
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