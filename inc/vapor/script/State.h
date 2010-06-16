/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//#ifdef VAPOR_SCRIPTING_LUA

struct lua_State;

#include "vapor/script/Script.h"

namespace vapor {

class Engine;

//-----------------------------------//

/**
 * Scripting context that can be used to execute scripts. 
 * Currently this represents a Lua interpreter state. 
 */

class VAPOR_API State : private boost::noncopyable
{
public:

	/// Constructs a new scripting VM.
	State();
	
	/// Destructs this scripting VM.
	~State();

	// Creates a new script.
	//ScriptPtr createScript();

	/// Executes a piece of source code.
	bool execute( const std::string& source );

	/// Executes a script resource.
	bool execute( const ScriptPtr& script );

	/// Bind the engine API to Lua.
	void bind( Engine* const engine );

	/// Registers a script into this state.
	void registerScript( const ScriptPtr& script );

	/// Handles the Lua error states.
	void handleError();

	/// Executes all the registered scripts.
	void update( float deltaTime );

	// Gets the last error.
	GETTER(LastError, const std::string&, lastError)

protected:

	/// Lua VM state.
	lua_State* luaState;

	/// Holds Lua scripts.
	std::vector< ScriptPtr > scripts;
	typedef std::vector< ScriptPtr >::iterator scriptsIterator;

	std::string lastError;
};

//-----------------------------------//

} // end namespace

//#endif