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

#ifdef ENABLE_ARCHIVE_ZIP

#include <cstdio>
#include <zzip/zzip.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

ZipStream::ZipStream()
	: Stream("", StreamOpenMode::Default)

{
}

//-----------------------------------//

ZipStream::ZipStream(ZZIP_DIR* dir, ZZIP_FILE* handle, String path,
	StreamOpenMode mode)
	: Stream(path, mode)
	, dir(dir)
	, handle(handle)
{
}


//-----------------------------------//

ZipStream::~ZipStream()
{
	if( !close() )
		LogDebug("Error closing zip stream: %s", path.c_str());
}

//-----------------------------------//

bool ZipStream::open()
{
	handle = zzip_file_open(dir, path.c_str(), ZZIP_ONLYZIP);
	return handle != nullptr;
}

//-----------------------------------//

bool ZipStream::close()
{
	return zzip_file_close(handle) == ZZIP_NO_ERROR;
}

//-----------------------------------//

int64 ZipStream::read(void* buf, uint64 len) const
{
	return zzip_file_read(handle, buf, (zzip_size_t) len);
}

//-----------------------------------//

int64 ZipStream::getPosition() const
{
	return zzip_tell(handle);
}

//-----------------------------------//

void ZipStream::setPosition(int64 offset, StreamSeekMode mode)
{
	int origin = 0;

	switch(mode)
	{
	case StreamSeekMode::Absolute:
		origin = SEEK_SET;
		break;
	case StreamSeekMode::Relative:
		origin = SEEK_CUR;
		break;
	case StreamSeekMode::RelativeEnd:
		origin = SEEK_END;
		break;
	}

	zzip_seek(handle, (zzip_off_t) offset, origin);
}

//-----------------------------------//

uint64 ZipStream::size() const
{
	ZZIP_STAT zs;
	int ret = zzip_file_stat(handle, &zs);
	assert( ret != -1 );
	
	return zs.st_size;
}

//-----------------------------------//

static bool    ZipArchiveOpen(Archive*, const String&);
static bool    ZipArchiveClose(Archive*);
static Stream* ZipArchiveOpenFile(Archive*, const Path&, Allocator*);
static void    ZipArchiveEnumerateFiles(Archive*, std::vector<Path>&);
static void    ZipArchiveEnumerateDirectories(Archive*, std::vector<Path>&);
static bool    ZipArchiveExistsFile(Archive*, const Path&);
static bool    ZipArchiveExistsDir(Archive*, const Path&);
static bool    ZipArchiveMonitor(Archive*); 

static ArchiveFuncs gs_ZipArchiveFuncs =
{
	ZipArchiveOpen,
	ZipArchiveClose,
	ZipArchiveOpenFile,
	ZipArchiveExistsFile,
	ZipArchiveExistsDir,
	ZipArchiveEnumerateFiles,
	ZipArchiveEnumerateDirectories,
	ZipArchiveMonitor,
};

//-----------------------------------//

Archive* ArchiveCreateFromZip(Allocator* alloc, const Path& path)
{
	Archive* archive = Allocate(alloc, Archive);
	
	archive->handle = nullptr;
	archive->scheme = "zip";
	archive->fn = &gs_ZipArchiveFuncs;
	
	if( !ArchiveOpen(archive, path) )
	{
		//LogWarn("Error opening archive: %s", path.c_str());
		Deallocate( archive);
		return nullptr;
	}

	return archive;
}

//-----------------------------------//

static bool ZipArchiveOpen(Archive* archive, const String& path)
{
	if( !archive ) return false;
	
	ZZIP_DIR* zip = zzip_dir_open(path.c_str(), nullptr);
	archive->handle = zip;
	
	return zip != nullptr;
}

//-----------------------------------//

static bool ZipArchiveClose(Archive* archive)
{
	ZZIP_DIR* zip = (ZZIP_DIR*) archive->handle;
	assert( zip != nullptr );

	int ret = zzip_dir_close(zip);
	return ret == ZZIP_NO_ERROR ;
}

//-----------------------------------//

static Stream* ZipArchiveOpenFile(Archive* archive, const Path& path, Allocator* alloc)
{
	if( !archive ) return nullptr;

	auto zip = AllocateHeap(ZipStream);
	zip->dir = (ZZIP_DIR*) archive->handle;
	zip->handle = nullptr;
	zip->path = path;
	zip->mode = StreamOpenMode::Read; // only read-only for zips for now.
	
	if( !zip->open() )
	{
		//LogWarn("Error opening zip file: %s", path.c_str());
		return nullptr;
	}
	return zip;
}

//-----------------------------------//

static void ZipArchiveEnumerate(Archive* archive, std::vector<Path>& paths, bool dir)
{
	if( !archive || !archive->handle ) return;

	ZZIP_DIR* zip = (ZZIP_DIR*) archive->handle;
	ZZIP_DIRENT entry;

	zzip_rewinddir(zip);
	
	while( zzip_dir_read(zip, &entry) != 0 )
	{
		Path name = entry.d_name;
		
		bool isDir = !name.empty() && name[name.size()-1] == '/';

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
	ZZIP_DIR* zip = (ZZIP_DIR*) archive->handle;

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
		const Path& dir = StringTrim(dirs[i], "/");
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

NAMESPACE_CORE_END

#endif
