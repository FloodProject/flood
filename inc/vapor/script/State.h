/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_SCRIPTING_LUA

struct lua_State;

#include "script/Script.h"

namespace vapor {

//-----------------------------------//

class Engine;

/**
 * Scripting context that can be used to execute scripts. 
 * Currently this represents a Lua interpreter state. 
 */

class VAPOR_API State
{
	DECLARE_UNCOPYABLE(State)

public:

	// Constructs a new scripting VM.
	State(lua_State* state);
	
	// Destructs this scripting VM.
	~State();

	// Gets the Lua state.
	GETTER(LuaState, lua_State*, luaState)

	// Loads a script resource.
	bool load( Script* script );

	// Executes a piece of source code.
	bool execute( const String& source );

	// Executes a script resource.
	bool execute( Script* script );

	// Invokes a function in the state.
	bool invoke( const String& name, uint8 numArguments );

	// Handles the Lua error states.
	void handleError();

	// Gets the last error.
	GETTER(LastError, const String&, lastError)

protected:

	// Lua VM state.
	lua_State* luaState;

	// Last error.
	String lastError;
};

//-----------------------------------//

} // end namespace

#endif