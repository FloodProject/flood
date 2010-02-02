/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/vfs/File.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Specialization of a resource that is backed by a file in the virtual 
 * file system layer of the engine. This file can be told to watch for 
 * changes, and it will auto-reload the resource in case that event 
 * happens. This will allow things like automatic textures/shaders 
 * updating in-game due to file changes. :)
 */

class VAPOR_API ResourceFile : private boost::noncopyable
{
public:

	virtual ~ResourceFile();

	// Reloads this resource.
	// bool reload();

	// Watches this resource for changes and auto-reloads it.
	// void watch();

	// Releases the contents of this resource (to save memory).
	// void release();

	// Gets the path that identifies this resource.
	virtual const std::string& getURI() const;

	// Gets the path that identifies this resource.
	//virtual const vfs::FilePtr& getFile() const;


protected:

	ResourceFile( vfs::FilePtr file );

	vfs::FilePtr file;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( ResourceFile );

//-----------------------------------//

} } // end namespaces


