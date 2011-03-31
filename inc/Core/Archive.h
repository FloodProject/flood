/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct Archive;

typedef bool (*ArchiveOpenFunction)(Archive*, const Path&);
typedef void (*ArchiveCloseFunction)(Archive*);
typedef bool (*ArchiveExistsFileFunction)(Archive*, const Path&);
typedef bool (*ArchiveExistsDirFunction)(Archive*, const Path&);
typedef void (*ArchiveEnumerateFilesFunction)(Archive*, std::vector<Path>&);
typedef void (*ArchiveEnumerateDirsFunction)(Archive*, std::vector<Path>&);
typedef bool (*ArchiveMonitorFunction)(Archive*);

struct ArchiveFuncs
{
	ArchiveOpenFunction            open;
	ArchiveCloseFunction           close;
	ArchiveExistsFileFunction      exists_file;
	ArchiveExistsDirFunction       exists_dir;
	ArchiveEnumerateFilesFunction  enumerate_files;
	ArchiveEnumerateDirsFunction   enumerate_dirs;
	ArchiveMonitorFunction         monitor;
};
	 
/**
 * Archives are a structured collection of files. The most common archive
 * implementations are ZIP archive files and OS filesystem directories.
 */

struct Archive
{
	String Path;
	String Scheme;
	void* Handle;
	
	ArchiveFuncs* fn;
};

// Creates a new archive from a ZIP.
API_CORE Archive* ArchiveCreateFromZip(MemoryAllocator*, const Path&);

// Creates a new archive from a directory.
API_CORE Archive* ArchiveCreateFromDirectory(MemoryAllocator*, const Path&);

// Destroys the archive and deallocates its memory.
API_CORE void ArchiveDestroy(Archive*, MemoryAllocator*);

// Opens the archive.
API_CORE bool ArchiveOpen(Archive*, const Path&);

// Opens a file from the archive.
//API_CORE void ArchiveOpenFile(Archive*, const String&);

// Tests if the given file/directory exists in the archive.
API_CORE bool ArchiveExistsFile(Archive*, const Path&);
API_CORE bool ArchiveExistsDirectory(Archive*, const Path&);

// Enumerates all the files or directories in the archive.
API_CORE void ArchiveEnumerateFiles(Archive*, std::vector<Path>&);
API_CORE void ArchiveEnumerateDirectories(Archive*, std::vector<Path>&);

// Filesystem helpers.
API_CORE bool FileExists(const Path&);
API_CORE void FileEnumerateFiles(const Path&, std::vector<Path>&);
API_CORE void FileEnumerateDirectories(const Path&, std::vector<Path>&);

//-----------------------------------//

NAMESPACE_EXTERN_END

