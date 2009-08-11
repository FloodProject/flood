/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_VFS_PHYSFS

struct PHYSFS_File;

#include "vapor/Platform.h"

namespace vapor {
	namespace vfs {

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

/**
 * Represents a virtual file in the virtual filesystem.
 * If you open a file for reading, it will be searched in the virtual 
 * filesystem and if found, a file pointer will be returned. If a write 
 * mode is used, a new file will be opened for writing in a special user
 * directory. The location of that directory will be platform-dependant, 
 * but you should use it. For example, different users can mantain game
 * data in the same machine, and it will be saved in each user's directory.
 */

class File 
{
public:

	File (const std::string path, AccessMode::Enum e = AccessMode::Read);
	File (const char* path, AccessMode::Enum e = AccessMode::Read);
	~File ();

	// Closes the file.
	bool close();
	
	// Gets the file size.
	long getSize();

	// Read file into buffer.
	std::vector<byte> read(long size = -1);

	// Gets the path of the file.
	const std::string getPath() const { return path; }

	// Checks if this file exists.
	bool exists();
	
	// Checks if the file in path exists.
	static bool exists(const std::string path);
	
private:

	// Opens the file if it exists.
	void open();

	AccessMode::Enum accessMode;
	PHYSFS_File *file;
	std::string	path;
};

} } // end namespaces

#endif