/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"

#ifdef VAPOR_VFS_PHYSFS

#include "Utilities.h"
#include "vfs/FileSystem.h"
#include "FileWatcherWin32.h"
#include <physfs.h>

namespace vapor {

//-----------------------------------//

FileSystem::FileSystem(const std::string& app, const char* argv0 )
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
	foreach( const std::string& point, mountPoints )
		PHYSFS_removeFromSearchPath( point.c_str() );
	
	if( !PHYSFS_deinit() )
		logError( "Could not clean up PhysFS" );

	delete fileWatcher;
}

//-----------------------------------//

void FileSystem::setDefaultConfig(const std::string& app)
{
	int err = PHYSFS_setSaneConfig("vapor", app.c_str(), nullptr, 0, 0);

	if( err == 0 )
	{
		logError( "Could not set a sane config" );
		return;
	}

	Log::info( "Mounted '%s' in mount point '/'", PHYSFS_getBaseDir() );
}

//-----------------------------------//

std::vector<std::string> FileSystem::enumerateFiles(const std::string& path)
{
	char **rc = PHYSFS_enumerateFiles( path.c_str() );
	
	std::vector<std::string> files;
	
	for (char** i = rc; *i != nullptr; i++)
		files.push_back(*i);	
		
	PHYSFS_freeList(rc);
	
	return files;
}

//-----------------------------------//

bool FileSystem::mount(const std::string& path, const std::string& mount, bool append )
{
	int err = PHYSFS_mount( path.c_str(), mount.c_str(), append ? 1 : 0 );

	if( err == 0 ) 
	{
		logError( "Could not mount '" + path + "'" );
		return false;
	}

	Log::info( "Mounted '%s' in mount point '%s'",
		path.c_str(), mount.empty() ? "/" : mount.c_str() );

	if( fileWatcher )
		fileWatcher->addWatch( path );

	return true;
}

//-----------------------------------//

void FileSystem::mountDefaultLocations()
{
	// Default filesystem mount points.
	const std::string media( "media/" );

	//std::string dirs[] = 
	//{
	//	"fonts",
	//	"images",
	//	"materials",
	//	"meshes",
	//	"scripts",
	//	"sounds"
	//};

	mount(".");

	if ( !mount(media) )
		return;

	std::vector<std::string> dirs = System::enumerateDirs(media);

	foreach( const std::string& dir, dirs )
		mount(dir);

	//foreach( const std::string& dir, dirs )
	//	mount(media+dir);
}

//-----------------------------------//

void FileSystem::update( double )
{
	if( fileWatcher )
		fileWatcher->update();
}

//-----------------------------------/

void FileSystem::logError( const std::string& msg )
{
	Log::error( "%s: %s", msg.c_str(), PHYSFS_getLastError() );
}

//-----------------------------------//

void FileSystem::log()
{
	PHYSFS_Version version;
	PHYSFS_getLinkedVersion(&version);

	Log::info( "Initialized PhysFS version %d.%d.%d", 
		version.major, version.minor, version.patch );

	const PHYSFS_ArchiveInfo **i;

	std::stringstream ss;
	ss << "Supported archives: ";

	for (i = PHYSFS_supportedArchiveTypes(); *i != nullptr; i++)
		ss << "'" << (*i)->extension << "', ";

	Log::info( "User write folder: %s", PHYSFS_getWriteDir() );
	
	std::string s = ss.str();
	s = s.substr( 0, s.find_last_of(',') );
	Log::info( "%s", s.c_str() );
}

//-----------------------------------//

} // end namespace

#endif