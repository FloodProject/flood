/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Log.h"
#include "Core/Memory.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Stream::Stream(const String& path, StreamOpenMode mode)
	: path(path)
	, mode(mode) 
{
}

//-----------------------------------//

bool Stream::open() 
{
	return false; 
}

//-----------------------------------//

bool Stream::close() 
{ 
	return false; 
}

//-----------------------------------//

int64 Stream::read(void* buffer, uint64 size) const
{ 
	return InvalidState; 
}	
	
//-----------------------------------//	
	
int64 Stream::write(void* buffer, uint64 size) 
{
	return InvalidState; 
}

//-----------------------------------//

int64 Stream::getPosition() const
{
	return InvalidState; 
}

//-----------------------------------//

void Stream::setPosition(int64 pos, StreamSeekMode mode) 
{
}

//-----------------------------------//

uint64 Stream::size() const
{ 
	return InvalidState; 
}

//-----------------------------------//

void Stream::resize(int64 size) 
{
}

//-----------------------------------//

int64 Stream::read(std::vector<uint8>& data) const
{
	int64 length = size();

	if( length < 0 ) return 0;

	data.resize( (size_t) length );

	if( data.empty() ) return 0;

	return readBuffer(&data.front(), data.size());
}

//-----------------------------------//

int64 Stream::readBuffer(void* buffer, int64 size) const
{
	return read(buffer, size);
}

//-----------------------------------//

int64 Stream::readString(String& text) const
{
	std::vector<uint8> data;
	int64 size = read(data);
	text.assign( data.begin(), data.end() );
	return size;
}

//-----------------------------------//

int64 Stream::readLines(std::vector<String>& lines) const
{
	String text;
	int64 size = readString(text);

	StringSplit(text, '\n', lines);
	
	// Erase extra line endings.
	for( size_t i = 0; i < lines.size(); i++ )
	{
		String& line = lines[i];
		size_t last = line.size() - 1;
		if( line[last] == '\r' ) line.erase(last);
	}

	return size;
}

//-----------------------------------//

int64 Stream::write(uint8* buf, uint64 size)
{
	return write((void *)buf, size);
}

//-----------------------------------//

int64 Stream::writeString(const String& string)
{
	return write((uint8*) string.data(), string.size());
}

//-----------------------------------//

NAMESPACE_CORE_END