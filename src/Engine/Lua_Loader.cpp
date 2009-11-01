/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/resources/Lua_Loader.h"

using vapor::vfs::File;

namespace vapor {
	namespace resources {

//-----------------------------------//

Lua_Loader::Lua_Loader()
{
	extensions.push_back( "lua" );
}

//-----------------------------------//

script::Script* Lua_Loader::decode(const File& file)
{
	std::vector<byte> text = file.read();

	const std::string& path = file.getPath();

	std::string str( text.begin(), text.end() );

	script::Script* script;
	script = new script::Script( str );
	script->setURI( path );

	return script;
}

//-----------------------------------//

} } // end namespaces

#endif