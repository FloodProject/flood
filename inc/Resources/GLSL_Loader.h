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
#include "Resources/Shader.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Loads GLSL shaders as text files.
 */

REFLECT_DECLARE_CLASS(GLSL_Loader)

class GLSL_Loader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(GLSL_Loader)

public:

	GLSL_Loader();

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Shader)

	// Creates the resource with no data.
	virtual Resource* prepare(const Stream& stream) OVERRIDE;

	// Parses a GLSL text shader to a buffer.
	virtual bool decode(const Stream& file, Resource* res) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "GLSL")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif