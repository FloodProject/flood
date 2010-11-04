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

namespace vapor {

//-----------------------------------//

Lua_Loader::Lua_Loader()
{
	extensions.push_back( "lua" );
}

//-----------------------------------//

bool Lua_Loader::decode(const File& file, Resource* res)
{
	std::vector<byte> text = file.read();
	std::string str( text.begin(), text.end() );

	Script* script = static_cast<Script*>( res );
	script->setSource( str );

	return true;
}

//-----------------------------------//

} // end namespace

#endif