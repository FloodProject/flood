/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"
#include "Core/Array.h"

#ifdef ENABLE_ARCHIVE_VIRTUAL

NAMESPACE_CORE_BEGIN

//-----------------------------------//

ArchiveVirtual::ArchiveVirtual() 
	: Archive("")
	, mounts(*AllocatorGetHeap())
{
}

//-----------------------------------//

ArchiveVirtual::~ArchiveVirtual()
{
	close();
}

//-----------------------------------//

static void HandleWatch(Archive*, const FileWatchEvent& event);

bool ArchiveVirtual::archiveMount(Archive * mount, const Path& mountPath)
{
	mounts.push_back(mount);

	// Setup archive watch callbacks.
	mount->userdata = this;
	mount->watch.Connect(HandleWatch);

	return true;
}

//-----------------------------------//

void ArchiveVirtual::archiveMountDirectories(const Path& dirPath, Allocator* alloc)
{
	Archive* dir = Allocate(alloc, ArchiveDirectory, dirPath);
	if (!dir) return;

	archiveMount(dir, "");
	
	Array<Path> dirs(*AllocatorGetHeap());
	dir->enumerateDirs(dirs);

	for(auto& dir : dirs)
	{
		auto& path = PathCombine(dirPath, dir);
		Archive* ndir = Allocate(alloc, ArchiveDirectory, path);
		archiveMount(ndir, "");
	}
}

//-----------------------------------//

bool ArchiveVirtual::open(const Path& path)
{
	isValid = true; 
	return isValid;
}

//-----------------------------------//

bool ArchiveVirtual::close()
{
	for(auto& i : mounts)
	{
		Archive* marchive = i;
		Deallocate(marchive);
	}

	mounts.clear();

	return true;
}

//-----------------------------------//

Stream* ArchiveVirtual::openFile(const Path& path, Allocator* alloc)
{
	if (mounts.empty()) return nullptr;

	Stream* stream = nullptr;

	for(auto& i : mounts)
	{
		Archive* marchive = i;
		stream = marchive->openFile(path, alloc);
		if (stream) break;
	}

	return stream;
}

//-----------------------------------//

void ArchiveVirtual::enumerate(Array<Path>& paths, bool dir)
{

	for(auto& i : mounts)
	{
		Archive* marchive = i;

		if (dir) marchive->enumerateDirs(paths);
		else marchive->enumerateFiles(paths);
	}
}

//-----------------------------------//

void ArchiveVirtual::enumerateFiles(Array<Path>& paths)
{
	enumerate(paths, false);
}

//-----------------------------------//

void ArchiveVirtual::enumerateDirs(Array<Path>& paths)
{
	enumerate(paths, true);
}

//-----------------------------------//

bool ArchiveVirtual::existsFile(const Path& path)
{
	for(auto& i : mounts)
		if (i->existsFile(path))
			return true;

	return false;
}

//-----------------------------------//

bool ArchiveVirtual::existsDir(const Path& path)
{
	for(auto& i : mounts)
		if(i->existsDir(path))
			return true;

	return false;
}

//-----------------------------------//

static void HandleWatch(Archive* archive, const FileWatchEvent& event)
{
	Archive* varchive = (ArchiveVirtual*) archive->userdata;
	varchive->watch(archive, event);
}

bool ArchiveVirtual::monitor()
{
	for(auto& i : mounts)
		i->monitor();

	return true;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif
