/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Stream.h"
#include "Core/File.h"

#ifdef VAPOR_VFS_PHYSFS

namespace vapor {

//-----------------------------------//

/**
 * Wraps an PhysFS file handle into a stream.
 */

class CORE_API PhysfsStream : public Stream
{
public:

	PhysfsStream(File& file);

	// Opens the stream.
	virtual bool open();

	// Closes the stream.
	virtual void close();

	// Reads the stream into a buffer.
	virtual long read(void* buffer, long size) const;

	// Reads the entire stream.
	virtual void read(std::vector<byte>& data) const;

	// Writes a buffer into the stream.
	virtual long write(const std::vector<byte>& buffer);

	using Stream::read;
	using Stream::write;

protected:

	mutable File& file;
};

//-----------------------------------//

} // end namespace

#endif