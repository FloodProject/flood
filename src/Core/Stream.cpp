/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "Stream.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

Stream::Stream(StreamMode::Enum mode, const std::string& path)
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

void Stream::read(std::string& text) const
{
	std::vector<byte> data;
	read(data);

	text.assign( data.begin(), data.end() );
}

//-----------------------------------//

std::vector<std::string> Stream::readLines() const
{
	std::string str;
	read(str);

	std::vector<std::string> lines = String::split(str, '\n');
	
	// Trim extra line endings that might be left.
	for( uint i = 0; i < lines.size(); i++ )
	{
		std::string& str = lines[i];
		
		if( str[str.size()-1] == '\r' )
			str.erase( str.size()-1 );
	}
	
	return lines;
}

//-----------------------------------//

long Stream::write(const std::string& string)
{
	std::vector<byte> data( string.begin(), string.end() );
	return write(data);  
}

//-----------------------------------//

} // end namespace