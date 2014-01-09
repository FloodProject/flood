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

ArchiveZip::ArchiveZip(const Path& path)
	: Archive(path)
	, handle(nullptr)
{
	open(path);
}

//-----------------------------------//

ArchiveZip::~ArchiveZip()
{
	close();
}

//-----------------------------------//

bool ArchiveZip::open(const Path& path)
{
	ZZIP_DIR* zip = zzip_dir_open(path.c_str(), nullptr);
	handle = zip;
	isValid = zip != nullptr;
	return isValid;
}

//-----------------------------------//

bool ArchiveZip::close()
{
	ZZIP_DIR* zip = (ZZIP_DIR*) handle;
	if (!isValid)
		return true;

	int ret = zzip_dir_close(zip);
	return ret == ZZIP_NO_ERROR ;
}

//-----------------------------------//

Stream* ArchiveZip::openFile(const Path& path, Allocator* alloc)
{
	if (!isValid)
		return nullptr;

	auto zip = Allocate(alloc, ZipStream, (ZZIP_DIR*) handle, nullptr, path, StreamOpenMode::Read);
	
	if (!zip->open())
	{
		LogWarn("Error opening zip file: %s", path.c_str());
		Deallocate(zip);
		return nullptr;
	}

	return zip;
}

//-----------------------------------//

void ArchiveZip::enumerate(Vector<Path>& paths, bool dir)
{
	if (!isValid || !handle) return;

	ZZIP_DIR* zip = (ZZIP_DIR*) handle;
	ZZIP_DIRENT entry;

	zzip_rewinddir(zip);
	
	while( zzip_dir_read(zip, &entry) != 0 )
	{
		Path name = entry.d_name;
		
		bool isDir = !name.empty() && name[name.size()-1] == '/';

		if( (dir && isDir) || (!dir && !isDir) )
			paths.Push(name);
	}
}

//-----------------------------------//

void ArchiveZip::enumerateFiles(Vector<Path>& paths)
{
	enumerate(paths, false);
}

//-----------------------------------//

void ArchiveZip::enumerateDirs(Vector<Path>& paths)
{
	enumerate(paths, true);
}

//-----------------------------------//

bool ArchiveZip::existsFile(const Path& path)
{
	if (!isValid)
		return false;
	
	ZZIP_DIR* zip = (ZZIP_DIR*) handle;

	ZZIP_STAT zstat;
	Path normalized = PathNormalize(path);
	int res = zzip_dir_stat(zip, normalized.c_str(), &zstat, ZZIP_CASEINSENSITIVE | ZZIP_IGNOREPATH);

	return (res == ZZIP_NO_ERROR);
}

//-----------------------------------//

bool ArchiveZip::existsDir(const Path& path)
{
	Vector<Path> dirs;
	enumerateDirs(dirs);
	Path normalized = PathNormalize(path);

	for(auto& i : dirs)
	{
		auto& dir = StringTrim(i, "/");
		if(dir == normalized) return true;
	}

	return false;
}

//-----------------------------------//

bool ArchiveZip::monitor()
{
	return false;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif
