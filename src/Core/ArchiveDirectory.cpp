/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_ARCHIVE_DIR

#include "Core/Memory.h"
#include "Core/Containers/Array.h"
#include "Core/Stream.h"
#include "Core/Log.h"
#include "Core/Utilities.h"

#include "Core/Archive.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static bool    DirArchiveOpen(Archive*, const String&);
static bool    DirArchiveClose(Archive*);
static Stream* DirArchiveOpenFile(Archive*, const Path&, Allocator*);
static void    DirArchiveEnumerateFiles(Archive*, Array<Path*>&);
static void    DirArchiveEnumerateDirectories(Archive*, Array<Path*>&);
static bool    DirArchiveExistsFile(Archive*, const Path&);
static bool    DirArchiveExistsDir(Archive*, const Path&);
static bool    DirArchiveMonitor(Archive*);

static ArchiveFuncs gs_DirArchiveFuncs =
{
	DirArchiveOpen,
	DirArchiveClose,
	DirArchiveOpenFile,
	DirArchiveExistsFile,
	DirArchiveExistsDir,
	DirArchiveEnumerateFiles,
	DirArchiveEnumerateDirectories,
	DirArchiveMonitor,
};

//-----------------------------------//

Archive* ArchiveCreateFromDirectory(Allocator* alloc, const Path& path)
{
	Archive* archive = Allocate(alloc, Archive);
	
	archive->path = path;
	archive->handle = nullptr;
	archive->scheme = "fs";
	archive->fn = &gs_DirArchiveFuncs;

	if( !ArchiveOpen(archive, path) )
	{
		LogDebug("Error opening archive: %s", path.c_str());
		Deallocate(archive);
		return nullptr;
	}

	return archive;
}

//-----------------------------------//

static bool DirArchiveOpen(Archive* archive, const String& path)
{
	if( !archive ) return false;
	return true;
}

//-----------------------------------//

static bool DirArchiveClose(Archive* archive)
{
	if( !archive ) return false;

	if(archive->watchId != 0)
	{
		// Remove the archive from the watch list.
		GetFileWatcher()->removeWatch(archive->watchId);
	}

	return true;
}

//-----------------------------------//

static Stream* DirArchiveOpenFile(Archive* archive, const Path& file, Allocator* alloc)
{
	if( !archive ) return nullptr;
	
	Path fullPath = ArchiveCombinePath(archive, file);
	Stream* stream = StreamCreateFromFile(alloc, fullPath, StreamOpenMode::Read);

	return stream;
}

//-----------------------------------//

static void DirArchiveEnumerate(Array<String*>&, Path, Path, bool);

static void DirArchiveEnumerateFiles(Archive* archive, Array<Path*>& paths)
{
	if( !archive ) return;
	FileEnumerateFiles(archive->path, paths);
}

//-----------------------------------//

static void DirArchiveEnumerateDirectories(Archive* archive, Array<Path*>& paths)
{
	if( !archive ) return;
	FileEnumerateDirectories(archive->path, paths);
}

//-----------------------------------//

static bool DirArchiveExistsFile(Archive* archive, const Path& file)
{
	if( !archive ) return false;
	
	const Path& fullPath = ArchiveCombinePath(archive, file);
	return FileExists(fullPath);
}

//-----------------------------------//

static bool DirArchiveExistsDir(Archive* archive, const Path& path)
{
	Array<Path*> dirs(*AllocatorGetHeap());
	ArchiveEnumerateDirectories(archive, dirs);

	for(size_t i = 0; i < array::size(dirs); i++)
	{
		Path normalized = PathNormalize(*dirs[i]);
		Path dir = StringTrim(normalized, "/");
		if(dir == path) return true;
	}

	return false;
}

//-----------------------------------//

static void HandleFileWatch(const FileWatchEvent& event)
{
	Archive* archive = (Archive*) event.userdata;
	assert( archive != nullptr );

	archive->watch(archive, event);
}

//-----------------------------------//

static bool DirArchiveMonitor(Archive* archive)
{
	if( !archive ) return false;

	GetFileWatcher()->onFileWatchEvent.Connect(&HandleFileWatch);

	if(archive->watchId == 0)
	{
		// Add the archive to the watch list.
		archive->watchId = GetFileWatcher()->addWatch(archive->path, archive);
	}

	GetFileWatcher()->update();

	return true;
}

#endif

//-----------------------------------//

NAMESPACE_CORE_END