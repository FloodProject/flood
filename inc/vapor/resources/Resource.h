/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

class ResourceLoader;

//-----------------------------------//

/**
 * Resource groups identify the kind of data a resource holds.
 * The ResourceManager class uses this information to keep the
 * related resources together, and for managing the data.
 */

namespace ResourceGroup
{
	enum Enum
	{
		General,
		Images,
		Meshes,
		Fonts,
		Shaders,
		Audio
	};

	// Yeah, a bit ugly, but it works.
	const std::string getString(Enum e);
}

//-----------------------------------//

/**
 * Represents a generic resource that tipically is used to hold a piece
 * of data that is used by the various systems of the engine, for example
 * mesh, sounds, fonts, images, shaders, and other types of data.
 *
 * Each resource is identified by a URI (Uniform Resource Identifier),
 * that way we can add virtual resources (it could be used for various
 * things, like procedural content generation and to identify streaming
 * resource from a network connection).
 */

class Resource
{
public:

	virtual ~Resource();

	// Reloads this resource.
	// bool reload();

	// Watches this resource for changes and auto-reloads it.
	// void watch();

	// Releases the contents of this resource (to save memory).
	// void release();

	// Gets the path that identifies this resource.
	virtual const std::string& getPath();

	virtual void setPath( const std::string& uri );
	
	// Gets the resource group associated with this resource.
	virtual ResourceGroup::Enum getResourceGroup() const = 0;

	// Gets the resource loader associated with this resource.
	//virtual ResourceLoader* getResourceLoader() = 0;

protected:

	Resource( const std::string& uri = "" );

	std::string uri;
};

//-----------------------------------//

typedef tr1::shared_ptr< Resource > ResourcePtr;

//-----------------------------------//

} } // end namespaces


