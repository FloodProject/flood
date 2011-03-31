/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef VAPOR_VFS_PHYSFS

#include "Core/Log.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "../Win32/FileWatcherWin32.h"

#include <physfs.h>
#include <sstream>

namespace vapor {

//-----------------------------------//

FileSystem::FileSystem(const String& app, const char* argv0 )
	: fileWatcher(nullptr)
{
	if( !PHYSFS_init(argv0) ) 
	{
		logError("Could not initialize PhysFS");	
		return;
	}

	setDefaultConfig(app);
	log();

#ifdef VAPOR_VFS_FILEWATCHER
	#ifdef VAPOR_PLATFORM_WINDOWS
		fileWatcher = new FileWatcherWin32();
	#endif
#endif
}

//-----------------------------------//

FileSystem::~FileSystem()
{
	for( uint i = 0; i < mountPoints.size(); i++ )
	{
		const String& point = mountPoints[i];
		PHYSFS_removeFromSearchPath( point.c_str() );
	}
	
	if( !PHYSFS_deinit() )
		logError( "Could not clean up PhysFS" );

	delete fileWatcher;
}

//-----------------------------------//

void FileSystem::setDefaultConfig(const String& app)
{
	int err = PHYSFS_setSaneConfig("vapor", app.c_str(), nullptr, 0, 0);

	if( err == 0 )
	{
		logError( "Could not set a sane config" );
		return;
	}

	LogInfo( "Mounted '%s' in mount point '/'", PHYSFS_getBaseDir() );
}

//-----------------------------------//

std::vector<String> FileSystem::enumerateFiles(const String& path)
{
	char **rc = PHYSFS_enumerateFiles( path.c_str() );
	
	std::vector<String> files;
	
	for (char** i = rc; *i != nullptr; i++)
		files.push_back(*i);	
		
	PHYSFS_freeList(rc);
	
	return files;
}

//-----------------------------------//

bool FileSystem::mount(const String& path, const String& mount, bool append )
{
	int err = PHYSFS_mount( path.c_str(), mount.c_str(), append ? 1 : 0 );

	if( err == 0 ) 
	{
		logError( "Could not mount '" + path + "'" );
		return false;
	}

	LogInfo( "Mounted '%s' in mount point '%s'",
		path.c_str(), mount.empty() ? "/" : mount.c_str() );

	if( fileWatcher )
		fileWatcher->addWatch( path );

	return true;
}

//-----------------------------------//

void FileSystem::mountDefaultLocations(const String& base)
{
	mount(".");

	if ( !mount(base) ) return;

	std::vector<String> dirs;
	FileEnumerateDirectories(base, dirs);

	for( size_t i = 0; i < dirs.size(); i++ )
	{
		const String& dir = dirs[i];
		mount(dir);
	}
}

//-----------------------------------//

void FileSystem::update( float )
{
	if( fileWatcher )
		fileWatcher->update();
}

//-----------------------------------/

void FileSystem::logError( const String& msg )
{
	LogError( "%s: %s", msg.c_str(), PHYSFS_getLastError() );
}

//-----------------------------------//

void FileSystem::log()
{
	PHYSFS_Version version;
	PHYSFS_getLinkedVersion(&version);

	LogInfo( "Initialized PhysFS version %d.%d.%d", 
		version.major, version.minor, version.patch );

	const PHYSFS_ArchiveInfo **i;

	std::stringstream ss;
	ss << "Supported archives: ";

	for (i = PHYSFS_supportedArchiveTypes(); *i != nullptr; i++)
		ss << "'" << (*i)->extension << "', ";

	LogInfo( "User write folder: %s", PHYSFS_getWriteDir() );
	
	String s = ss.str();
	s = s.substr( 0, s.find_last_of(',') );
	LogInfo( "%s", s.c_str() );
}

//-----------------------------------//

} // end namespace

#endif