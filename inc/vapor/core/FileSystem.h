/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//#ifdef VAPOR_VFS_PHYSFS

#include "Subsystem.h"
#include "Core/File.h"
#include "Core/FileWatcher.h"

namespace vapor {

//-----------------------------------//

/**
 * Represents a virtual file system that will be used by all the other
 * engine classes for file I/O operations. When this is constructed,
 * it needs to be populated with 'mount points'. Each mount point can
 * be a different kind of device. For example, you can mount a CD-ROM
 * drive, or a filesystem folder hierarchy, or even a compressed ZIP.
 * Mount points can also overlap. In that case, the first mount found
 * with the requested file will be the one used. You can control the
 * order of search by prepending or appending the archive or directory
 * to the search path while mounting it in the virtual filesystem.
 */

class CORE_API FileSystem : public Subsystem
{
public:

	// Constructs a new instance of the virtual filesystem.
	FileSystem( const String& app, const char* argv0 = nullptr );
	virtual ~FileSystem();

	// Enumerate files in a virtual mount point.
	std::vector<String> enumerateFiles(const String& path);

	// Mount an archive into the virtual filesystem point.
	bool mount(const String& path, const String& mount = "",
		bool append = true);

	// Mounts all the default resource locations.
	void mountDefaultLocations(const String& baseFolder);

	// Gets all the mount points in the FileSystem.
	GETTER(MountPoints, const std::vector< String >&, mountPoints)

	// Gets a reference to the notification watcher.
	GETTER(FileWatcher, FileWatcher*, fileWatcher)

	// Updates the watcher with new notifications.
	virtual void update( float );

protected:

	// Sets up some sensible default mount points.
	void setDefaultConfig(const String& app);

	// Logs the version and supported archive types.
	void log();

	// Logs the FileSystem-related errors.
	void logError( const String& );

	// Watch notifications for file events.
	FileWatcher* fileWatcher;

	// Holds the mounted archives/directories in the filesystem.
	std::vector<String> mountPoints;
};

//-----------------------------------//

} // end namespace

//#endif