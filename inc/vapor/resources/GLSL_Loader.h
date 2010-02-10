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
#include "vapor/resources/Shader.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Loads GLSL shader files.
 */

class GLSL_Loader : public ResourceLoader
{
public:

	GLSL_Loader();

	// Parses a GLSL text shader to a buffer.
	virtual Shader* decode(const vfs::File& file);

	// Gets the name of this codec.
	virtual const std::string getName() { return "GLSL"; }

	// Gets the list of extensions this codec can handle.
	virtual std::list< std::string >& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Shaders; }

protected:

	// holds all file extensions recognized by this codec
	std::list< std::string > extensions;
};

//-----------------------------------//

} } // end namespaces

#endif