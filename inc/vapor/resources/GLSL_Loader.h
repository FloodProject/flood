/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_SHADER_GLSL

#include "vapor/resources/ResourceLoader.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Loads GLSL shaders as text files.
 */

class GLSL_Loader : public ResourceLoader
{
public:

	GLSL_Loader();

	// Creates the resource with no data.
	virtual Resource* prepare(const vfs::File& file);

	// Parses a GLSL text shader to a buffer.
	virtual bool decode(const vfs::File&, Resource* res);

	// Gets the name of this codec.
	IMPLEMENT_GETTER(Name, const std::string, "GLSL")

	// Gets the list of extensions this codec can handle.
	IMPLEMENT_GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)

protected:

	// holds all file extensions recognized by this codec
	mutable ExtensionList extensions;
};

//-----------------------------------//

} } // end namespaces

#endif