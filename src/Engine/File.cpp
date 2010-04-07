/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_VFS_PHYSFS

#include "vapor/vfs/File.h"
#include "vapor/Utilities.h"

#include <physfs.h>

namespace vapor { namespace vfs {

//-----------------------------------//

File::File(const std::string path, AccessMode::Enum e)
	: accessMode(e), path(path), file( nullptr ), closed( false )
{
	open();
}

//-----------------------------------//

File::File(const char* path, AccessMode::Enum e)
	: accessMode(e), path(path), file( nullptr ), closed( false )
{
	open();
}

//-----------------------------------//

File::~File() 
{
	close();
}

//-----------------------------------//

const std::string File::getFullPath() const
{
	// Get the full file path
	const char* realPath = PHYSFS_getRealDir( path.c_str() );
	
	std::string fullPath( realPath );
	fullPath.append( PHYSFS_getDirSeparator() );
	fullPath.append( path );

	return fullPath;
}

//-----------------------------------//

const std::string File::getExtension() const
{
	// Check if it has a file extension.
	uint ch = path.find_last_of( "." );

	if( ch == std::string::npos ) 
		return "";

	// Return the file extension.
	return path.substr( ++ch );
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
		return;
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

long File::getSize() const
{
	if( !file ) return -1;

	PHYSFS_sint64 sz = PHYSFS_fileLength( file );

	return static_cast< long >( sz );
}

//-----------------------------------//

std::vector<byte> File::read(long sz) const
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
	
	PHYSFS_sint64 bytesRead = PHYSFS_read (file, &buffer[0], 1, sz); 

	if(bytesRead < 0)
	{
		error( "vfs::file", "Could not read from file '%s': %s", 
			path.c_str(), PHYSFS_getLastError());

		return std::vector<byte>();	
	}

	return buffer;
}

//-----------------------------------//

long File::read(void* buffer, long size ) const
{
	if( accessMode != AccessMode::Read )
	{
		error( "vfs::file", "Access mode violation in file '%s'", 
			path.c_str() );
		
		return -1;
	}

	if( !file || PHYSFS_eof(file) ) 
	{
		return 0;
	}

	if ( tell()+size > getSize() )
	{
		size = getSize() - tell();
	}

	if( size == 0 ) return 0;
	
	PHYSFS_sint64 bytesRead = PHYSFS_read (file, buffer, 1, size); 

	if(bytesRead < 0)
	{
		error( "vfs::file", "Could not read from file '%s': %s", 
			path.c_str(), PHYSFS_getLastError());

		return -1;	
	}

	return static_cast< long >( bytesRead );
}

//-----------------------------------//

std::vector<std::string> File::readLines() const
{
	std::vector<byte> font = read();
	std::string str( font.begin(), font.end() );
	
	std::vector<std::string> lines = str_split(str, '\n');
	
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

long File::write(const std::vector<byte>& buffer, long size)
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
	}

	return static_cast< long >( numObjs );
}

//-----------------------------------//

long File::write(const std::string& text)
{
	std::vector<byte> data( text.begin(), text.end() );
	return write( data );
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

long File::tell() const
{
	if(!file) return -1;

	PHYSFS_sint64 pos = PHYSFS_tell(file);
	
	return static_cast< long >( pos );
}

//-----------------------------------//

bool File::exists() const
{
	return ( file && exists( path ) );
}

//-----------------------------------//

bool File::exists(const std::string& path)
{
	if( PHYSFS_exists( path.c_str() ) != 0 )
		return true;
	else
		return false;
}

//-----------------------------------//

} } // end namespaces

#endif