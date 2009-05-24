/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace resources {

// forward decl.
class ResourceLoader;

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
		Shaders
	};

	// Yeah, a bit ugly, but it works.
	const char* getString(Enum e);
}

/**
 * Represents a generic resource that tipically is used to hold a piece
 * of data that is used by the various systems of the engine, for example
 * mesh data, sounds, fonts, images, shaders, and other types of stuff.
 *
 * Each resource is identified by a URI (Uniform Resource Identifier),
 * that way we can add virtual resources (it could be used for various
 * things, like procedural content generation and to identify streaming
 * resource from a network connection).
 */

class Resource
{
protected:

	Resource () { }
	virtual ~Resource () { }

public:

	// Gets the URI that identifies this resource.
	//virtual string getURI() = 0;
	
	// Gets the resource group associated with this resource.
	virtual ResourceGroup::Enum getResourceGroup() = 0;

	// Gets the resource loader associated with this resource.
	//virtual ResourceLoader*	getResourceLoader() = 0;
};

} } // end namespaces


