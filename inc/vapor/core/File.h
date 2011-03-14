/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "FileStream.h"
#include "Utilities.h"

#ifdef VAPOR_VFS_PHYSFS

struct PHYSFS_File;

BEGIN_NAMESPACE_EXTERN

//-----------------------------------//

/**
 * Represents a virtual file in the virtual filesystem.
 * If you open a file for reading, it will be searched in the virtual 
 * filesystem and if found, a file pointer will be returned. If a write 
 * mode is used, a new file will be opened for writing in a special user
 * directory. The location of that directory will be platform-dependant, 
 * but you should use it. For example, different users can mantain game
 * data in the same machine, and it will be saved in each user's directory.
 */

struct MemoryAllocator;

struct File
{
	DECLARE_UNCOPYABLE(File);

	File();
	File(const Path& path, StreamMode::Enum mode);

	Path Path;
	bool IsClosed;
	PHYSFS_File* Handle;
	StreamMode::Enum Mode;
};

CORE_API File*  FileCreate(MemoryAllocator* mem, const Path& path, StreamMode::Enum mode);
CORE_API void   FileDestroy(File* file, MemoryAllocator* mem);

CORE_API bool   FileOpen(File* file);
CORE_API bool   FileClose(File* file);

CORE_API bool   FileRead(File* file, std::vector<uint8>& data);
CORE_API bool   FileReadLines(File* file, std::vector<String>& lines);
CORE_API bool   FileReadBuffer(File* file, void* buffer, sint32 size);

CORE_API sint64 FileWrite(File* file, const std::vector<byte>& buffer);
CORE_API sint64 FileWriteString(const String& string);

CORE_API sint64 FileTell(File* file);
CORE_API bool   FileSeek(File* file, sint64 position);

CORE_API sint64 FileGetSize(File* file);
CORE_API Path   FileGetFullPath(File* file);
CORE_API bool   FileExists(const Path& path);

CORE_API void	FileEnumerateFiles(const String& path, std::vector<String>&);
CORE_API void	FileEnumerateDirs(const String& path, std::vector<String>&);
	
//-----------------------------------//

END_NAMESPACE_EXTERN

#endif