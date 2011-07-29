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

#ifdef ENABLE_ARCHIVE_VIRTUAL

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static bool    VirtualArchiveOpen(Archive*, const String&);
static bool    VirtualArchiveClose(Archive*);
static Stream* VirtualArchiveOpenFile(Archive*, const Path&, Allocator*);
static void    VirtualArchiveEnumerateFiles(Archive*, std::vector<Path>&);
static void    VirtualArchiveEnumerateDirectories(Archive*, std::vector<Path>&);
static bool    VirtualArchiveExistsFile(Archive*, const Path&);
static bool    VirtualArchiveExistsDir(Archive*, const Path&);
static bool    VirtualArchiveMonitor(Archive*); 

static ArchiveFuncs gs_VirtualArchiveFuncs =
{
	VirtualArchiveOpen,
	VirtualArchiveClose,
	VirtualArchiveOpenFile,
	VirtualArchiveExistsFile,
	VirtualArchiveExistsDir,
	VirtualArchiveEnumerateFiles,
	VirtualArchiveEnumerateDirectories,
	VirtualArchiveMonitor,
};

struct ArchiveMount
{
	Archive* archive;
	String mount;
};

struct ArchiveVirtual : public Archive
{
	std::vector<Archive*> mounts;
};

//-----------------------------------//

Archive* ArchiveCreateVirtual(Allocator* alloc)
{
	Archive* archive = Allocate(ArchiveVirtual, alloc);
	
	archive->handle = nullptr;
	archive->scheme = "vfs";
	archive->fn = &gs_VirtualArchiveFuncs;

	return archive;
}

//-----------------------------------//

bool ArchiveMount(Archive* archive, Archive* mount, const String& mountPath)
{
	if( !archive || !mount ) return false;

	ArchiveVirtual* varchive = (ArchiveVirtual*) archive;
	varchive->mounts.push_back(mount);

	return true;
}

//-----------------------------------//

void ArchiveMountDirectories(Archive* archive, const String& dirPath, Allocator* alloc)
{
	Archive* dir = ArchiveCreateFromDirectory(alloc, dirPath);
	
	ArchiveMount(archive, dir, "");
	
	std::vector<String> dirs;
	ArchiveEnumerateDirectories(dir, dirs);

	for(size_t i = 0; i < dirs.size(); i++)
	{
		const String& path = dirPath + PathGetSeparator() + dirs[i];
		Archive* ndir = ArchiveCreateFromDirectory(alloc, path);
		ArchiveMount(archive, ndir, "");
	}
}

//-----------------------------------//

static bool VirtualArchiveOpen(Archive* archive, const String& path)
{
	return (archive != nullptr);
}

//-----------------------------------//

static bool VirtualArchiveClose(Archive* archive)
{
	if( !archive ) return false;

	ArchiveVirtual* varchive = (ArchiveVirtual*) archive;
	
	for(size_t i = 0; i < varchive->mounts.size(); i++)
	{
		Archive* marchive = varchive->mounts[i];
		ArchiveDestroy(marchive);
	}

	varchive->mounts.clear();

	return true;
}

//-----------------------------------//

static Stream* VirtualArchiveOpenFile(Archive* archive, const Path& path, Allocator* alloc)
{
	if( !archive ) return nullptr;
	
	ArchiveVirtual* varchive = (ArchiveVirtual*) archive;
	if( varchive->mounts.empty() ) return nullptr;

	Stream* stream = nullptr;

	for(size_t i = 0; i < varchive->mounts.size(); i++)
	{
		Archive* marchive = varchive->mounts[i];
		stream = ArchiveOpenFile(marchive, path, alloc);
		if( stream ) break;
	}

	return stream;
}

//-----------------------------------//

static void VirtualArchiveEnumerate(Archive* archive, std::vector<Path>& paths, bool dir)
{
	ArchiveVirtual* varchive = (ArchiveVirtual*) archive;

	for(size_t i = 0; i < varchive->mounts.size(); i++)
	{
		Archive* marchive = varchive->mounts[i];

		if(dir) ArchiveEnumerateDirectories(marchive, paths);
		else ArchiveEnumerateFiles(marchive, paths);
	}
}

//-----------------------------------//

static void VirtualArchiveEnumerateFiles(Archive* archive, std::vector<Path>& paths)
{
	if( !archive ) return;
	VirtualArchiveEnumerate(archive, paths, false);
}

//-----------------------------------//

static void VirtualArchiveEnumerateDirectories(Archive* archive, std::vector<Path>& paths)
{
	if( !archive ) return;
	VirtualArchiveEnumerate(archive, paths, true);
}

//-----------------------------------//

static bool VirtualArchiveExistsFile(Archive* archive, const Path& path)
{
	if( !archive ) return false;
	ArchiveVirtual* varchive = (ArchiveVirtual*) archive;

	for(size_t i = 0; i < varchive->mounts.size(); i++)
	{
		Archive* marchive = varchive->mounts[i];
		bool exists = ArchiveExistsFile(marchive, path);
		if(exists) return true;
	}

	return false;
}

//-----------------------------------//

static bool VirtualArchiveExistsDir(Archive* archive, const Path& path)
{
	if( !archive ) return false;
	ArchiveVirtual* varchive = (ArchiveVirtual*) archive;

	for(size_t i = 0; i < varchive->mounts.size(); i++)
	{
		Archive* marchive = varchive->mounts[i];
		bool exists = ArchiveExistsDirectory(marchive, path);
		if(exists) return true;
	}

	return false;
}

//-----------------------------------//

static bool VirtualArchiveMonitor(Archive*)
{
	return false;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif
