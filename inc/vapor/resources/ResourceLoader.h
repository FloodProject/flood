/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/vfs/File.h"
#include "vapor/resources/Resource.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Resource loading interface for classes that want to provide resource
 * encoding and decoding services for a given resource format type.
 * Each resource format will have a corresponding resource loader that
 * decodes the buffer with the content and returns a decoded resource.
 */

class VAPOR_API ResourceLoader : private boost::noncopyable
{
public:

	ResourceLoader() {}
	virtual ~ResourceLoader() {}

	// Decodes a given file into a resource.
	virtual Resource* decode(const vfs::File& file) = 0;

	// Gets a list of recognized extensions by this loader.
	// TODO: make this const
	virtual std::list< std::string >& getExtensions() = 0;

	// Gets the name of this resource loader.
	virtual const std::string getName() = 0;

	// Gets the resource group of this loader.
	virtual ResourceGroup::Enum getResourceGroup() = 0;
};

//-----------------------------------//

} } // end namespaces