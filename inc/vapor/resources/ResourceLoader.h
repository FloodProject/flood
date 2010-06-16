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

namespace vapor {

#define PREPARE(T) \
	virtual Resource* prepare(const File&) { return new T(); }

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

	// Creates the resource with no data.
	// Note: File I/O should be done in 'decode'.
	virtual Resource* prepare(const File&) = 0;

	// Decodes a given file into a resource.
	virtual bool decode(const File&, Resource*) = 0;

	// Gets a list of recognized extensions by this loader.
	typedef std::list< std::string > ExtensionList;
	virtual ExtensionList& getExtensions() const = 0;

	// Gets the name of this resource loader.
	virtual const std::string getName() const = 0;

	// Gets the resource group of this loader.
	virtual ResourceGroup::Enum getResourceGroup() const = 0;
};

TYPEDEF_PTR(ResourceLoader)

//-----------------------------------//

} // end namespace