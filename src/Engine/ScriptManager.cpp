/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "script/ScriptManager.h"
#include "Engine.h"

#include <lua.hpp>

extern "C" int luaopen_vapor(lua_State* L);

namespace vapor {

//-----------------------------------//

ScriptManager::ScriptManager()
	: state(nullptr)
{
	ResourceManager* rm = GetEngine()->getResourceManager();
	rm->onResourceReloaded.Connect( this, &ScriptManager::onReload );

	// Create a new Lua VM instance.
	state = new State( luaL_newstate() );

	// Check for proper initialization of the Lua state.
	if( !state->getLuaState() )
	{
		Log::error( "Error initializing %s", LUA_RELEASE );
		return;
	}

	// Initialize the standard libraries (we want to disallow I/O though).
	luaL_openlibs( state->getLuaState() );
	luaopen_vapor( state->getLuaState() );

	Log::info( "Initialized %s", LUA_RELEASE );
}

//-----------------------------------//

ScriptManager::~ScriptManager()
{
	ResourceManager* rm = GetEngine()->getResourceManager();
	rm->onResourceReloaded.Disconnect( this, &ScriptManager::onReload );

	Log::info("Cleaning up the Lua state");
	delete state;
}

//-----------------------------------//

State* ScriptManager::createScriptInstance(const ScriptPtr& script)
{
	if( !script )
		return nullptr;

	// Create a new thread state sharing the main state.
	lua_State* thread = lua_newthread( state->getLuaState() );
	State* newState = new State(thread);

	lua_State* L = thread;

	// Replaces the environment table of the thread.
	lua_pushthread(L);                  // thread
	lua_newtable(L);                    // thread, t
	lua_pushvalue(L, LUA_GLOBALSINDEX); // thread, t, _G
	lua_setfield(L, -2, "__index");     // thread, t
	lua_pushvalue(L, -1);               // thread, t, t
	lua_setmetatable(L, -2);            // thread, t
	lua_setfenv(L, -2);                 // thread 

	// Keeps a reference to the thread in the registry.
    //int top = lua_gettop(L);
    //lua_getfield(L, LUA_REGISTRYINDEX, "Scripts"); // top + 1
    //lua_pushlightuserdata(L, newState); // key, the pointer to my own Script class
    //lua_pushvalue(L, top+2);          // value, the new lua thread.
    //lua_rawset(L, top+1); // Scripts table
    //lua_settop(L, top); // Cleans the stack.

	// Register the new state.
	scripts[newState] = script;

	// Register the source in the state.
	newState->load( script );
	
	return newState;
}

//-----------------------------------//

void ScriptManager::onReload( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Scripts )
		return;
	
	Log::debug( "Reloading script '%s'", evt.resource->getPath().c_str() );
	
	const ScriptPtr& script = RefCast<Script>( evt.resource );

	ScriptsMap::const_iterator it;
	for( it = scripts.cbegin(); it != scripts.cend(); it++ )
	{
		if( script != it->second )
			continue;

		State* state = it->first;
		assert( state != nullptr );
		
		state->load( script );
	}
}

//-----------------------------------//

void ScriptManager::update(float delta)
{

}

//-----------------------------------//

} // end namespace

#endif