/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_VFS_PHYSFS

#include "vapor/vfs/File.h"

#include <physfs.h>

namespace vapor {
	namespace vfs {

//-----------------------------------//

File::File(const std::string path, AccessMode::Enum e)
	: accessMode(e), path(path), file( nullptr ), closed( false )
{
	open();
}

//-----------------------------------//

File::File(const char* path, AccessMode::Enum e)
	: accessMode(e), path(path), file( nullptr )
{
	open();
}

//-----------------------------------//

File::~File() 
{
	close();
}

//-----------------------------------//

const std::string& File::getPath() const
{
	return path;
}

//-----------------------------------//

void File::open()
{
	switch( accessMode )
	{
	case AccessMode::Read:
		file = PHYSFS_openRead( path.c_str() );
		break;
	case AccessMode::Write:
		file = PHYSFS_openWrite( path.c_str() );
		break;
	case AccessMode::Append:
		file = PHYSFS_openAppend( path.c_str() );
		break;
	}

	if ( file == nullptr )
	{
		error( "vfs::file", "Could not open file '%s': %s",
			path.c_str(), PHYSFS_getLastError() );
	}
}

//-----------------------------------//

bool File::close()
{
	if( !file ) return false;
	if( closed ) return true;

	// close the file and check for errors
	int err = PHYSFS_close( file );

	if( err == 0 ) 
	{
		error( "vfs::file", "Could not close file '%s': %s",
			path.c_str(), PHYSFS_getLastError() );

		return false;
	}

	file = nullptr;
	closed = true;

	return true;
}

//-----------------------------------//

long File::getSize()
{
	if( !file ) return -1;

	PHYSFS_sint64 sz = PHYSFS_fileLength( file );

	return sz;
}

//-----------------------------------//

std::vector<byte> File::read(long sz)
{
	if( accessMode != AccessMode::Read )
	{
		error( "vfs::file", "Access mode violation in file '%s'", 
			path.c_str() );
		
		return std::vector<byte>();
	}

	if( !file || PHYSFS_eof(file) ) 
	{
		return std::vector<byte>();
	}

	if( sz == -1 )
	{
		sz = getSize() ;
	} 
	else if ( tell()+sz > getSize() )
	{
		sz = getSize() - tell();
	}

	if( sz == 0 ) return std::vector<byte>();
	
	std::vector<byte> buffer( sz ); 
	
	PHYSFS_sint64 numObjs = PHYSFS_read (file, &buffer[0], 1, sz); 

	if(numObjs < 0)
	{
		error( "vfs::file", "Could not read from file '%s': %s", 
			path.c_str(), PHYSFS_getLastError());

		return std::vector<byte>();	
	}

	return buffer;
}

//-----------------------------------//

std::vector<std::string> File::readLines()
{
	std::vector<byte> font = read();
	std::string str( font.begin(), font.end() );
	
	std::vector<std::string> lines = split(str, '\n');
	
	// trim excess \r fat that can be left over
	foreach( std::string& str, lines )
	{
		if( str[str.size()-1] == '\r' )
		{
			str.erase( str.size()-1 );
		}
	}
	
	return lines;
}

//-----------------------------------//

long File::write(std::vector<byte> buffer, long size)
{
	if( (accessMode != AccessMode::Write)
		&& (accessMode != AccessMode::Append) ) 
	{
		error( "vfs::file", "Access mode violation in file '%s'", 
			path.c_str());

		return -1;
	}

	if( !file ) return -1;

	if( (size < 0) || ((ulong) size > buffer.size()) )
	{
		size = buffer.size();
	}

	PHYSFS_sint64 numObjs = PHYSFS_write(file, &buffer[0], 1, size);
	
	if(numObjs < 0)
	{
		error( "vfs::file", "Could not write to file '%s': %s",
			path.c_str(), PHYSFS_getLastError() );

		return numObjs;	
	}

	return numObjs;
}

//-----------------------------------//

bool File::seek(long pos)
{
	if(!file) return false;
	
	if( (pos < 0) || (pos >= getSize()) )
	{
		error( "vfs::file", 
			"Attempting to access a position out of bounds in file '%s': %d",
			path.c_str(), pos);
		
		return false;
	}
	
	int err = PHYSFS_seek(file, pos);
	
	if(err == 0)
	{
		error( "vfs::file", "Failure to seek in file '%s': %s", 
			path.c_str(), PHYSFS_getLastError() );
	}

	return err != 0;
}

//-----------------------------------//

long File::tell()
{
	if(!file) return -1;

	PHYSFS_sint64 pos = PHYSFS_tell(file);
	
	return pos;
}

//-----------------------------------//

bool File::exists()
{
	return ( file && exists( path ) );
}

//-----------------------------------//

bool File::exists(const std::string path)
{
	if( PHYSFS_exists( path.c_str() ) != 0 )
		return true;
	else
		return false;
}

//-----------------------------------//

} } // end namespaces

#endif