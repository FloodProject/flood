/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/resources/ResourceLoader.h"
#include "vapor/script/Script.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Loads Lua script files.
 */

class Lua_Loader : public ResourceLoader
{
public:

	Lua_Loader();

	// Parses a Lua text script to a buffer.
	virtual script::Script* decode(const vfs::File& file);

	// Gets the name of this codec.
	virtual const std::string getName() { return "LUA"; }

	// Gets the list of extensions this codec can handle.
	virtual std::list< std::string >& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Scripts; }

protected:

	// holds all file extensions recognized by this codec
	std::list< std::string > extensions;
};

//-----------------------------------//

} } // end namespaces

#endif