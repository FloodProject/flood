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

#ifdef SCENE_LOADER

NAMESPACE_BEGIN

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

	// Parses a Lua text script to a buffer.
	virtual bool decode(const Stream& file, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const String, "Scene")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Scripts)
};

//-----------------------------------//

NAMESPACE_END

#endif