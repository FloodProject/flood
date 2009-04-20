/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"

#include <cstdio>

namespace vapor {
	namespace vfs {

/**
 * Represents a file.
 */

class File {

public:

	File (string path);
	~File ();

	/**
	 * Closes the file.
	 */
	void close();
	
	/**
	 * Gets the file size.
	 */
	long getSize();

	/**
	 * Read file to buffer.
	 */
	long read(void* buffer, long size);

	/**
	 * Gets the path of the file.
	 */
	string getPath() { return _path; }

	/**
	 * Checks if a file exists.
	 */
	static bool exists(string path);

private:

	string	_path;
	FILE*	_fp;
};

} } // end namespaces
