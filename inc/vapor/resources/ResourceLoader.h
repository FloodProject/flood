/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/vfs/File.h"
#include "vapor/resources/Resource.h"

using namespace vapor::vfs;

namespace vapor {
	namespace resources {

/**
 * Resource Loader interface for classes that want to provide image
 * coding and decoding services for a given image file format.
 */

class ResourceLoader
{
protected:

	ResourceLoader() {}
	virtual ~ResourceLoader() {}

public:

	// Decodes a given file into a resource.
	virtual Resource* decode(File& file) = 0;
	
	// Gets a list of recognized extensions by this loader.
	virtual list<string>& getExtensions() = 0;

	// Gets the name of this resource loader.
	virtual string getName() = 0;

	// Gets the resource group of this loader.
	virtual ResourceGroup::Enum getResourceGroup() = 0;
};

} } // end namespaces