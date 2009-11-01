/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/script/State.h"

#include <luabind/luabind.hpp>

namespace vapor {
	namespace script {

//-----------------------------------//

void bindEngine( lua_State* luaState, Engine* engine );

//-----------------------------------//

State::State()
{
	// Create a new lua state
	luaState = lua_open();

	// Check for proper initialization
	if( luaState != nullptr )
		info( "script::lua", "Initialized %s", LUA_RELEASE );
	else
		error( "script::lua", "Error initializing %s", LUA_RELEASE );

	// Initialize the standard libraries
	luaL_openlibs( luaState );

	// Pass the state to luabind
	luabind::open( luaState );

	execute( "log.debug( \"hello from lua 1\" )" );
}

//-----------------------------------//

State::~State()
{
	info("script::lua", "Cleaning up the Lua state");

	// Clean up the Lua state
	lua_close( luaState );
}

//-----------------------------------//

void State::execute( Script* script )
{
	execute( script->getSource() );
}

//-----------------------------------//

void State::execute( const std::string& source )
{
	luaL_dostring( luaState, source.c_str() );
}

//-----------------------------------//

void State::bind( Engine* engine )
{
	bindEngine( luaState, engine );
}

//-----------------------------------//

} } // end namespaces

#endif