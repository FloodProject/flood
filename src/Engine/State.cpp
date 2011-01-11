/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/script/State.h"
#include <lua.hpp>
#include <sstream>

namespace vapor {

//-----------------------------------//

static int handleLuaError( lua_State* L )
{
	// We will ask Lua some more information about the error.
	// This is the structure where Lua will return it to us.
	lua_Debug d;
	
	// Get the last value from the stack?
	lua_getstack(L, 1, &d);

	// Puts more information into the debug structure.
	lua_getinfo(L, "Sln", &d);

	// Lua will automatically push a default error message
	// into the stack, so we will get it and clean the stack.
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);

	// Format the error message.
	std::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if (d.name != 0)
	{
		msg << "(" << d.namewhat << " " << d.name << ")";
	}

	msg << " " << err;
	lua_pushstring(L, msg.str().c_str());

	return 1;
}

//-----------------------------------//

State::State(lua_State* state)
	: luaState(nullptr)
{
	assert( state != nullptr );

	luaState = state;

	lua_atpanic( luaState, &handleLuaError );
}

//-----------------------------------//

State::~State()
{
	// Clean up the Lua state.
	lua_close( luaState );
}

//-----------------------------------//

bool State::load( const ScriptPtr& script )
{
	if( !script )
		return false;

	int status = luaL_dostring( luaState,
		script->getSource().c_str() );

	if( status == 0 )
		return true;

	handleError();
	return false;
}

//-----------------------------------//

bool State::execute( const ScriptPtr& script )
{
	return execute( script->getSource() );
}

//-----------------------------------//

bool State::execute( const std::string& source )
{
	// Send the source code to Lua.
	int status = luaL_dostring( luaState, source.c_str() );

	if( status == 1 )
	{
		// An error has occured so we need to handle it.
		handleError();
		return false;
	}

	return true;
}

//-----------------------------------//

bool State::invoke( const std::string& name, int numArguments )
{
	// Get the function from the global table.
	lua_getglobal(luaState, name.c_str());

	if( !lua_isfunction(luaState, -1) )
	{
		lua_pop(luaState, 1);
		return false;
	}

	if( numArguments > 0 )
		lua_insert(luaState, -numArguments-1);
	
	// Resume the coroutine in the function.
	int res = lua_resume(luaState, numArguments);

	if( res != 0 && res != LUA_YIELD )
	{
		std::string msg = lua_tostring(luaState, -1);
		lua_pop(luaState, -1);

		if( lastError != msg )
		{
			Log::debug(msg);
			lastError = msg;
		}

		return false;
	}

	return true;
}

//-----------------------------------//

void State::handleError()
{
	// This will be called when you call a Lua function and it errors.
	// In that case, Lua will push an error message to the stack and we 
	// must retrieve it and correct the stack.

	lastError = lua_tostring( luaState, -1 );
	lua_pop( luaState, 1 );
}

//-----------------------------------//

} // end namespace

#endif