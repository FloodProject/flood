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
#include "Core/Array.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

MemoryStream::MemoryStream(uint64 size)
	: Stream("", StreamOpenMode::Default)
	, data(*AllocatorGetHeap())
{
	init();
	resize(size);
}

//-----------------------------------//

MemoryStream::MemoryStream()
	: Stream("", StreamOpenMode::Default)
	, data(*AllocatorGetHeap())
{
	init();
}

//-----------------------------------//

MemoryStream::~MemoryStream()
{
	if( !close() )
		LogDebug("Error closing memory stream: %s", path.c_str());
}

//-----------------------------------//

void MemoryStream::init()
{
	position = 0;
	buffer = nullptr;
	useRawBuffer = false;
	data.clear();
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

int64 MemoryStream::read(void* buffer, uint64 size) const
{
	if(size < 0) return -1;

	if(!useRawBuffer)
	{
		int64 left = this->size() - position;
		if(size > left) size = left;
	}

	uint8* cur = this->buffer + position;
	memcpy(buffer, cur, (size_t) size);

	position += size;
	return size;
}

//-----------------------------------//

int64 MemoryStream::write(void* buffer, uint64 size)
{
	if(size <= 0) return -1;

	if(!useRawBuffer)
	{
		int64 newSize = position + size;
		bool needsResize = newSize > data.size();
	
		if(needsResize)
			resize(newSize);

		if(data.empty()) return 0;
	}

	uint8* cur = this->buffer + position;
	memcpy(cur, buffer, (size_t) size);

	position += size;
	return size;
}

//-----------------------------------//

int64 MemoryStream::getPosition() const
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
	return data.size();
}

//-----------------------------------//

void MemoryStream::resize(int64 size)
{
	if( size <= 0 ) return;

	data.resize((size_t)size);
	buffer = data.data();
}

//-----------------------------------//

NAMESPACE_CORE_END