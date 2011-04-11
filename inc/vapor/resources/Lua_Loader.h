/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_SCRIPTING_LUA

#include "Resources/ResourceLoader.h"
#include "Script/Script.h"

namespace vapor {

//-----------------------------------//

/**
 * Loads Lua script files.
 */

class Lua_Loader : public ResourceLoader
{
	DECLARE_CLASS_()

public:

	Lua_Loader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Script)

	// Parses a Lua text script to a buffer.
	virtual bool decode(const Stream& file, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "LUA")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Scripts)
};

//-----------------------------------//

} // end namespace

#endif