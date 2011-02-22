/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/File.h"

#ifdef VAPOR_VFS_PHYSFS

#include "Log.h"
#include "Utilities.h"

#include <physfs.h>

namespace vapor {

//-----------------------------------//

File::File(const std::string& tempPath, StreamMode::Enum mode)
	: mode(mode)
	, file(nullptr)
	, closed(false)
{
	path = PathUtils::normalize(tempPath);
	path = PathUtils::getFile(path);

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
	case StreamMode::Read:
		file = PHYSFS_openRead( getPath().c_str() );
		break;
	case StreamMode::Write:
		file = PHYSFS_openWrite( getPath().c_str() );
		break;
	case StreamMode::Append:
		file = PHYSFS_openAppend( getPath().c_str() );
		break;
	default:
		assert( false );
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
	Log::error( "%s '%s': %s", err.c_str(),
		getPath().c_str(), PHYSFS_getLastError() );
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

bool File::validate(StreamMode::Enum check) const
{
	if( mode == check )
		return true;

	Log::error( "Access mode violation in file '%s'", getName().c_str() );
	return false;
}

//-----------------------------------//

bool File::read(std::vector<byte>& data, long size) const
{
	if( !(validate(StreamMode::Read) || validate(StreamMode::Append)) )
		return false;

	if( !file || PHYSFS_eof(file) ) 
		return false;

	if( size == -1 )
		size = getSize() ;
	else if ( tell()+size > getSize() )
		size = getSize() - tell();

	if( size == 0 )
		return false;
	
	data.resize(size); 
	PHYSFS_sint64 bytesRead = PHYSFS_read(file, &data[0], 1, size);

	if(bytesRead < 0)
	{
		log("Could not read from file");
		return false;	
	}

	return true;
}

//-----------------------------------//

long File::read(void* buffer, long size) const
{
	if( !(validate(StreamMode::Read) || validate(StreamMode::Append)) )
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
	std::vector<byte> data;
	read( data );

	std::string str( data.begin(), data.end() );
	std::vector<std::string> lines = String::split(str, '\n');
	
	// Trim excess fat that can be left over.
	for( uint i = 0; i < lines.size(); i++ )
	{
		std::string& str = lines[i];
		if( str[str.size()-1] == '\r' )
			str.erase( str.size()-1 );
	}
	
	return lines;
}

//-----------------------------------//

long File::write(const std::vector<byte>& buffer, long size)
{
	if( !validate(StreamMode::Write) )
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
	return PHYSFS_exists(path.c_str()) != 0;
}

//-----------------------------------//

const Path File::getPath() const
{
	return path;
}

//-----------------------------------//

const Path File::getRealPath() const
{
	// Gets the full file path.
	const char* realPath = PHYSFS_getRealDir( getPath().c_str() );
	
	Path fullPath( realPath );
	fullPath.append( "/" );
	fullPath.append( getPath() );

	return PathUtils::normalize(fullPath);
}

//-----------------------------------//

const Path File::getName() const
{
	return String::split( path, '/' ).back();
}

//-----------------------------------//

const Path File::getExtension() const
{
	Path name = getName();
	return PathUtils::getExtension(name);
}

//-----------------------------------//

} // end namespace

#endif