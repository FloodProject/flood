/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_SCRIPTING_LUA

#include "Script/ScriptManager.h"
#include "Resources/ResourceManager.h"

#include <lua.hpp>

extern "C" int luaopen_vapor(lua_State* L);

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

ScriptManager::ScriptManager()
	: state(nullptr)
{
	ResourceManager* rm = GetResourceManager();
	rm->onResourceReloaded.Connect( this, &ScriptManager::onReload );

	// Create a new Lua VM instance.
	state = Allocate(State, AllocatorGetHeap(), luaL_newstate());

	// Check for proper initialization of the Lua state.
	if( !state->getLuaState() )
	{
		LogError( "Error initializing %s", LUA_RELEASE );
		return;
	}

	// Initialize the standard libraries (we want to disallow I/O though).
	luaL_openlibs( state->getLuaState() );
	luaopen_vapor( state->getLuaState() );

	LogInfo( "Initialized %s", LUA_RELEASE );
}

//-----------------------------------//

ScriptManager::~ScriptManager()
{
	ResourceManager* rm = GetResourceManager();
	rm->onResourceReloaded.Disconnect( this, &ScriptManager::onReload );

	LogInfo("Cleaning up the Lua state");
	Deallocate(state);
}

//-----------------------------------//

State* ScriptManager::createScriptInstance(Script* script)
{
	if( !script ) return nullptr;

	// Create a new thread state sharing the main state.
	lua_State* thread = lua_newthread( state->getLuaState() );
	State* newState = Allocate(State, AllocatorGetHeap(), thread);

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

void ScriptManager::onReload( const ResourceEvent& event )
{
	const ScriptHandle& handle = HandleCast<Script>(event.handle);
	Script* script = handle.Resolve();

	if( script->getResourceGroup() != ResourceGroup::Scripts )
		return;
	
	LogDebug( "Reloading script '%s'", script->getPath().c_str() );

	ScriptsMap::const_iterator it;
	for( it = scripts.begin(); it != scripts.end(); it++ )
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

NAMESPACE_ENGINE_END

#endif