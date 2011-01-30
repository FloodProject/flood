/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Utilities.h"
#include "Log.h"

#include <dirent.h>
#include <sstream>
#include <algorithm>

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

void System::sleep( double time )
{
#ifdef VAPOR_PLATFORM_WINDOWS
	::Sleep( static_cast<DWORD>(time) );
#else
	timespec param;
	param.tv_nsec = time*1000000000.0;
	nanosleep(&param, NULL);
#endif
}

//-----------------------------------//

void System::messageDialog(const std::string& msg/*, LogLevel::Enum level*/)
{
#ifdef VAPOR_PLATFORM_WINDOWS
	UINT style = MB_OK;

	//switch(level)
	//{
	//case LogLevel::Info:	
	//	style |= MB_ICONINFORMATION; 
	//	break;
	//case LogLevel::Warning:	
	//	style |= MB_ICONWARNING; 
	//	break;
	//case LogLevel::Error:	
	//	style |= MB_ICONERROR; 
	//	break;
	//}

	MessageBoxA(nullptr, msg.c_str(), nullptr, style);
#elif defined(VAPOR_PLATFORM_LINUX)
	Log* log = getLogger();
	log->write(level, "MessageBox", msg.c_str() );
#else
	#error "Missing message box implementation"
#endif
}

//-----------------------------------//

static void enumerateFilesHelper(std::vector<std::string>& files, std::string path, bool dirs)
{
	DIR *dir;
	struct dirent *ent;

	// Open directory stream.
	dir = opendir( path.c_str() );
    
	if( !dir )
	{
		Log::error("Could not open directory '%s'", path.c_str());
		goto end;
	}

	// Get all the files and directories within directory.
	while((ent = readdir(dir)) != nullptr)
	{
		std::string name(ent->d_name);
		std::string newPath = path + "/" + name;

		switch(ent->d_type)
		{
		case DT_REG:
		{
			if(!dirs)
				files.push_back(newPath);
			break;
		}
		case DT_DIR:
		{
			if(!name.empty() && name[0] == '.')
				continue;

			if(dirs)
				files.push_back(newPath);

			enumerateFilesHelper(files, newPath, dirs);

			break;
		}
		default:
			Log::debug("Unknown dir entry: %s", path.c_str());
		}
	}

end:
	closedir(dir);
}

//-----------------------------------//

std::vector<std::string> System::enumerateFiles(const std::string& path)
{
	std::vector<std::string> files;
	enumerateFilesHelper(files, path, false);

	return files;
}

//-----------------------------------//

std::vector<std::string> System::enumerateDirs(const std::string& path)
{
	std::vector<std::string> dirs;
	enumerateFilesHelper(dirs, path, true);

	return dirs;
}

//-----------------------------------//

int String::compareInsensitive(const std::string& s1, const std::string& s2)
{
	return __stricmp(s1.c_str(), s2.c_str());
}

//-----------------------------------//

std::string String::fromFloat( float n, byte precision )
{
	return String::format("%#.*f", precision, n );
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

void String::replace(std::string& source, const std::string& from, const std::string& to)
{
	std::string::size_type pos = 0;

	while(true)
	{
		pos = source.find(from, pos);
		
		if( pos == std::string::npos )
			return;
		
		source.replace( pos++, from.size(), to );
	}
}

//-----------------------------------//

Path PathUtils::getBase(const Path& path)
{
	// Check if it has a file extension.
	size_t ch = path.find_last_of(".");

	if( ch == std::string::npos ) 
		return "";

	// Return the file extension.
	return path.substr( 0, ch );
}

//-----------------------------------//

Path PathUtils::getExtension(const Path& path)
{
	// Check if it has a file extension.
	size_t ch = path.find_last_of(".");

	if( ch == std::string::npos ) 
		return "";

	// Return the file extension.
	return path.substr( ++ch );
}

//-----------------------------------//

Path PathUtils::getFile(const Path& path)
{
	// Check if it has a file extension.
	size_t ch = path.find_last_of("/");

	if( ch == std::string::npos ) 
		return path;

	// Return the file extension.
	return path.substr( ++ch );
}

//-----------------------------------//

Path PathUtils::normalize(const Path& path)
{
	Path norm = path;

	String::replace(norm, "\\", "/");
	String::replace(norm, "//", "/");
	String::replace(norm, "../", "");
	String::replace(norm, "./", "");

	return norm;
}

//-----------------------------------//

} // end namespace