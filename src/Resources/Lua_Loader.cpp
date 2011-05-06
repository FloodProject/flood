/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_SCRIPTING_LUA

#include "Resources/Lua_Loader.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Lua_Loader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

Lua_Loader::Lua_Loader()
{
	extensions.push_back( "lua" );
}

//-----------------------------------//

bool Lua_Loader::decode(const Stream& stream, Resource* res)
{
	String text;
	StreamReadString((Stream*) &stream, text);

	Script* script = static_cast<Script*>( res );
	script->setSource( text );

	return true;
}

//-----------------------------------//

} // end namespace

#endif