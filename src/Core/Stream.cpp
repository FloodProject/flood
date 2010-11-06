/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vapor/Stream.h"

namespace vapor {

//-----------------------------------//

Stream::Stream(AccessMode::Enum mode)
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

long Stream::writeString(const std::string& str)
{
	std::vector<byte> data( str.begin(), str.end() );
	return write(data);  
}

//-----------------------------------//

} // end namespace