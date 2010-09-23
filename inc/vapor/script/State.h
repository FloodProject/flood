/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_SCRIPTING_LUA

struct lua_State;

#include "vapor/script/Script.h"

namespace vapor {

//-----------------------------------//

class Engine;

//-----------------------------------//

/**
 * Scripting context that can be used to execute scripts. 
 * Currently this represents a Lua interpreter state. 
 */

class VAPOR_API State : private boost::noncopyable
{
public:

	// Constructs a new scripting VM.
	State(lua_State* state);
	
	// Destructs this scripting VM.
	~State();

	// Gets the Lua state.
	GETTER(LuaState, lua_State*, luaState)

	// Loads a script resource.
	bool load( const ScriptPtr& script );

	// Executes a piece of source code.
	bool execute( const std::string& source );

	// Executes a script resource.
	bool execute( const ScriptPtr& script );

	// Invokes a function in the state.
	bool invoke( const std::string& name, int numArguments = 0 );

	// Handles the Lua error states.
	void handleError();

	// Gets the last error.
	GETTER(LastError, const std::string&, lastError)

protected:

	// Lua VM state.
	lua_State* luaState;

	// Last error.
	std::string lastError;
};

//-----------------------------------//

} // end namespace

#endif