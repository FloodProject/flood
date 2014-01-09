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

NAMESPACE_CORE_BEGIN

//-----------------------------------//

MemoryStream::MemoryStream(uint64 size)
	: Stream("", StreamOpenMode::Default)
{
	init();
	resize(size);
}

//-----------------------------------//

MemoryStream::MemoryStream()
	: Stream("", StreamOpenMode::Default)

{
	init();
}

//-----------------------------------//

MemoryStream::~MemoryStream()
{
	if( !close() )
		LogDebug("Error closing memory stream: %s", path.CString());
}

//-----------------------------------//

void MemoryStream::init()
{
	position = 0;
	buffer = nullptr;
	useRawBuffer = false;
	data.Clear();
}

//-----------------------------------//

void MemoryStream::setRawBuffer(uint8* buffer)
{
	this->buffer = buffer;
	useRawBuffer = true;
}

//-----------------------------------//

bool MemoryStream::open()
{
	return true;
}

//-----------------------------------//

bool MemoryStream::close()
{
	return true;
}

//-----------------------------------//

uint64 MemoryStream::read(void* buffer, uint64 size) const
{
	if(size < 0) return -1;

	if(!useRawBuffer)
	{
		int64 left = this->size() - position;
		if((int64)size > left) size = left;
	}

	uint8* cur = this->buffer + position;
	memcpy(buffer, cur, (size_t) size);

	position += size;
	return size;
}

//-----------------------------------//

uint64 MemoryStream::write(void* buffer, uint64 size)
{
	if(size <= 0) return -1;

	if(!useRawBuffer)
	{
		int64 newSize = position + size;
		bool needsResize = newSize > data.Size();
	
		if(needsResize)
			resize(newSize);

		if(data.Empty()) return 0;
	}

	uint8* cur = this->buffer + position;
	memcpy(cur, buffer, (size_t) size);

	position += size;
	return size;
}

//-----------------------------------//

uint64 MemoryStream::getPosition() const
{
	return position;
}

//-----------------------------------//

void MemoryStream::setPosition(int64 offset, StreamSeekMode mode)
{
	switch(mode)
	{
	case StreamSeekMode::Absolute:
		position = offset;
		break;
	case StreamSeekMode::Relative:
		position += offset;
		break;
	case StreamSeekMode::RelativeEnd:
		position = size() - offset;
		break;
	}
}

//-----------------------------------//

uint64 MemoryStream::size() const
{
	return data.Size();
}

//-----------------------------------//

void MemoryStream::resize(int64 size)
{
	if( size <= 0 ) return;

	data.Resize((size_t)size);
	buffer = data.Buffer();
}

//-----------------------------------//

NAMESPACE_CORE_END