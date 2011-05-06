/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Subsystem.h"
#include "script/State.h"
#include "Resources/ResourceManager.h"

#ifdef ENABLE_SCRIPTING_LUA

namespace vapor {

//-----------------------------------//

typedef std::map<State*, Script*> ScriptsMap;
typedef std::pair<const State*, Script> ScriptsMapPair;

//-----------------------------------//

/**
 * Manages all the script instances in the engine.
 */

class VAPOR_API ScriptManager : public Subsystem
{
public:

	ScriptManager();
	~ScriptManager();

	// Creates a new script instance.
	State* createScriptInstance(Script* script);

	// Gets the Lua state.
	GETTER(State, State*, state)

	// Updates all the registered scripts.
	void update( float deltaTime );

protected:

	// Gets called when a resource is reloaded.
	void onReload( const ResourceEvent& event );

	// Lua VM state.
	State* state;

	// Holds Lua script instances.
	ScriptsMap scripts;
};

//-----------------------------------//

} // end namespace

#endif