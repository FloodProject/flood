/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <sstream>

namespace vapor {

//---------------------------------------------------------------------//
// System Information
//---------------------------------------------------------------------//

namespace System
{
	// Returns if the system is little-endian.
	bool isLittleEndian();

	// Swaps the endianness of a long.
	long swapEndian(long i);

	// Shows a message box dialog.
	void messageDialog(const std::string& msg/*, LogLevel::Enum = LogLevel::Warning*/);
	
	// Sleeps for a specified ammount of time.
	void sleep( float time );

	// Enumerates files in a given path.
	std::vector<std::string> enumerateFiles(const std::string& path);

	// Enumerates dirs in a given path.
	std::vector<std::string> enumerateDirs(const std::string& path);
}

//---------------------------------------------------------------------//
// Conversions
//---------------------------------------------------------------------//

namespace String
{
	// Compares a string in a case insensitive way.
	CORE_API int compareInsensitive(const std::string& s1, const std::string& s2);

	// Formats the string according to the 'printf' style format string.
	// Note: This is not type-safe so be careful.
	CORE_API std::string format(const char* str, ...);
	CORE_API std::string format(const char* str, va_list args);

	// Splits a string (tokenizes) into sub-string separated by given elements.
	CORE_API std::vector<std::string>& split(const std::string& s, char delim, 
								std::vector<std::string>& elems);

	// Splits a string (tokenizes) into sub-string separated by given elements.
	CORE_API std::vector<std::string> split(const std::string& s, char delim);

	// Converts a wide string to a regular string.
	CORE_API std::string fromWideString(const std::wstring& wstr);
	
	// Converts a regular string into a wide string.
	CORE_API std::wstring toWideString(const std::string& str);

	// Converts a regular string to lower case.
	CORE_API std::string toLowerCase(const std::string& str);

	// Converts a regular string to upper case.
	CORE_API std::string toUpperCase(const std::string& str);

	// Replaces a sub-string by another sub-string in the source string.
	CORE_API void replace(std::string& source, const std::string& from, const std::string& to);

	// Converts a number to a string.
	template< typename T >
	std::string fromNumber(const T& t)
	{
		std::ostringstream oss;
		
		oss << t;
		assert( !oss.fail() );
		
		return oss.str();
	}

	// Converts a string to a number.
	template< typename T >
	T toNumber(const std::string& s)
	{
		std::stringstream ss(s);
		
		T num;
		ss >> num; 

		return num;
	}

	// Converts a float to a string.
	CORE_API std::string fromFloat( float n, byte precision = 2 );
}

//---------------------------------------------------------------------//
// Path manipulation
//---------------------------------------------------------------------//

typedef std::string Path;

namespace PathUtils
{
	// Gets the base part of the filename string.
	CORE_API Path getBase(const Path& path);

	// Gets the extension part of the filename string.
	CORE_API Path getExtension(const Path& path);

	// Gets the base path part of the path string.
	CORE_API Path getBasePath(const Path& path);

	// Gets the file part of the path string.
	CORE_API Path getFile(const Path& path);

	// Normalizes the path.
	CORE_API Path normalize(const Path& path);

	// Gets the current directory.
	CORE_API Path getCurrentDir();

	// Gets the path separator.
	CORE_API Path getPathSeparator();
};


//---------------------------------------------------------------------//
// Locales
//---------------------------------------------------------------------//

struct LocaleSwitch
{
public:

	LocaleSwitch() { c = setlocale(LC_NUMERIC, "C"); }
	~LocaleSwitch() { setlocale(LC_NUMERIC, c); }

private:

	char* c;
};

//-----------------------------------//

} // end namespace