/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Log.h"
#include "Core/Memory.h"

NAMESPACE_BEGIN

//-----------------------------------//

Stream* StreamCreateFromURI(Allocator*, const Path&)
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
#if 0
	if( !StreamClose(stream) )
	{
		LogDebug("Error closing stream: %s", stream->path.c_str());
		return;
	}
#endif

	StreamClose(stream);
	Deallocate(stream);
}

//-----------------------------------//

int64 StreamRead(Stream* stream, std::vector<uint8>& data)
{
	if( !stream ) return 0;
	
	int64 length = StreamGetSize(stream);
	data.resize( (size_t) length );

	if( data.empty() ) return 0;

	return StreamReadBuffer(stream, &data.front(), data.size());
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
	std::vector<uint8> data;
	int64 size = StreamRead(stream, data);
	text.assign( data.begin(), data.end() );
	return size;
}

//-----------------------------------//

int64 StreamReadLines(Stream* stream, std::vector<String>&)
{
	String str;
	int64 size = StreamReadString(stream, str);

	std::vector<String> lines;
	StringSplit(str, '\n', lines);
	
	// Trim extra line endings that might be left.
	for( size_t i = 0; i < lines.size(); i++ )
	{
		String& str = lines[i];
		
		if( str[str.size()-1] == '\r' )
			str.erase( str.size()-1 );
	}

	return size;
}

//-----------------------------------//

int64 StreamWrite(Stream* stream, const std::vector<uint8>& data)
{
	if( !stream || !stream->fn->write ) return -1;
	return stream->fn->write(stream, (void*) &data[0], data.size());
}

//-----------------------------------//

int64 StreamWriteString(Stream* stream, const String& string)
{
	std::vector<uint8> data( string.begin(), string.end() );
	return StreamWrite(stream, data);
}

//-----------------------------------//

int64 StreamGetPosition(Stream* stream)
{
	if( !stream->fn->tell ) return -1;
	return stream->fn->tell(stream);
}

//-----------------------------------//

int64 StreamSetPosition(Stream* stream, int64 offset, StreamSeekMode::Enum mode)
{
	if( !stream->fn->seek ) return -1;
	return stream->fn->seek(stream, offset, (int8) mode);
}

//-----------------------------------//

int64 StreamGetSize(Stream* stream)
{
	if( !stream->fn->size ) return -1;
	return stream->fn->size(stream);
}

//-----------------------------------//

NAMESPACE_END