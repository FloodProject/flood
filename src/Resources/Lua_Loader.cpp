/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "Resources/Lua_Loader.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Lua_Loader, ResourceLoader)
END_CLASS()

//-----------------------------------//

Lua_Loader::Lua_Loader()
{
	extensions.push_back( "lua" );
}

//-----------------------------------//

bool Lua_Loader::decode(const Stream& file, Resource* res)
{
	std::string text;
	file.read(text);

	Script* script = static_cast<Script*>( res );
	script->setSource( text );

	return true;
}

//-----------------------------------//

} // end namespace

#endif