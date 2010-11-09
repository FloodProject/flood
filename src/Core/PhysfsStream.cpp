/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vfs/PhysfsStream.h"

namespace vapor {

//-----------------------------------//

PhysfsStream::PhysfsStream(File& file)
	: Stream(StreamMode::Read)
	, file(file)
	//, path(file.getPath())
{ }

//-----------------------------------//

bool PhysfsStream::open()
{
	return true;//file.open();
}

//-----------------------------------//

void PhysfsStream::close()
{
	file.close();
}

//-----------------------------------//

long PhysfsStream::read(void* buffer, long size) const
{
	return file.read(buffer, size);
}

//-----------------------------------//

std::vector<byte> PhysfsStream::read() const
{
	return file.read();
}

//-----------------------------------//

long PhysfsStream::write(const std::vector<byte>& buffer)
{
	return file.write(buffer);
}

//-----------------------------------//

} // end namespace