/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/Engine.h"

#include <luabind/luabind.hpp>

using namespace luabind;

using namespace vapor::scene;
using namespace vapor::resources;

namespace vapor {
	namespace script {

//-----------------------------------//

void bindNode( lua_State* luaState )
{
	module( luaState )
	[
	class_< Node >( "Node" )
		// Constructors
		.def( constructor<>() )
		.def( constructor< const std::string& >() )
		// Methods
		.def( "addComponent", &Node::addComponent )
		.def( "update", &Node::update )
		// Properties
		.property( "name", &Node::getName )
		.property( "parent", &Node::getParent, &Node::setParent )
		// Fields
		//.def_readwrite("a", &A::a)
		//.def_readonly("a", &A::a)
	];
}

//-----------------------------------//

void debug( const std::string& msg )
{
	log::debug( msg );
}

void bindLogger( lua_State* luaState )
{
	module( luaState, "log" )
	[
		def( "debug", &debug )
		//def( "info", &log::info ),
		//def( "warn", &log::warn ),
		//def( "error", &log::error )
	];
}

//-----------------------------------//

void bindResourceManager( lua_State* luaState )
{
	module( luaState )
	[
	class_< ResourceManager >( "ResourceManager" )
		// Constructors
		.def( constructor<>() )
		// Methods
		.def( "load", &ResourceManager::loadResource )
		//.def( "update", &Node::update )
		// Properties
		//.property( "name", &Node::getName )
		//.property( "parent", &Node::getParent, &Node::setParent )
		// Fields
		//.def_readwrite("a", &A::a)
		//.def_readonly("a", &A::a)
	];
}

//-----------------------------------//

void bindEngine( lua_State* luaState, Engine* engine )
{
	bindNode( luaState );
	bindLogger( luaState );
	bindResourceManager( luaState );

	globals(luaState)["ResourceManager"] = engine->getResourceManager();
}

//-----------------------------------//

} } // end namespaces

#endif