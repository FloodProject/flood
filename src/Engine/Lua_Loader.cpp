/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/resources/Lua_Loader.h"

using vapor::vfs::File;

namespace vapor { namespace resources {

//-----------------------------------//

Lua_Loader::Lua_Loader()
{
	extensions.push_back( "lua" );
}

//-----------------------------------//

script::Script* Lua_Loader::decode(const File& file)
{
	std::vector<byte> text = file.read();

	std::string str( text.begin(), text.end() );

	script::Script* script = new script::Script();
	script->setSource( str );
	script->setURI( file.getFullPath() );

	return script;
}

//-----------------------------------//

} } // end namespaces

#endif