/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Use these for different kinds of access to the streams.
 */

namespace StreamMode
{
	enum Enum
	{
		Read,
		Write,
		Append
	};
}

//-----------------------------------//

/**
 * A stream allows uniform access to data backed by different storage
 * mediums. It can be stored in memory, files, archives, or even in
 * remove servers.
 */

class VAPOR_API Stream
{
	DECLARE_UNCOPYABLE(Stream)

public:

	virtual ~Stream();

	// Opens the stream.
	virtual bool open() = 0;

	// Closes the stream.
	virtual void close();

	// Reads the stream into a buffer.
	virtual long read(void* buffer, long size) const = 0;

	// Reads the entire stream.
	virtual void read(std::vector<byte>& data) const = 0;

	// Reads the entire stream as a string.
	virtual void read(std::string& string) const;

	// Writes a buffer into the stream.
	virtual long write(const std::vector<byte>& buffer) = 0;

	// Writes a string into the stream.
	virtual long write(const std::string& string);

	// Gets the path of the stream.
	GETTER(Path, const std::string&, path)

protected:

	Stream(StreamMode::Enum mode, const std::string& path);

	// Access mode of the stream.
	StreamMode::Enum mode;

	// Path of the stream.
	std::string	path;
};

//-----------------------------------//

} // end namespace