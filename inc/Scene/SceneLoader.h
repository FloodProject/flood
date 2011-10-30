/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/ResourceLoader.h"
#include "Scene/Scene.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Loads scene files.
 */

REFLECT_DECLARE_CLASS(SceneLoader)

class SceneLoader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(SceneLoader)

public:

	SceneLoader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Scene)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Scene)

	// Parses a Lua text script to a buffer.
	bool decode(const Stream& file, Resource* res) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "Scene")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Scenes)
};

//-----------------------------------//

NAMESPACE_ENGINE_END