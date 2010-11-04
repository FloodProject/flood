/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

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
	VAPOR_API int compareInsensitive(const std::string& s1, const std::string& s2);

	// Formats the string according to the 'printf' style format string.
	// Note: This is not type-safe so be careful.
	VAPOR_API std::string format(const char* str, ...);
	VAPOR_API std::string format(const char* str, va_list args);

	// Splits a string (tokenizes) into sub-string separated by given elements.
	VAPOR_API std::vector<std::string>& split(const std::string& s, char delim, 
								std::vector<std::string>& elems);

	// Splits a string (tokenizes) into sub-string separated by given elements.
	VAPOR_API std::vector<std::string> split(const std::string& s, char delim);

	// Converts a wide string to a regular string.
	VAPOR_API std::string fromWideString(const std::wstring& wstr);
	
	// Converts a regular string into a wide string.
	VAPOR_API std::wstring toWideString(const std::string& str);

	// Converts a regular string to lower case.
	VAPOR_API std::string toLowerCase(const std::string& str);

	// Converts a regular string to upper case.
	VAPOR_API std::string toUpperCase(const std::string& str);

	// Replaces a sub-string by another sub-string in the source string.
	VAPOR_API void replace(std::string& source, const std::string& from, const std::string& to);

	// Gets the base part of the filename string.
	VAPOR_API std::string getBaseFromPath(const std::string& name);

	// Gets the extension part of the filename string.
	VAPOR_API std::string getExtensionFromPath(const std::string& name);

	// Gets the file part of the path string.
	VAPOR_API std::string getFileFromPath(const std::string& name);

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
		assert( !ss.fail() );

		return num;
	}

	// Converts a float to a string.
	VAPOR_API std::string fromFloat( float n, byte precision = 2 );
}

//---------------------------------------------------------------------//
// Locales
//---------------------------------------------------------------------//

struct LocaleSaveRestore
{
public:

	LocaleSaveRestore()
	{
		cur = setlocale(LC_NUMERIC, "C");
	}

	~LocaleSaveRestore()
	{
		setlocale(LC_NUMERIC, cur);
	}

private:

	char* cur;
};

//-----------------------------------//

} // end namespace