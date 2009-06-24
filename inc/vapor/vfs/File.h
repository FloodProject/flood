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
	namespace vfs {

/**
 * Represents a file.
 */

class File 
{
public:

	File (string path);
	File (const char* path);
	~File ();

	// Closes the file.
	void close();
	
	// Gets the file size.
	long getSize();

	// Read file into buffer.
	long read(void* buffer, long size);

	// Gets the path of the file.
	const string getPath() const { return path; }

	// Checks if this file exists.
	bool exists();
	
	// Checks if the file in path exists.
	static bool exists(string path);
	

private:

	string	path;
	FILE*	fp;
};

} } // end namespaces
