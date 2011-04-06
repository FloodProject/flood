/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#ifdef VAPOR_VFS_PHYSFS

#include <physfs.h>

namespace vapor {

//-----------------------------------//

struct PhysfsStream : Stream
{
	PHYSFS_File* handle;
};

static bool  FileOpen(Stream*);
static bool  FileClose(Stream*);
static int64 FileRead(Stream*, void*, int64);
static int64 FileWrite(Stream*, void*, int64);
static int64 FileTell(Stream*);
static bool  FileSeek(Stream*, int64, int8);
static int64 FileGetSize(Stream*);
static void  FileLog(Stream*, const String&);

static StreamFuncs gs_FileFuncs = 
{
	FileOpen,
	FileClose,
	FileRead,
	FileWrite,
	FileTell,
	FileSeek,
	FileGetSize
};

//-----------------------------------//

Stream* StreamCreateFromPhysfs(Allocator* alloc, const Path& path, StreamMode::Enum mode)
{
	PhysfsStream* pfs = Allocate<PhysfsStream>(mem);
	
	pfs->handle = nullptr;
	pfs->mode = mode;
	pfs->fn = &gs_FileFuncs;

	Path newPath = PathNormalize(path);
	newPath = PathGetFile(newPath);
	pfs->path = newPath;

	if( !FileOpen(pfs) )
	{
		LogWarn("Error opening pfs: %s", PathGetFile(path).c_str());
		Deallocate(mem, pfs);
		return nullptr;
	}

	return pfs;
}

//-----------------------------------//

typedef PHYSFS_File* (*PhysfsOpenFunction) (const char*);

static bool FileOpen(Stream* stream)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;
	assert( pfs->handle == 0 );

	PhysfsOpenFunction open = nullptr;

	switch(pfs->mode)
	{
	case StreamMode::Read:   open = PHYSFS_openRead; break;
	case StreamMode::Write:  open = PHYSFS_openWrite; break;
	case StreamMode::Append: open = PHYSFS_openAppend; break;
	}

	pfs->handle = open( pfs->path.c_str() );

	if( !pfs->handle )
	{
		FileLog(pfs, "Could not open pfs");
		return false;
	}

	return true;
}

//-----------------------------------//

static bool FileClose(Stream* stream)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;
	
	if( !PHYSFS_close(pfs->handle) )
		return false;

	pfs->handle = nullptr;

	return true;
}

//-----------------------------------//

static int64 FileRead(Stream* stream, void* buffer, int64 size)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;

#if 0
	if( !FileValidate(pfs, StreamMode::Read) ) return false;
	if( !FileValidate(pfs, StreamMode::Append) ) return false;
#endif

	if( PHYSFS_eof(pfs->handle) ) return false; 
	sint64 read = PHYSFS_read(pfs->handle, buffer, 1, (uint32) size);
	
	if(read < 0) FileLog(pfs, "Could not read from pfs");

	return read;
}

//-----------------------------------//

static sint64 FileWrite(Stream* stream, void* buffer, int64 size)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;

#if 0
	if( !FileValidate(pfs, StreamMode::Write) ) return 0;
#endif

	sint64 written = PHYSFS_write(pfs->handle, buffer, 1, (uint32) size);
	
	if(written < 0)
	{
		FileLog(pfs, "Could not write to pfs");
		return 0;
	}

	return written;
}

//-----------------------------------//

static sint64 FileGetSize(Stream* stream)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;
	PHYSFS_sint64 size = PHYSFS_fileLength(pfs->handle);
	return size;
}

//-----------------------------------//

static void FileValidate(Stream* stream, StreamMode::Enum mode)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;
	assert( pfs->mode == mode );
}

//-----------------------------------//

static bool FileSeek(Stream* stream, sint64 pos, int8)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;
	
	if(pos < 0 || pos >= FileGetSize(pfs))
	{
		FileLog(pfs, "Seek to out of bounds position");
		return false;
	}
	
	if( !PHYSFS_seek(pfs->handle, pos) )
	{
		FileLog(pfs, "Failure to seek");
		return false;
	}

	return true;
}

//-----------------------------------//

static sint64 FileTell(Stream* stream)
{
	PhysfsStream* pfs = (PhysfsStream*) stream;
	PHYSFS_sint64 position = PHYSFS_tell(pfs->handle);
	return position;
}

//-----------------------------------//

static bool FileExists(const Path& path)
{
	return PHYSFS_exists(path.c_str()) != 0;
}

//-----------------------------------//

static void FileLog(Stream* pfs, const String& err)
{
	LogError( "%s '%s': %s", err.c_str(), pfs->path.c_str(), PHYSFS_getLastError() );
}

//-----------------------------------//

} // end namespace

#endif