/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "Stream.h"

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

void Stream::read(std::string& str) const
{
	std::vector<byte> lines;
	read(lines);

	str.assign( lines.begin(), lines.end() );
}

//-----------------------------------//

long Stream::write(const std::string& string)
{
	std::vector<byte> data( string.begin(), string.end() );
	return write(data);  
}

//-----------------------------------//

} // end namespace