/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"
#include "Core/Utilities.h"

#ifdef PLATFORM_WINDOWS
	#include "Core/FileWatcherWin32.h"
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

#ifdef ENABLE_ARCHIVE_DIR

static scoped_ptr<FileWatcherWin32> gs_watcher;

static bool    DirArchiveOpen(Archive*, const String&);
static bool    DirArchiveClose(Archive*);
static Stream* DirArchiveOpenFile(Archive*, const Path&, Allocator*);
static void    DirArchiveEnumerateFiles(Archive*, std::vector<Path>&);
static void    DirArchiveEnumerateDirectories(Archive*, std::vector<Path>&);
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
		gs_watcher->removeWatch(archive->watchId);

		if( gs_watcher->mWatches.empty() )
		{
			// Remove the watcher when there are no more watches.
			gs_watcher.reset();
		}
	}

	return true;
}

//-----------------------------------//

static Stream* DirArchiveOpenFile(Archive* archive, const Path& file, Allocator* alloc)
{
	if( !archive ) return nullptr;
	
	Path fullPath = ArchiveCombinePath(archive, file);
	Stream* stream = StreamCreateFromFile(alloc, fullPath, StreamMode::Read);

	return stream;
}

//-----------------------------------//

static void DirArchiveEnumerate(std::vector<String>&, Path, Path, bool);

static void DirArchiveEnumerateFiles(Archive* archive, std::vector<Path>& paths)
{
	if( !archive ) return;
	FileEnumerateFiles(archive->path, paths);
}

//-----------------------------------//

static void DirArchiveEnumerateDirectories(Archive* archive, std::vector<Path>& paths)
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
	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);

	for(size_t i = 0; i < dirs.size(); i++)
	{
		Path normalized = PathNormalize(dirs[i]);
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

	bool created = false;

	// Setup the watcher if it has not been created.
	if( !gs_watcher )
	{
		LogInfo("Creating the file watcher");

		gs_watcher.reset( AllocateHeap(FileWatcherWin32) );
		gs_watcher->onFileWatchEvent.Connect(&HandleFileWatch);

		created = true;
	}

	if(archive->watchId == 0)
	{
		// Add the archive to the watch list.
		archive->watchId = gs_watcher->addWatch(archive->path, archive);
	}

	if(archive->watchId == 0 && created )
		gs_watcher.reset();
	else
		gs_watcher->update();

	return true;
}

#endif

//-----------------------------------//

NAMESPACE_CORE_END