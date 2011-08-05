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

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static bool  MemoryOpen(Stream*);
static bool  MemoryClose(Stream*);
static int64 MemoryRead(Stream*, void*, int64);
static int64 MemoryWrite(Stream*, void*, int64);
static int64 MemoryTell(Stream*);
static bool  MemorySeek(Stream*, int64, int8);
static int64 MemoryGetSize(Stream*);
static void  MemoryResize(Stream*, int64 size);

static StreamFuncs gs_MemoryFuncs = 
{
	MemoryOpen,
	MemoryClose,
	MemoryRead,
	MemoryWrite,
	MemoryTell,
	MemorySeek,
	MemoryGetSize,
	MemoryResize
};

//-----------------------------------//

MemoryStream* StreamCreateFromMemory(Allocator* alloc, uint64 size)
{
	MemoryStream* ms = Allocate(MemoryStream, alloc);
	if( !ms ) return nullptr;
	
	ms->position = 0;
	ms->fn = &gs_MemoryFuncs;

	if( !MemoryOpen(ms) )
	{
		Deallocate(ms);
		return nullptr;
	}

	MemoryResize(ms, size);

	return ms;
}

//-----------------------------------//

static bool MemoryOpen(Stream* stream)
{
	MemoryStream* ms = (MemoryStream*) stream;
	return ms != nullptr;
}

//-----------------------------------//

static bool MemoryClose(Stream* stream)
{
	MemoryStream* ms = (MemoryStream*) stream;
	ms->data.clear();
	return true;
}

//-----------------------------------//

static int64 MemoryRead(Stream* stream, void* buffer, int64 size)
{
	if(size < 0) return -1;

	MemoryStream* ms = (MemoryStream*) stream;
	int64 left = MemoryGetSize(stream) - ms->position;

	if(size > left) size = left;
	
	uint8* cur = &ms->data[0] + ms->position;
	memcpy(buffer, cur, (size_t) size);

	ms->position += size;
	return size;
}

//-----------------------------------//

static int32 GetNextPower2(int32 v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

//-----------------------------------//

static int64 MemoryWrite(Stream* stream, void* buffer, int64 size)
{
	if(size < 0) return -1;

	MemoryStream* ms = (MemoryStream*) stream;
	uint64& position = ms->position;

	int64 newSize = MemoryGetSize(stream) + size;
	bool needsResize = newSize > ms->data.size();
	
	if(needsResize)
		MemoryResize(stream, GetNextPower2((int32)newSize));

	uint8* cur = &ms->data[0] + position;
	memcpy(cur, buffer, (size_t) size);

	position += size;
	return size;
}

//-----------------------------------//

static int64 MemoryTell(Stream* stream)
{
	MemoryStream* ms = (MemoryStream*) stream;
	return ms->position;
}

//-----------------------------------//

static bool MemorySeek(Stream* stream, int64 offset, int8 mode)
{
	MemoryStream* ms = (MemoryStream*) stream;

	switch(mode)
	{
	case StreamSeekMode::Absolute:
		ms->position = offset;
		break;
	case StreamSeekMode::Relative:
		ms->position += offset;
		break;
	case StreamSeekMode::RelativeEnd:
		ms->position = MemoryGetSize(stream) - offset;
		break;
	}

	return true;
}

//-----------------------------------//

static int64 MemoryGetSize(Stream* stream)
{
	MemoryStream* ms = (MemoryStream*) stream;
	return ms->data.size();
}

//-----------------------------------//

static void MemoryResize(Stream* stream, int64 size)
{
	MemoryStream* ms = (MemoryStream*) stream;
	ms->data.resize((size_t)size);
}

//-----------------------------------//

NAMESPACE_CORE_END