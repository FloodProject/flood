/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_SCRIPTING_LUA

#include "Resources/LuaLoader.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Lua_Loader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

Lua_Loader::Lua_Loader()
{
	extensions.push_back( "lua" );
}

//-----------------------------------//

bool Lua_Loader::decode(ResourceLoadOptions& options)
{
	String text;
	StreamReadString(options.stream, text);

	Script* script = static_cast<Script*>( options.resource );
	script->setSource( text );

	return true;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif