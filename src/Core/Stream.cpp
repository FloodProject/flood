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

Stream::Stream(const Path& path, StreamOpenMode mode)
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

uint64 Stream::read(void* buffer, uint64 size) const
{ 
    return InvalidState; 
}   
    
//-----------------------------------// 
    
uint64 Stream::write(void* buffer, uint64 size) 
{
    return InvalidState; 
}

//-----------------------------------//

uint64 Stream::getPosition() const
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

uint64 Stream::read(Vector<uint8>& data) const
{
    int64 length = size();

    if( length < 0 ) return 0;

    data.Resize( (size_t) length );

    if( data.Empty() ) return 0;

    return readBuffer(&data.Front(), data.Size());
}

//-----------------------------------//

uint64 Stream::readBuffer(void* buffer, int64 size) const
{
    return read(buffer, size);
}

//-----------------------------------//

uint64 Stream::readString(String& text) const
{
    Vector<uint8> data;
    int64 size = read(data);
    text.Clear();
    for(auto i:data)
        text += i;
    return size;
}

//-----------------------------------//

uint64 Stream::readUTF8String(UTF8String& text) const
{
    Vector<uint8> data;
    int64 size = read(data);
    text.Clear();
    for(byte i:data)
        text.Append(i);
    return size;
}

//-----------------------------------//

uint64 Stream::readLines(Vector<String>& lines) const
{
    String text;
    int64 size = readString(text);

    lines = text.Split('\n');
    
    // Erase extra line endings.
    for( size_t i = 0; i < lines.Size(); i++ )
    {
        String& line = lines[i];
        size_t last = line.Length() - 1;
        if( line[last] == '\r' ) line.Erase(last);
    }

    return size;
}

//-----------------------------------//

uint64 Stream::readUTF8Lines(Vector<UTF8String>& lines) const
{
    UTF8String text;
    int64 size = readUTF8String(text);

    lines = text.Split('\n');
    
    // Erase extra line endings.
    for( size_t i = 0; i < lines.Size(); i++ )
    {
        UTF8String& line = lines[i];
        size_t last = line.Length() - 1;
        if( line[last] == '\r' ) line.Erase(last);
    }

    return size;
}

//-----------------------------------//

uint64 Stream::write(uint8* buf, uint64 size)
{
    return write((void *)buf, size);
}

//-----------------------------------//

uint64 Stream::writeUTF8String(const UTF8String& string)
{
    return write((uint8*) string.CString(), string.Length());
}

//-----------------------------------//

uint64 Stream::writeString(const String& string)
{
    return write((uint8*) string.CString(), string.Length());
}

//-----------------------------------//

NAMESPACE_CORE_END