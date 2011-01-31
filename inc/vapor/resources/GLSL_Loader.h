/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "CompileOptions.h"

#ifdef VAPOR_SHADER_GLSL

#include "Resources/ResourceLoader.h"

namespace vapor {

//-----------------------------------//

/**
 * Loads GLSL shaders as text files.
 */

class GLSL_Loader : public ResourceLoader
{
	DECLARE_CLASS_()

public:

	GLSL_Loader();

	// Creates the resource with no data.
	virtual Resource* prepare(const Stream& stream);

	// Parses a GLSL text shader to a buffer.
	virtual bool decode(const Stream& file, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "GLSL")

	// Gets the list of extensions this codec can handle.
	GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)

protected:

	// Holds all file extensions recognized by this codec.
	mutable ExtensionList extensions;
};

//-----------------------------------//

} // end namespace

#endif