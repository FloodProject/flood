/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Stream.h"
#include "vapor/vfs/File.h"

namespace vapor {

//-----------------------------------//

/**
 * Wraps an PhysFS file handle into a stream.
 */

class VAPOR_API PhysfsStream : public Stream
{
public:

	PhysfsStream(File& file);

	// Opens the stream.
	virtual bool open();

	// Closes the stream.
	virtual void close();

	// Reads the stream into a buffer.
	virtual long read(void* buffer, long size);

	// Reads the entire stream.
	virtual std::vector<byte> read() const;

	// Writes a buffer into the stream.
	virtual long write(const std::vector<byte>& buffer);

protected:

	File& file;
};

//-----------------------------------//

} // end namespace
