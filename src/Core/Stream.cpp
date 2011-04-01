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

namespace vapor {

//-----------------------------------//

Stream* StreamCreateFromURI(Allocator*, const Path&)
{
	assert("TODO: Creating URIs from schemes");
	return nullptr;
}

//-----------------------------------//

bool StreamClose(Stream* stream)
{
	if(!stream) return false;
	return stream->fn->close(stream);
}

//-----------------------------------//

void StreamDestroy(Stream* stream, Allocator* mem)
{
	if( !StreamClose(stream) )
	{
		//LogDebug("Error closing stream: %s", stream->path.c_str());
		return;
	}

	Deallocate(mem, stream);
}

//-----------------------------------//

int64 StreamRead(Stream* stream, std::vector<uint8>& data)
{
	if( !stream ) return 0;
	
	data.resize( (size_t) StreamGetSize(stream) );
	return stream->fn->read(stream, &data[0], data.size());
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
	return stream->fn->write(stream, (void*) &data[0], data.size());
}

//-----------------------------------//

int64 StreamWriteString(Stream* stream, const String& string)
{
	std::vector<uint8> data( string.begin(), string.end() );
	return stream->fn->write(stream, &data[0], data.size());
}

//-----------------------------------//

int64 StreamGetPosition(Stream* stream)
{
	return stream->fn->tell(stream);
}

//-----------------------------------//

int64 StreamSetPosition(Stream* stream, int64 offset, StreamSeekMode::Enum mode)
{
	return stream->fn->seek(stream, offset, (int8) mode);
}

//-----------------------------------//

int64 StreamGetSize(Stream* stream)
{
	return stream->fn->size(stream);
}

//-----------------------------------//

} // end namespace