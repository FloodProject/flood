/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/PhysfsStream.h"

#ifdef VAPOR_VFS_PHYSFS

namespace vapor {

//-----------------------------------//

PhysfsStream::PhysfsStream(File& file)
	: Stream(StreamMode::Read, file.Path)
	, file(file)
{ }

//-----------------------------------//

bool PhysfsStream::open()
{
	FileOpen(&file);
	return true;
}

//-----------------------------------//

void PhysfsStream::close()
{
	FileClose(&file);
}

//-----------------------------------//

long PhysfsStream::read(void* buffer, long size) const
{
	return FileReadBuffer(&file, buffer, size);
}

//-----------------------------------//

void PhysfsStream::read(std::vector<byte>& data) const
{
	FileRead(&file, data);
}

//-----------------------------------//

long PhysfsStream::write(const std::vector<byte>& data)
{
	return FileWrite(&file, data);
}

//-----------------------------------//

} // end namespace

#endif