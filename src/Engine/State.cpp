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

#include <lua.hpp>
#include <luabind/luabind.hpp>

namespace vapor {
	namespace script {

//-----------------------------------//

// Forward declaration. This function is on another file (Bindings.cpp).
void bindEngine( lua_State* luaState, Engine* engine );

//-----------------------------------//

int handle_error( lua_State* L )
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

State::State()
{
	// Create a new Lua VM instance (called a state in Lua terminology).
	// It calls lua_newstate with an allocator based on the standard C 
	// realloc function and then sets a panic function (see lua_atpanic)
	// that prints an error message to the standard error output in case
	// of fatal errors. 
	luaState = luaL_newstate();

	// Check for proper initialization of the Lua state.
	if( luaState != nullptr )
		info( "script::lua", "Initialized %s", LUA_RELEASE );
	else
		error( "script::lua", "Error initializing %s", LUA_RELEASE );

	// Initialize the standard libraries (we want to disallow I/O though).
	luaL_openlibs( luaState );

	// Pass the newly created Lua state to Luabind.
	luabind::open( luaState );

	// Set our own error handling function for Luabind.
	luabind::set_pcall_callback( &handle_error );
}

//-----------------------------------//

State::~State()
{
	info("script::lua", "Cleaning up the Lua state");

	scripts.clear();

	// Clean up the Lua state.
	//lua_close( luaState );
}

//-----------------------------------//

bool State::execute( Script* script )
{
	// From the Lua C API docs:
	//	"It returns 0 if there are no errors or 1 in case of errors."
	int status = luaL_dofile( luaState, script->getURI().c_str() );

	if( status == 1 )
	{
		// An error has occured so we need to handle it.
		handleError();
		return false;
	}

	return true;
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

void State::bind( Engine* engine )
{
	// Binds all the engine API to Lua.
	bindEngine( luaState, engine );
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

void State::registerScript( ScriptPtr script )
{
	// Check if the script is already in the database.
	scriptsIterator it = std::find( scripts.begin(), scripts.end(), script );

	// If it was not found...
	if( it == scripts.end() )
	{
		// Add the new script to this state's database.
		scripts.push_back( script );

		// Additionaly set the script's state.
		script->setState( this );
	}
}

//-----------------------------------//

void State::update( float deltaTime )
{
	// Execute all scripts.
	foreach( ScriptPtr script, scripts )
	{
		script->execute();
	}
}

const std::string& State::getLastError() const
{
	return lastError;
}

//-----------------------------------//

} } // end namespaces

#endif