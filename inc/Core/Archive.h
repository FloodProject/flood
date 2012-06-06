/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Pointers.h"
#include "Core/FileWatcher.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct Stream;
struct Archive;
struct ArchiveFuncs;

// Destroys the archive and deallocates its memory.
API_CORE void ArchiveDestroy(Archive*);

// Opens the archive.
API_CORE bool ArchiveOpen(Archive*, const Path&);

// Opens a file from the archive.
API_CORE Stream* ArchiveOpenFile(Archive*, const Path&, Allocator*);

// Tests if the given file/directory exists in the archive.
API_CORE bool ArchiveExistsFile(Archive*, const Path&);
API_CORE bool ArchiveExistsDirectory(Archive*, const Path&);

// Enumerates all the files or directories in the archive.
API_CORE void ArchiveEnumerateFiles(Archive*, std::vector<Path>&);
API_CORE void ArchiveEnumerateDirectories(Archive*, std::vector<Path>&);

// Combines the path of a file to get the full path to an archive file.
API_CORE Path ArchiveCombinePath(Archive*, const Path& filePath);

// Creates a new virtual archive.
API_CORE Archive* ArchiveCreateVirtual(Allocator*);

// Mounts an archive in the virtual archive.
API_CORE bool ArchiveMount(Archive*, Archive*, const String& mountPath);

// Mounts a directory and its direct hierarchy.
API_CORE void ArchiveMountDirectories(Archive*, const String& dirPath, Allocator*);

// Creates a new archive from a ZIP.
API_CORE Archive* ArchiveCreateFromZip(Allocator*, const Path&);

// Creates a new archive from a directory.
API_CORE Archive* ArchiveCreateFromDirectory(Allocator*, const Path&);

// Pointer helpers.
typedef scoped_ptr<Archive, ArchiveDestroy> ArchiveScopedPtr;
#define ArchiveCreateFromZipScoped(alloc, ...) CreateScopedPtr(ArchiveCreateFromZip, alloc, __VA_ARGS__)
#define ArchiveCreateFromDirectoryScoped(alloc, ...) CreateScopedPtr(ArchiveCreateFromDirectory, alloc, __VA_ARGS__)
#define ArchiveCreateVirtualScoped(alloc, ...) CreateScopedPtr(ArchiveCreateVirtual, alloc, __VA_ARGS__)

typedef uint32 ArchiveWatchId;
typedef Event2<Archive*, const FileWatchEvent&> ArchiveWatchEvent;

// Sets up and updates the watching functionality for the archive.
API_CORE bool ArchiveWatchUpdate(Archive*);

/**
 * Archives are a structured collection of files. The most common archive
 * implementations are ZIP archive files and OS filesystem directories.
 */

struct Archive
{
	Archive();
	virtual ~Archive();

	Path   path;
	String scheme;

	void*  handle;
	void*  userdata;

	ArchiveWatchEvent watch;
	ArchiveWatchId watchId;

	ArchiveFuncs* fn;
};

typedef bool (*ArchiveOpenFunction)(Archive*, const Path&);
typedef bool (*ArchiveCloseFunction)(Archive*);
typedef Stream* (*ArchiveOpenFileFunction)(Archive*, const Path&, Allocator*);
typedef bool (*ArchiveExistsFileFunction)(Archive*, const Path&);
typedef bool (*ArchiveExistsDirFunction)(Archive*, const Path&);
typedef void (*ArchiveEnumerateFilesFunction)(Archive*, std::vector<Path>&);
typedef void (*ArchiveEnumerateDirsFunction)(Archive*, std::vector<Path>&);
typedef bool (*ArchiveWatchFunction)(Archive*);

struct ArchiveFuncs
{
	ArchiveOpenFunction            open;
	ArchiveCloseFunction           close;
	ArchiveOpenFileFunction        open_file;
	ArchiveExistsFileFunction      exists_file;
	ArchiveExistsDirFunction       exists_dir;
	ArchiveEnumerateFilesFunction  enumerate_files;
	ArchiveEnumerateDirsFunction   enumerate_dirs;
	ArchiveWatchFunction           watch;
};

//-----------------------------------//

NAMESPACE_EXTERN_END

