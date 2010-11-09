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

Stream::Stream(StreamMode::Enum mode)
  : mode(mode)
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

std::string Stream::readString() const
{
	std::vector<byte> lines = read();
	std::string str( lines.begin(), lines.end() );
	return str;
}

//-----------------------------------//

long Stream::write(const std::string& string)
{
	std::vector<byte> data( string.begin(), string.end() );
	return write(data);  
}

//-----------------------------------//

} // end namespace