/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Stream.h"

namespace vapor {

//-----------------------------------//

/**
 * Wraps a file in an I/O stream.
 */

class CORE_API FileStream : public Stream
{
	DECLARE_UNCOPYABLE(FileStream)

public:

	FileStream(const std::string& path, StreamMode::Enum mode = StreamMode::Read);
	virtual ~FileStream();

	// Opens the file.
	bool open();

	// Closes the file.
	void close();

	// Read file into buffer.
	long read(void* buffer, long size) const;

	// Read entire file.
	void read(std::vector<byte>& data) const;

	// Write buffer into file.
	long write(const std::vector<byte>& buf);

	// Gets the file size.
	long getSize() const;

	// Sets if the file should be buffered.
	void setBuffering(bool state);

	// Checks if this file exists.
	bool exists() const;
	
	// Checks if the file in path exists.
	static bool exists(const std::string& path);

	// Gets the internal pointer of the file.
	FILE* getFilePointer();

	using Stream::read;
	using Stream::write;
	
protected:

	FILE* fp;
};

//-----------------------------------//

} // end namespace
