/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Containers/Array.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Stream* StreamCreateFromURI(Allocator*, const Path&, StreamOpenMode)
{
	assert("TODO: Creating URIs from schemes");
	return nullptr;
}

//-----------------------------------//

bool StreamClose(Stream* stream)
{
	if(!stream || !stream->fn->close ) return false;
	return stream->fn->close(stream);
}

//-----------------------------------//

void StreamDestroy(Stream* stream)
{
	if( !stream ) return;

	if( !StreamClose(stream) )
	{
		LogDebug("Error closing stream: %s", stream->path.c_str());
	}
	
	Deallocate(stream);
}

//-----------------------------------//

int64 StreamRead(Stream* stream, Array<uint8>& data)
{
	if( !stream ) return 0;
	
	int64 length = StreamGetSize(stream);

	if( length < 0 ) return 0;

	array::resize(data, (size_t) length );

	if( array::empty(data) ) return 0;

	return StreamReadBuffer(stream, &array::front(data), array::size(data));
}

//-----------------------------------//

int64 StreamReadBuffer(Stream* stream, void* buffer, int64 size)
{
	if( !stream || !stream->fn->read ) return 0;
	return stream->fn->read(stream, buffer, size);
}

//-----------------------------------//

int64 StreamReadString(Stream* stream, String& text)
{
	Array<uint8> data(*AllocatorGetHeap());
	int64 size = StreamRead(stream, data);
	text.assign( array::begin(data), array::end(data) );
	return size;
}

//-----------------------------------//

int64 StreamReadLines(Stream* stream, Array<String*>& lines)
{
	String text;
	int64 size = StreamReadString(stream, text);

	StringSplit(text, '\n', lines);
	
	// Erase extra line endings.
	for( size_t i = 0; i < array::size(lines); ++i )
	{
		String& line = *lines[i];
		size_t last = line.size() - 1;
		if( line[last] == '\r' ) line.erase(last);
	}

	return size;
}

//-----------------------------------//

int64 StreamWrite(Stream* stream, uint8* buf, uint64 size)
{
	if( !stream || !stream->fn->write ) return -1;
	return stream->fn->write(stream, buf, size);
}

//-----------------------------------//

int64 StreamWriteString(Stream* stream, const String& string)
{
	return StreamWrite(stream, (uint8*) string.data(), string.size());
}

//-----------------------------------//

int64 StreamGetPosition(Stream* stream)
{
	if( !stream || !stream->fn->tell ) return -1;
	return stream->fn->tell(stream);
}

//-----------------------------------//

int64 StreamSetPosition(Stream* stream, int64 offset, StreamSeekMode mode)
{
	if( !stream || !stream->fn->seek ) return -1;
	return stream->fn->seek(stream, offset, (int8) mode);
}

//-----------------------------------//

int64 StreamGetSize(Stream* stream)
{
	if( !stream || !stream->fn->size ) return -1;
	return stream->fn->size(stream);
}

//-----------------------------------//

void StreamResize(Stream* stream, int64 size)
{
	if( !stream || !stream->fn->resize ) return;
	stream->fn->resize(stream, size);
}

//-----------------------------------//

NAMESPACE_CORE_END