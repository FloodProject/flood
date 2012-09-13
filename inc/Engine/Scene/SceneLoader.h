/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/ResourceLoader.h"
#include "Engine/Scene/Scene.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Loads scene files.
 */

API_SCENE REFLECT_DECLARE_CLASS(SceneLoader)

class API_SCENE SceneLoader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(SceneLoader)

public:

	SceneLoader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Scene)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Scene)

	// Parses a Lua text script to a buffer.
	bool decode(ResourceLoadOptions&) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "Scene")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Scenes)
};

//-----------------------------------//

NAMESPACE_ENGINE_END