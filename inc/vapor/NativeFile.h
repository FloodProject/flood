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
		Append
	};
}

//-----------------------------------//

/**
 * Represents a native file managed by the OS.
 * This is a small utility wrapper class that will be extended as needed,
 * but usually you should use the File class of the  virtual filesystem 
 * (VFS) provided by the engine.
 */

class VAPOR_API NativeFile : private boost::noncopyable
{
public:

	NativeFile(const std::string& path, 
		AccessMode::Enum mode = AccessMode::Read);

	NativeFile(const char* path,
		AccessMode::Enum mode = AccessMode::Read);
	
	~NativeFile ();

	// Opens the file.
	bool open();

	// Closes the file.
	void close();

	// Read file into buffer.
	long read(void* buffer, long size) const;

	// Read entire file.
	std::vector<byte> read() const;

	// Read entire file as string.
	std::string readString() const;

	// Write text into file.
	long write(const std::string& text);

	// Write buffer into file.
	long write(const std::vector<byte>& buf);

	// Gets the file size.
	long getSize() const;

	// Gets the path of the file.
	GETTER(Path, const std::string, path)

	// Sets if the file should be buffered.
	void setBuffering(bool state);

	// Checks if this file exists.
	bool exists() const;
	
	// Checks if the file in path exists.
	static bool exists(const std::string& path);
	
protected:

	std::string	path;
	AccessMode::Enum mode;

	FILE* fp;
};

//-----------------------------------//

} // end namespace
