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
 * Use these for different kinds of access to the files.
 * Check the File class description for more information.
 */

namespace AccessMode
{
	enum Enum
	{
		Read,
		Write,
	};
}

//-----------------------------------//

/**
 * A stream allows uniform access to data backed by different storage
 * mediums. It can be stored in memory, files, archives, or even in
 * remove servers.
 */

class VAPOR_API Stream : private boost::noncopyable
{
public:

	Stream(AccessMode::Enum mode);
	virtual ~Stream();

	// Opens the stream.
	virtual bool open() = 0;

	// Closes the stream.
	virtual void close();

	// Reads the stream into a buffer.
	virtual long read(void* buffer, long size) = 0;

	// Reads the entire stream.
	virtual std::vector<byte> read() const = 0;

	// Writes a buffer into the stream.
	virtual long write(const std::vector<byte>& buffer) = 0;

	// Reads the stream as a string.
	std::string readString() const;

	// Writes a string into the stream.
	long writeString(const std::string& str);

	// Gets the path of the stream.
	GETTER(Path, const std::string&, path)

protected:

	std::string	path;
	AccessMode::Enum mode;
};

//-----------------------------------//

} // end namespace
