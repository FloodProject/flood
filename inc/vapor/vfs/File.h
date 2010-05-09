/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_VFS_PHYSFS

struct PHYSFS_File;

/** \addtogroup virtual Virtual Filesystem 
 * @{ */

namespace vapor { namespace vfs {

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
 * Represents a virtual file in the virtual filesystem.
 * If you open a file for reading, it will be searched in the virtual 
 * filesystem and if found, a file pointer will be returned. If a write 
 * mode is used, a new file will be opened for writing in a special user
 * directory. The location of that directory will be platform-dependant, 
 * but you should use it. For example, different users can mantain game
 * data in the same machine, and it will be saved in each user's directory.
 */

class VAPOR_API File : private boost::noncopyable
{
public:

	/// Opens a new file given a path (defaults for reading access).
	File (const std::string path, AccessMode::Enum e = AccessMode::Read);
	
	/// Closes the file.
	~File ();

	/// Closes the file.
	bool close();
	
	/// Gets the file size.
	long getSize() const;

	/// Returns the contents of the file.
	std::vector<byte> read(long size = -1) const;

	/// Reads up to size bytes into the buffer.
	long read(void* buffer, long size) const;

	/// Read lines (assumes a text file).
	std::vector<std::string> readLines() const;

	/// Write buffer into file.
	long write(const std::vector<byte>& buffer, long size = -1);

	/// Write text into file.
	long write(const std::string& text);

	/// Seek to a new position within the file. 
	/// The next read or write will occur at that place.
	bool seek(long pos);

	/// Determine current position within the file.
	long tell() const;

	/// Gets the extension of the file (if there is one).
	const std::string getExtension() const;

	/// Gets the path of the file.
	IMPLEMENT_GETTER(Path, const std::string&, path)

	/// Gets the full path of the file.
	const std::string getFullPath() const;

	/// Checks if this file exists.
	bool exists() const;
	
	/// Checks if the file in path exists.
	static bool exists(const std::string& path);
	
private:

	/// Opens the file if it exists.
	bool open();

	/// Allowed access mode to the file.
	AccessMode::Enum accessMode;
	
	/// PhysFS handle to the file.
	PHYSFS_File* file;
	
	/// Virtual path of the file.
	std::string	path;

	/// Is the file already closed?
	bool closed;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( File );

//-----------------------------------//

} } // end namespaces

/** @} */

#endif