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
#include <physfs.h>

namespace vapor {

//-----------------------------------//

static std::string normalizePath(std::string path)
{
	std::replace( path.begin(), path.end(), '\\', '/' );

	std::string::size_type pos = 0;

	while( (pos = path.find("//", pos)) != std::string::npos )
	{
        path.replace( pos, 2, "/" );
        pos++;
    }

	pos = 0;

	while( (pos = path.find("../", pos)) != std::string::npos )
	{
        path.replace( pos, 3, "" );
        pos++;
    }

	return path;
}

//-----------------------------------//

File::File(const std::string& tempPath, FileMode::Enum mode)
	: mode(mode)
	, file(nullptr)
	, closed(false)
{
	path = normalizePath(tempPath);

	if( !open() )
		return;
}

//-----------------------------------//

File::~File() 
{
	close();
}

//-----------------------------------//

bool File::open()
{
	switch( mode )
	{
	case FileMode::Read:
		file = PHYSFS_openRead( getPath().c_str() );
		break;
	case FileMode::Write:
		file = PHYSFS_openWrite( getPath().c_str() );
		break;
	case FileMode::Append:
		file = PHYSFS_openAppend( getPath().c_str() );
		break;
	}

	if( !file )
	{
		log("Could not open file");
		return false;
	}

	return true;
}

//-----------------------------------//

bool File::close()
{
	if( !file ) return false;
	if( closed ) return true;

	// Close the file and check for errors.
	int err = PHYSFS_close( file );

	if( !err ) 
		return false;

	file = nullptr;
	closed = true;

	return true;
}

//-----------------------------------//

void File::log(const std::string& err) const
{
	error( "file", "%s '%s': %s", err.c_str(), getPath().c_str(),
		PHYSFS_getLastError() );
}

//-----------------------------------//

long File::getSize() const
{
	if( !file )
		return -1;

	PHYSFS_sint64 sz = PHYSFS_fileLength( file );

	return (long) sz;
}

//-----------------------------------//

bool File::validate(FileMode::Enum check) const
{
	if( mode == check )
		return true;

	error( "file", "Access mode violation in file '%s'", getName().c_str() );
	return false;
}

//-----------------------------------//

std::vector<byte> File::read(long sz) const
{
	std::vector<byte> buffer;

	if( !(validate(FileMode::Read) || validate(FileMode::Append)) )
		return buffer;

	if( !file || PHYSFS_eof(file) ) 
		return buffer;

	if( sz == -1 )
		sz = getSize() ;
	else if ( tell()+sz > getSize() )
		sz = getSize() - tell();

	if( sz == 0 )
		return buffer;
	
	buffer.resize(sz); 
	PHYSFS_sint64 bytesRead = PHYSFS_read(file, &buffer[0], 1, sz); 

	if(bytesRead < 0)
	{
		log("Could not read from file");
		buffer;	
	}

	return buffer;
}

//-----------------------------------//

long File::read(void* buffer, long size ) const
{
	if( !(validate(FileMode::Read) || validate(FileMode::Append)) )
		return -1;

	if( !file || PHYSFS_eof(file) ) 
		return 0;

	if( tell()+size > getSize() )
		size = getSize() - tell();

	if( size == 0 )
		return 0;
	
	PHYSFS_sint64 bytesRead = PHYSFS_read (file, buffer, 1, size); 

	if(bytesRead < 0)
	{
		log("Could not read from file");
		return -1;	
	}

	return (long) bytesRead;
}

//-----------------------------------//

std::vector<std::string> File::readLines() const
{
	std::vector<byte> data = read();
	std::string str( data.begin(), data.end() );
	
	std::vector<std::string> lines = String::split(str, '\n');
	
	// Trim excess \r fat that can be left over.
	foreach( std::string& str, lines )
	{
		if( str[str.size()-1] == '\r' )
			str.erase( str.size()-1 );
	}
	
	return lines;
}

//-----------------------------------//

long File::write(const std::vector<byte>& buffer, long size)
{
	if( !validate(FileMode::Write) )
		return -1;

	if( (size < 0) || ((ulong) size > buffer.size()) )
		size = buffer.size();

	PHYSFS_sint64 bytesWritten = PHYSFS_write(file, &buffer[0], 1, size);
	
	if(bytesWritten < 0)
		log("Could not write to file");

	return (long) bytesWritten;
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
	if(!file)
		return false;
	
	if(pos < 0 || pos >= getSize())
	{
		log("Attempt to access a position out of bounds");
		return false;
	}
	
	int err = PHYSFS_seek(file, pos);
	
	if(!err)
		log("Failure to seek");

	return err != 0;
}

//-----------------------------------//

long File::tell() const
{
	if(!file) return -1;

	PHYSFS_sint64 pos = PHYSFS_tell(file);
	
	return static_cast<long>( pos );
}

//-----------------------------------//

bool File::exists() const
{
	return file && exists( getPath() );
}

//-----------------------------------//

bool File::exists(const std::string& path)
{
	return PHYSFS_exists(path.c_str());
}

//-----------------------------------//

const std::string File::getPath() const
{
	return path;
}

//-----------------------------------//

const std::string File::getRealPath() const
{
	// Gets the full file path.
	const char* realPath = PHYSFS_getRealDir( getPath().c_str() );
	
	std::string fullPath( realPath );
	fullPath.append( "/" );
	fullPath.append( getPath() );

	return normalizePath(fullPath);
}

//-----------------------------------//

const std::string File::getName() const
{
	return String::split( getPath(), '/' ).back();
}

//-----------------------------------//

const std::string File::getExtension() const
{
	std::string name = getName();

	// Check if it has a file extension.
	size_t ch = name.find_last_of(".");

	if( ch == std::string::npos ) 
		return "";

	// Return the file extension.
	return name.substr( ++ch );
}

//-----------------------------------//

} // end namespace

#endif