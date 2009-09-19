/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/vfs/File.h"
#include "vapor/resources/Resource.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

/**
 * Resource Loader interface for classes that want to provide resource
 * encoding and decoding services for a given file format type.
 */

class VAPOR_API ResourceLoader : private boost::noncopyable
{
public:

	ResourceLoader() {}
	virtual ~ResourceLoader() {}

	// Decodes a given file into a resource.
	virtual Resource* decode(vfs::File& file) = 0;
	
	// Gets a list of recognized extensions by this loader.
	virtual std::list< std::string >& getExtensions() = 0;

	// Gets the name of this resource loader.
	virtual const std::string getName() = 0;

	// Gets the resource group of this loader.
	virtual ResourceGroup::Enum getResourceGroup() = 0;
};

//-----------------------------------//

} } // end namespaces