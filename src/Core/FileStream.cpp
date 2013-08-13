/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#include <cstdio>

#ifdef PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

FileStream::FileStream(const Path& path, StreamOpenMode mode)
	: Stream(path, mode)
	, fileHandle(nullptr)
    , isValid(false)
{
	open();
}

//-----------------------------------//

FileStream::~FileStream()
{
	if( !close() )
		LogDebug("Error closing file stream: %s", path.c_str());
}

//-----------------------------------//

bool FileStream::open()
{
	const char* mode = nullptr;

	switch(this->mode)
	{
		case StreamOpenMode::Read:   mode = "rb"; break;
		case StreamOpenMode::Write:  mode = "w+b"; break;
		case StreamOpenMode::Append: mode = "a+b"; break;
	}

#ifdef COMPILER_MSVC
	fopen_s(&fileHandle, path.c_str(), mode);
#else
	fileHandle = fopen(path.c_str(), mode);
#endif

	isValid = fileHandle != nullptr;
	return isValid;
}

//-----------------------------------//

bool FileStream::close()
{
	if (!fileHandle)
		return true;

	int ret = fclose(fileHandle);
	fileHandle = nullptr;
	isValid = false;
	return ret == 0;
}

//-----------------------------------//

int64 FileStream::read(void* buffer, uint64 size) const
{
	if (!isValid)
		return InvalidState;

	if (feof(fileHandle))
		return EndOfStream;

	return fread(buffer, (size_t)size, /*NumbBlocks=*/1, fileHandle);
}

//-----------------------------------//

int64 FileStream::write(void* buffer, uint64 size)
{
	if (!isValid)
		return InvalidState;

	assert( mode == StreamOpenMode::Write || mode == StreamOpenMode::Append );
	assert( buffer && size >= 0 );

	return fwrite(buffer, size_t(size), 1, fileHandle);
}

//-----------------------------------//

int64 FileStream::getPosition() const
{
	if (!isValid)
		return InvalidState;

#ifdef COMPILER_MSVC
	return _ftelli64(fileHandle);
#else
	return ftell(fileHandle);
#endif
}

//-----------------------------------//

void FileStream::setPosition(int64 offset, StreamSeekMode mode)
{
	if (!isValid)
		return;

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

#ifdef COMPILER_MSVC
	_fseeki64(fileHandle, offset, origin);
#else
	fseek(fileHandle, (long) offset, origin);
#endif
}

//-----------------------------------//

uint64 FileStream::size() const
{
	if (!isValid)
		return InvalidState;

#ifdef COMPILER_MSVC
	return _filelengthi64( _fileno(fileHandle) );
#else
	// Hold the current file position.
	int64 curr = getPosition();
	
	// Seek to the end of the file and get position.
	setPosition(0, (int)StreamSeekMode::Absolute);
	
	int64 size = getPosition();;
	
	// Seek again to the previously current position.
	setPosition(curr, (int)StreamSeekMode::Absolute);

	return size;
#endif
}

//-----------------------------------//

void FileStream::setBuffering(bool state)
{
	if (!isValid)
		return;
	
	int mode = _IOFBF;
	
	if (!state)
		mode = _IONBF;
	
	setvbuf(fileHandle, nullptr, mode, 0);
}

//-----------------------------------//

NAMESPACE_CORE_END