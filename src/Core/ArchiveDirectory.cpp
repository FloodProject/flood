/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_ARCHIVE_DIR

#include "Core/Archive.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"
#include "Core/Utilities.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

ArchiveDirectory::ArchiveDirectory(const Path& path)
	: Archive(path)
{
	open(path);
}

//-----------------------------------//

ArchiveDirectory::~ArchiveDirectory()
{
	close();
}

//-----------------------------------//

bool ArchiveDirectory::open(const Path& path)
{
	isValid = true;
	return isValid;
}

//-----------------------------------//

bool ArchiveDirectory::close()
{
	if (watchId != 0)
	{
		// Remove the archive from the watch list.
		GetFileWatcher()->removeWatch(watchId);
	}

	return true;
}

//-----------------------------------//

Stream* ArchiveDirectory::openFile(const Path& file, Allocator* alloc)
{
	Path fullPath = combinePath(file);

	if (!FileExists(fullPath))
		return nullptr;

	Stream* stream = Allocate(alloc, FileStream, fullPath, StreamOpenMode::Read);

	return stream;
}

//-----------------------------------//

static void DirArchiveEnumerate(Vector<String>&, Path, Path, bool);

void  ArchiveDirectory::enumerateFiles(Vector<Path>& paths)
{
	FileEnumerateFiles(path, paths);
}

//-----------------------------------//

void ArchiveDirectory::enumerateDirs(Vector<Path>& paths)
{
	FileEnumerateDirectories(path, paths);
}

//-----------------------------------//

bool ArchiveDirectory::existsFile(const Path& path)
{
	Path normalized = PathNormalize(path);
	auto& fullPath = combinePath(normalized);
	return FileExists(fullPath);
}

//-----------------------------------//

bool ArchiveDirectory::existsDir(const Path& path)
{
	Vector<Path> dirs;
	enumerateDirs(dirs);
	Path normalizedPath = PathNormalize(path);


	for(auto& i : dirs)
	{
		Path normalized = PathNormalize(i);
		Path dir = StringTrim(normalized, "/");
		if(dir == normalizedPath) return true;
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

bool ArchiveDirectory::monitor()
{
	GetFileWatcher()->onFileWatchEvent.Connect(&HandleFileWatch);

	if(watchId == 0)
	{
		// Add the archive to the watch list.
		watchId = GetFileWatcher()->addWatch(path, this);
	}

	GetFileWatcher()->update();

	return true;
}

#endif

//-----------------------------------//

NAMESPACE_CORE_END