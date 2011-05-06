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

#include <zzip/zzip.h>

#ifdef ENABLE_ARCHIVE_ZIP

NAMESPACE_BEGIN

//-----------------------------------//

struct ZipStream : Stream
{
	ZZIP_DIR* dir;
	ZZIP_FILE* handle;
};

static bool  ZipStreamOpen(Stream*);
static bool  ZipStreamClose(Stream*);
static int64 ZipStreamRead(Stream*, void*, int64);
static int64 ZipStreamTell(Stream*);
static bool  ZipStreamSeek(Stream*, int64, int8);
static int64 ZipStreamGetSize(Stream*);

static StreamFuncs gs_ZipStreamFuncs = 
{
	ZipStreamOpen,
	ZipStreamClose,
	ZipStreamRead,
	nullptr /*ZipStreamWrite*/,
	ZipStreamTell,
	ZipStreamSeek,
	ZipStreamGetSize
};

//-----------------------------------//

static bool ZipStreamOpen(Stream* stream)
{
	if( !stream ) return false;

	ZipStream* zip = (ZipStream*) stream;
	zip->handle = zzip_file_open(zip->dir, zip->path.c_str(), ZZIP_ONLYZIP);

	return zip->handle != nullptr;
}

//-----------------------------------//

static bool ZipStreamClose(Stream* stream)
{
	if( !stream ) return false;

	ZipStream* zip = (ZipStream*) stream;
	int ret = zzip_file_close(zip->handle);

	return ret == ZZIP_NO_ERROR;
}

//-----------------------------------//

static int64 ZipStreamRead(Stream* stream, void* buf, int64 len)
{
	if( !stream ) return false;

	ZipStream* zip = (ZipStream*) stream;
	return zzip_file_read(zip->handle, buf, (zzip_size_t) len);
}

//-----------------------------------//

static int64 ZipStreamTell(Stream* stream)
{
	if( !stream ) return false;

	ZipStream* zip = (ZipStream*) stream;
	return zzip_tell(zip->handle);
}

//-----------------------------------//

static bool ZipStreamSeek(Stream* stream, int64 offset, int8 mode)
{
	if( !stream ) return false;

	ZipStream* zip = (ZipStream*) stream;

	int origin = 0;

	switch(mode)
	{
	case StreamSeekMode::Absolute: origin = SEEK_SET; break;
	case StreamSeekMode::Relative: origin = SEEK_CUR; break;
	case StreamSeekMode::RelativeEnd: origin = SEEK_END; break;
	}

	return zzip_seek(zip->handle, (zzip_off_t) offset, origin) > 0;
}

//-----------------------------------//

static int64 ZipStreamGetSize(Stream* stream)
{
	if( !stream ) return false;
	
	ZipStream* zip = (ZipStream*) stream;

	ZZIP_STAT zs;
	int ret = zzip_file_stat(zip->handle, &zs);
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
	Archive* archive = Allocate(Archive, alloc);
	
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

	ZipStream* stream = Allocate(ZipStream, alloc);
	stream->dir = (ZZIP_DIR*) archive->handle;
	stream->handle = nullptr;
	stream->path = path;
	stream->mode = StreamMode::Read; // only read-only for zips for now.
	stream->fn = &gs_ZipStreamFuncs;

	if( !ZipStreamOpen(stream) )
	{
		//LogWarn("Error opening zip file: %s", path.c_str());
		Deallocate( stream);
		return nullptr;
	}

	return stream;
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

NAMESPACE_END

#endif
