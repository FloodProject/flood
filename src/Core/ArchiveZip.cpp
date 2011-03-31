/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#include <zzip/zzip.h>

NAMESPACE_BEGIN

//-----------------------------------//

#ifdef VAPOR_ARCHIVE_ZIP

static bool ZipArchiveOpen(Archive*, const String&);
static void ZipArchiveClose(Archive*);
static void ZipArchiveEnumerateFiles(Archive*, std::vector<Path>&);
static void ZipArchiveEnumerateDirectories(Archive*, std::vector<Path>&);
static bool ZipArchiveExistsFile(Archive*, const Path&);
static bool ZipArchiveExistsDir(Archive*, const Path&);
static bool ZipArchiveMonitor(Archive*); 

static ArchiveFuncs gs_ZipArchiveFuncs =
{
	ZipArchiveOpen,
	ZipArchiveClose,
	ZipArchiveExistsFile,
	ZipArchiveExistsDir,
	ZipArchiveEnumerateFiles,
	ZipArchiveEnumerateDirectories,
	ZipArchiveMonitor,
};

//-----------------------------------//

Archive* ArchiveCreateFromZip(MemoryAllocator* mem, const Path& path)
{
	Archive* archive = Allocate<Archive>(mem);
	
	archive->Handle = nullptr;
	archive->Scheme = "zip";
	archive->fn = &gs_ZipArchiveFuncs;

	if( !ArchiveOpen(archive, path) )
	{
		LogWarn("Error opening archive: %s", path.c_str());
		Deallocate(mem, archive);
		return nullptr;
	}

	return archive;
}

//-----------------------------------//

static bool ZipArchiveOpen(Archive* archive, const String& path)
{
	if( !archive ) return false;
	
	ZZIP_DIR* zip = zzip_dir_open(path.c_str(), nullptr);
	archive->Handle = zip;
	
	return zip != nullptr;
}

//-----------------------------------//

static void ZipArchiveEnumerate(Archive* archive, std::vector<Path>& paths, bool dir)
{
	if( !archive || !archive->Handle ) return;

	ZZIP_DIR* zip = (ZZIP_DIR*) archive->Handle;
	ZZIP_DIRENT entry;

	zzip_rewinddir(zip);
	
	while( zzip_dir_read(zip, &entry) != 0 )
	{
		Path name = entry.d_name;
		bool isDir = name.back() == '/';

		if( (dir && isDir) || (!dir && !isDir) )
			paths.push_back(name);
	}
}

//-----------------------------------//

static void ZipArchiveEnumerateFiles(Archive* archive, std::vector<Path>& paths)
{
	ZipArchiveEnumerate(archive, paths, false);
}

//-----------------------------------//

static void ZipArchiveEnumerateDirectories(Archive* archive, std::vector<Path>& paths)
{
	ZipArchiveEnumerate(archive, paths, true);
}

//-----------------------------------//

static bool ZipArchiveExistsFile(Archive* archive, const Path& path)
{
	ZZIP_DIR* zip = (ZZIP_DIR*) archive->Handle;

	ZZIP_STAT zstat;
	int res = zzip_dir_stat(zip, path.c_str(), &zstat, ZZIP_CASEINSENSITIVE | ZZIP_IGNOREPATH);

	return (res == ZZIP_NO_ERROR);
}

//-----------------------------------//

static bool ZipArchiveExistsDir(Archive* archive, const Path& path)
{
	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);

	for(size_t i = 0; i < dirs.size(); i++)
	{
		Path& dir = StringTrim(dirs[i], "/");
		if(dir == path) return true;
	}

	return false;
}

//-----------------------------------//

static bool ZipArchiveMonitor(Archive*)
{
	return false;
}

//-----------------------------------//

void ZipArchiveClose(Archive* archive)
{
	ZZIP_DIR* zip = (ZZIP_DIR*) archive->Handle;
	assert( zip != nullptr );

	int ret = zzip_dir_close(zip);
	assert( ret == ZZIP_NO_ERROR );
}

#endif

//-----------------------------------//

NAMESPACE_END