/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_VFS_PHYSFS

#include "vapor/vfs/File.h"

#include <physfs.h>

namespace vapor {
	namespace vfs {

//-----------------------------------//

File::File(const std::string path, AccessMode::Enum e)
	: accessMode(e), path(path), file( nullptr )
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
	if( !file ) return;

	close();
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

	// close the file and check for errors
	int err = PHYSFS_close( file );

	if( err == 0 ) 
	{
		error( "vfs::file", "Could not close file '%s': %s",
			path.c_str(), PHYSFS_getLastError() );
		
		return false;
	}

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
	if(sz == -1) sz = getSize();
	std::vector<byte> buffer(sz); 
	if(!file || PHYSFS_eof(file)) return std::vector<byte>();
	
	PHYSFS_sint64 numObjs = PHYSFS_read (file, &buffer[0], 1, sz); 
		
	if(numObjs < 0){
			error( "vfs::file", "Could not read from file '%s': %s",
			path.c_str(), PHYSFS_getLastError() );
		
		return std::vector<byte>();	
	}
	return buffer;

	
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