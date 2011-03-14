/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Stream.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

Stream::Stream(StreamMode::Enum mode, const String& path)
  : mode(mode)
  , path(path)
{ }

//-----------------------------------//

Stream::~Stream() 
{
	close();
}

//-----------------------------------//

void Stream::close()
{}

//-----------------------------------//

int Stream::tell()
{
	return -1;
}

//-----------------------------------//

void Stream::read(String& text) const
{
	std::vector<byte> data;
	read(data);

	text.assign( data.begin(), data.end() );
}

//-----------------------------------//

std::vector<String> Stream::readLines() const
{
	String str;
	read(str);

	std::vector<String> lines;
	StringSplit(str, '\n', lines);
	
	// Trim extra line endings that might be left.
	for( size_t i = 0; i < lines.size(); i++ )
	{
		String& str = lines[i];
		
		if( str[str.size()-1] == '\r' )
			str.erase( str.size()-1 );
	}
	
	return lines;
}

//-----------------------------------//

long Stream::write(const String& string)
{
	std::vector<byte> data( string.begin(), string.end() );
	return write(data);  
}

//-----------------------------------//

} // end namespace