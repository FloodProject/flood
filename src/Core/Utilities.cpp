/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Utilities.h"
#include "Core/String.h"
#include "Core/Math/Hash.h"

#include <sstream>
#include <algorithm>
#include <cstdio>

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#undef  NOMINMAX
	#define NOMINMAX
	#include <Windows.h>

	#include <direct.h>
#else
	#include <unistd.h>
#endif

#ifdef PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

#if defined(PLATFORM_NACL)
// Declare these manually because GCC/Clang in C++11 mode
// define __STRICT_ANSI__ and that makes them hidden.
extern "C" {
__cdecl int strcasecmp(const char *, const char *);
__cdecl int vsnprintf(char *, size_t, const char *, __VALIST);
}
#endif

#include <dirent.h>

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
	return _stricmp(s1.CString(), s2.CString());
#else
	return strcasecmp(s1.CString(), s2.CString());
#endif
}

//-----------------------------------//

UTF8String StringFromFloat( float n, byte precision )
{
	return StringFormat("%#.*f", precision, n );
}

//-----------------------------------//

#ifdef PLATFORM_WINDOWS

UTF8String StringFromWideString(const WString &wstr)
{
	UTF8String strTo(wstr);
	return strTo;
}

#endif

//-----------------------------------//

#ifdef PLATFORM_WINDOWS

WString StringToWideString(const UTF8String &str)
{
	WString ret(str);
	return ret;
}

#endif

//-----------------------------------//

UTF8String StringFormat(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	UTF8String formatted = StringFormatArgs(str, args);

	va_end(args);

	return formatted;
}

//-----------------------------------//

UTF8String StringFormatArgs(const char* str, va_list args)
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

	return UTF8String(buf);
}

//-----------------------------------//

void StringSplit(const UTF8String& s, char delim, Vector<UTF8String>& elems)
{
	s.Split(delim, elems);
}

//-----------------------------------//

UTF8String StringTrim(const UTF8String& s, const char* trim)
{
	UTF8String Trim(trim);
	unsigned first = s.FindFirstNotOf(Trim);
	unsigned last = s.FindLastNotOf(Trim);
	if (first == UTF8String::NPOS) return "";
	
	return s.Substring(first, last - first + 1);
}

//-----------------------------------//

Path PathGetFileBase(const Path& path)
{
	Path filePath = PathGetFile(path);

	// Check if it has a file extension.
	size_t ch = filePath.FindLast(".");

	if( ch == UTF8String::NPOS ) return "";

	// Return the file extension.
	return filePath.Substring( 0, ch );
}

//-----------------------------------//

Path PathGetFileExtension(const Path& path)
{
	// Check if it has a file extension.
	size_t ch = path.FindLast(".");

	if( ch == UTF8String::NPOS ) 
		return "";

	// Return the file extension.
	return path.Substring( ++ch );
}

//-----------------------------------//

Path PathGetBase(const Path& path)
{
	size_t ch = path.FindLast("/");

	if( ch == UTF8String::NPOS )
		ch = path.FindLast("\\");

	if( ch == UTF8String::NPOS )
		return path;

	// Return the file extension.
	return path.Substring( 0, ++ch );
}

//-----------------------------------//

Path PathGetFile(const Path& path)
{
	size_t ch = path.FindLast("/");

	if( ch == UTF8String::NPOS )
		ch = path.FindLast("\\");

	if( ch == UTF8String::NPOS )
		return path;

	// Return the file part.
	return path.Substring( ++ch );
}

//-----------------------------------//

Path PathNormalize(const Path& path)
{
	Path norm = path;

	norm.Replace("\\", "/");
	norm.Replace("//", "/");
	norm.Replace("/\\", "/");
	norm.Replace("\\/", "/");

	// These transformations are unsafe.
#if 0
	norm.Replace("../", "");
	norm.Replace("./", "");
#endif

	return norm;
}

//-----------------------------------//

Path PathGetCurrentDir()
{
	char buf[256];
#ifdef PLATFORM_WINDOWS
	_getcwd(buf, FLD_ARRAY_SIZE(buf));
#else
	getcwd(buf, FLD_ARRAY_SIZE(buf));
#endif
	return UTF8String(buf);
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

	if( base.Empty() ) return extra;

	const Path& sep = PathGetSeparator();

	base.Replace("\\", sep);
	base.Replace("/", sep);

	extra.Replace("\\", sep);
	extra.Replace("/", sep);

	return base + PathGetSeparator() + extra;
}

//-----------------------------------//

StringHash::StringHash()
{

}

//-----------------------------------//

StringHash::StringHash(const char* str, size_t size)
	: hash( HashMurmur2(0xF00D, (uint8*)str, size) )
{
}

//-----------------------------------//

StringHash HashString(const UTF8String& s)
{
	return StringHash(s.CString(), s.ByteLength());
}

//-----------------------------------//

static void DirArchiveEnumerate(Vector<UTF8String>& paths, Path dirPath,
								Path filePath, bool dirs)
{
	// Open directory stream.
	DIR* dir = opendir(dirPath.CString());
	if( !dir ) return;

	dirent* entry = nullptr;
	
	// Get all the files and directories within directory.
	while((entry = readdir(dir)))
	{
		const Path& name = entry->d_name;
		
#ifndef PLATFORM_NACL
		switch(entry->d_type)
		{
		case DT_REG:
		{
			Path sep = filePath.Empty() ? "" : PathGetSeparator();
			Path path = StringFormat("%s%s%s", filePath.CString(), sep.CString(),
				name.CString() );
			if(!dirs) paths.Push(path);
			break;
		}
		case DT_DIR:
		{
			if(!name.Empty() && name[0] == '.') continue;
		
			Path _dirPath = PathCombine(dirPath, name);
			Path _filePath = PathCombine(filePath, name);

			if(dirs) paths.Push(_filePath);
			DirArchiveEnumerate(paths, _dirPath, _filePath, dirs);
			
			break;
		} }
#endif
	}

	closedir(dir);
}

//-----------------------------------//

void FileEnumerateFiles(const Path& path, Vector<Path>& files)
{
	DirArchiveEnumerate(files, path, "", false);
}

//-----------------------------------//

void FileEnumerateDirectories(const Path& path, Vector<Path>& dirs)
{
	DirArchiveEnumerate(dirs, path, "", true);
}

//-----------------------------------//

bool FileExists(const Path& path)
{
#ifdef COMPILER_MSVC
	return _access(path.CString(), F_OK) == 0;
#else
	return access(path.CString(), F_OK) == 0;
#endif
}

//-----------------------------------//

NAMESPACE_CORE_END