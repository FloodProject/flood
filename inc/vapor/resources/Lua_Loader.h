/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/resources/ResourceLoader.h"
#include "vapor/script/Script.h"

namespace vapor {

//-----------------------------------//

/**
 * Loads Lua script files.
 */

class Lua_Loader : public ResourceLoader
{
public:

	Lua_Loader();

	// Creates the resource with no data.
	PREPARE(Script)

	// Parses a Lua text script to a buffer.
	virtual bool decode(const File&, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "LUA")

	// Gets the list of extensions this codec can handle.
	GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Scripts)

protected:

	// Holds all file extensions recognized by this codec.
	mutable ExtensionList extensions;
};

//-----------------------------------//

} // end namespace

#endif