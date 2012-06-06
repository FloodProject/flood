/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_RENDERER_OPENGL_GLSL

#include "Resources/ResourceLoader.h"
#include "Resources/ShaderMaterial.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Loads shaders from text files.
 */

REFLECT_DECLARE_CLASS(ShaderLoader)

class ShaderLoader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(ShaderLoader)

public:

	ShaderLoader();

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(ShaderMaterial)

	// Creates the resource with no data.
	virtual Resource* prepare(ResourceLoadOptions&) OVERRIDE;

	// Parses a GLSL text shader to a buffer.
	virtual bool decode(ResourceLoadOptions&) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "GLSL")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif