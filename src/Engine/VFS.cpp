/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_VFS_PHYSFS

#include <sstream>

#include "vapor/vfs/VFS.h"

#include <physfs.h>

namespace vapor {
	namespace vfs {

//-----------------------------------//

VFS::VFS(const std::string& app)
{
	int err = PHYSFS_init( nullptr );

	if( err == 0 ) 
	{
		error( "vfs",
			"Could not initialize PhysFS: %s", PHYSFS_getLastError() );
		
		return;
	}

	setDefaultConfig(app);
	log();
}

//-----------------------------------//

VFS::~VFS()
{
	std::vector< std::string >::iterator it;
	
	for( it = mountPoints.begin(); it != mountPoints.end(); it++)
	{
		PHYSFS_removeFromSearchPath( (*it).c_str() );
	}

	mountPoints.clear();

	int err = PHYSFS_deinit();

	if( err == 0 ) 
	{
		error( "vfs", 
			"Could not clean up PhysFS: %s", PHYSFS_getLastError() );
	}
}

//-----------------------------------//

void VFS::setDefaultConfig(const std::string& app)
{
	int err = PHYSFS_setSaneConfig("vapor", app.c_str(), nullptr, 0, 0);

	if( err == 0 ) 
	{
		error( "vfs",
			"Could not set a sane config: %s", PHYSFS_getLastError() );
	}

	info( "vfs", "Mounted '%s' in mount point '/'", PHYSFS_getBaseDir() );
}

//-----------------------------------//

void VFS::log()
{
	std::stringstream ss;

	PHYSFS_Version version;
	PHYSFS_getLinkedVersion(&version);

	info("vfs", "Initialized PhysFS version %d.%d.%d", 
		version.major, version.minor, version.patch);

	const PHYSFS_ArchiveInfo **i;

	ss << "Supported archives: ";

	for (i = PHYSFS_supportedArchiveTypes(); *i != nullptr; i++)
	{
		ss << "'" << (*i)->extension << "', ";
			// << "' (" << (*i)->description << "), ";
	}

	info( "vfs", "User write folder: %s", PHYSFS_getWriteDir() );
	
	std::string s = ss.str();
	s = s.substr( 0, s.find_last_of( ',' ) );
	info( "vfs", "%s", s.c_str() );
}

//-----------------------------------//

bool VFS::mount(const std::string& path, const std::string& mount, 
				bool append )
{
	int err = PHYSFS_mount( path.c_str(), mount.c_str(), append ? 1 : 0 );

	if( err == 0 ) 
	{
		error( "vfs", "Could not mount '%s': %s", 
			path.c_str(), PHYSFS_getLastError() );

		return false;
	}

	mountPoints.push_back( path );

	info( "vfs", "Mounted '%s' in mount point '%s'",
		path.c_str(), mount.empty() ? "/" : mount.c_str() );

	return true;
}

//-----------------------------------//

} } // end namespaces

#endif