/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include <cstdio>

namespace vapor {

/**
 * Represents a native file managed by the OS.
 * This is a small utility wrapper class that will be extended as needed,
 * but usually you should use the File class of the  virtual filesystem 
 * (VFS) provided by the engine. This class was not properly tested, so 
 * use with caution. ;)
 */

class NativeFile 
{
public:

	NativeFile (std::string path);
	NativeFile (const char* path);
	~NativeFile ();

	// Closes the file.
	void close();
	
	// Gets the file size.
	long getSize();

	// Read file into buffer.
	long read(void* buffer, long size);

	// Gets the path of the file.
	const std::string getPath() const { return path; }

	// Checks if this file exists.
	bool exists();
	
	// Checks if the file in path exists.
	static bool exists(std::string path);
	
private:

	std::string	path;
	FILE* fp;
};

} // end namespace
