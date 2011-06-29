/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_SCRIPTING_LUA

#include "Resources/ResourceLoader.h"
#include "Script/Script.h"

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Loads Lua script files.
 */

REFLECT_DECLARE_CLASS(Lua_Loader)

class Lua_Loader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(Lua_Loader)

public:

	Lua_Loader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Script)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Script)

	// Parses a Lua text script to a buffer.
	virtual bool decode(const Stream& file, Resource* res) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "LUA")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Scripts)
};

//-----------------------------------//

NAMESPACE_END

#endif