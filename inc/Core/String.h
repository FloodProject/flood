/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include <string>
#include <sstream>
#include <cstdarg>
#include <cstring>
#include "Core/Containers/Vector.h"
#include "Core/Containers/str.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

//typedef std::string String;
//typedef std::wstring StringWide;

struct API_CORE StringHash
{
	StringHash();
	StringHash(const char* str, size_t size);

	uint32 hash;
};

// Hashes the string and returns its hash.
API_CORE StringHash HashString(const UTF8String&);

// Compares a string in a case insensitive way.
API_CORE int StringCompareInsensitive(const UTF8String& s1, const UTF8String& s2);

// Formats the string according to the 'printf' style format string.
API_CORE UTF8String StringFormat(const char* s, ...);
API_CORE UTF8String StringFormatArgs(const char* s, va_list args);

// Splits a string (tokenizes) into sub-string separated by given elements.
API_CORE void StringSplit(const UTF8String& s, char delim, Vector<UTF8String>& elems);

// Converts a wide string in UTF-16 to a UTF-8 string.
API_CORE UTF8String StringFromWideString(const WString& ws);

// Converts an UTF-8 encoded string to a UTF-16 wide string.
API_CORE WString StringToWideString(const UTF8String& s);

// Converts between caseness of the string.
API_CORE UTF8String StringToLowerCase(const UTF8String& s);
API_CORE UTF8String StringToUpperCase(const UTF8String& s);

// Replaces a match in the string by another string.
API_CORE void StringReplace(UTF8String& s, const UTF8String& from, const UTF8String& to);
	
// Strips the string of the given characters.
API_CORE UTF8String StringTrim(const UTF8String& s, const char* trim);

// Converts a float to a string.
API_CORE UTF8String StringFromFloat( float n, uint8 precision = 2 );

// Pattern matches a string.
API_CORE const char* StringMatch(const UTF8String& s, const UTF8String& pattern);
API_CORE const char* RawStringMatch(const char* s, size_t len, const char* p);

// Path utilities

typedef UTF8String Path;

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

// Combines two path elements.
API_CORE Path PathCombine(Path base, Path extra);

//-----------------------------------//

// Converts a number to a string.
template< typename T > FLD_IGNORE
NAMESPACE_CORE::UTF8String StringFromNumber(const T& t)
{
	return new UTF8String(t);
}

// Converts a string to a number.
template< typename T > FLD_IGNORE
T StringToNumber(const NAMESPACE_CORE::UTF8String& s)
{
	std::stringstream ss(s);
	
	T num;
	ss >> num; 

	return num;
}

struct FLD_IGNORE RawStringCompare
{
	bool operator()(char const* a, char const* b)
	{
		return std::strcmp(a, b) < 0;
	}
};

//-----------------------------------//

NAMESPACE_CORE_END