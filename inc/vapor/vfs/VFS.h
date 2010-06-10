/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_VFS_PHYSFS

#include "vapor/Subsystem.h"
#include "vapor/vfs/File.h"
#include "vapor/vfs/Watcher.h"

/** \addtogroup virtual Virtual Filesystem 
 * @{ */

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

class VAPOR_API VFS : public Subsystem
{
public:

	/// Constructs a new instance of the virtual filesystem..
	VFS( const std::string& app, const char* argv0 = nullptr );
	virtual ~VFS();

	/// Mount an archive into the virtual filesystem point.
	bool mount(const std::string& path, const std::string& mount = "",
		bool append = true);

	/// Gets all the mount points in the VFS.
	IMPLEMENT_GETTER(MountPoints, const std::vector< std::string >&, mountPoints)

	/// Mounts all the default resource locations.
	void mountDefaultLocations();

	/// Gets a reference to the notification watcher.
	IMPLEMENT_GETTER(Watcher, Watcher*, watcher)

	/// Updates the watcher with new notifications.
	virtual void update( double );

private:

	/// Watch notifications for file events.
	Watcher* watcher;

	/// Logs the version and supported archive types.
	void log();

	/// Logs the VFS-related errors.
	void logError( const std::string& );

	/// Sets up some sensible default mount points.
	void setDefaultConfig(const std::string& app);

	/// Holds the mounted archives/directories in the filesystem.
	std::vector<std::string> mountPoints;
};

//-----------------------------------//

} // end namespace

/** @} */

#endif