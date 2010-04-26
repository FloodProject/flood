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
 * (VFS) provided by the engine. This class was not properly tested, so 
 * use with caution. ;)
 */

class VAPOR_API NativeFile : private boost::noncopyable
{
public:

	NativeFile (const std::string& path, AccessMode::Enum mode = AccessMode::Read );
	NativeFile (const char* path, AccessMode::Enum mode = AccessMode::Read);
	~NativeFile ();

	// Opens the file.
	bool open();

	// Closes the file.
	void close();
	
	// Gets the file size.
	long getSize();

	// Read file into buffer.
	long read(void* buffer, long size);

	// Write text into file.
	long write(const std::string& text);

	// Sets if the file should be buffered.
	void setBuffering( bool state );

	// Gets the path of the file.
	IMPLEMENT_GETTER(Path, const std::string, path)

	// Checks if this file exists.
	bool exists();
	
	// Checks if the file in path exists.
	static bool exists(std::string path);
	
protected:

	AccessMode::Enum mode;
	std::string	path;
	FILE* fp;
};

//-----------------------------------//

} // end namespace
