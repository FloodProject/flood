/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#define VAPOR_USE_NAMESPACES
#include "vapor/Headers.h"

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning( push )
	#pragma warning( disable : 4127 )
	#pragma warning( disable : 4100 )
	#pragma warning( disable : 4512 )
	#include <luabind/luabind.hpp>
	#pragma warning( pop )
#else
	#include <luabind/luabind.hpp>
#endif

using namespace luabind;

namespace vapor { namespace script {

//-----------------------------------//

void bindEntity( lua_State* luaState )
{
	module( luaState )
	[
	class_< Node >( "Entity" )
		// Constructors
		.def( constructor<>() )
		.def( constructor< const std::string& >() )
		// Methods
		.def( "AddComponent", &Node::addComponent )
		.def( "Update", &Node::update )
		.def( "GetName", &Node::getName )
		.def( "GetTransform", &Node::getTransformPtr )
		// Properties
		//.property( "Components", &Node::getComponents )
		.property( "Name", &Node::getName )
		.property( "Parent", &Node::getParent, &Node::setParent ),
		// Fields
		//.def_readwrite("a", &A::a)
		//.def_readonly("a", &A::a)
	
	class_< Group, Node >( "Group" )
		// Constructors
		.def( constructor< const std::string& >() )
		// Methods
		.def( "Add", &Group::add )
		// Properties
		.property( "Count", &Group::count )
		.property( "Children", &Group::getChildren ),
		// Fields
		//.def_readwrite("a", &A::a)
		//.def_readonly("a", &A::a)
	
	class_< Scene, Group >( "Scene" )
		// Constructors
		.def( constructor<>() )
		// Methods
		//.def( "AddComponent", &Node::addComponent )
		.def( "Update", &Scene::update )
		.def( "GetEntity", &Scene::getEntityPtr )
		// Properties
		.def( "GetName", &Scene::getName )
		//.property( "parent", &Node::getParent, &Node::setParent )
		// Fields
		//.def_readwrite("a", &A::a)
		//.def_readonly("a", &A::a)
	];
}

//-----------------------------------//

void bindComponents( lua_State* luaState )
{
	module( luaState )
	[
	class_< Component >( "Component" )
		// Constructors
		//.def( constructor<>() )
		// Methods
		.def( "Update", &Component::update )
		// Properties
		.property( "Type", &Component::getType )
		.property( "Node", &Component::getNode, &Component::setNode ),
	
	class_< Transform, Component >( "Transform" )
		// Constructors
		.def( constructor<>() )
		// Methods
		.def( "Translate", (void(Transform::*)(float,float,float)) &Transform::translate )
		.def( "Scale", (void(Transform::*)(float,float,float)) &Transform::scale )
		.def( "Rotate", (void(Transform::*)(float,float,float)) &Transform::rotate )
		// Properties
		.property( "Count", &Group::count )
		.property( "Children", &Group::getChildren )
		// Fields
		//.def_readwrite("a", &A::a)
		//.def_readonly("a", &A::a)
	];
}

//-----------------------------------//

void bindScene( lua_State* luaState )
{
	bindEntity( luaState );
	bindComponents( luaState );
}

//-----------------------------------//

void debug( const std::string& msg )
{
	debug( msg );
}

void bindLogger( lua_State* luaState )
{
	module( luaState, "log" )
	[
		def( "debug", &debug )
		//def( "info", &info ),
		//def( "warn", &warn ),
		//def( "error", &error )
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
		.def( "Load", &ResourceManager::loadResource )
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

void bindMath( lua_State* luaState )
{
	module( luaState )
	[
	class_< Vector3 >( "Vector3" )
		// Constructors
		.def( constructor<>() )
		.def( constructor<const float, const float, const float>() )
		// Methods
		//.def( "update", &Node::update )
		// Properties
		//.property( "name", &Node::getName )
		//.property( "parent", &Node::getParent, &Node::setParent )
		// Fields
		.def_readwrite("X", &Vector3::x)
		.def_readwrite("Y", &Vector3::y)
		.def_readwrite("Z", &Vector3::z)
		//.def_readonly("a", &A::a)
	];
}

//-----------------------------------//

void bindEngine( lua_State* luaState, Engine* engine )
{
	bindLogger( luaState );
	bindResourceManager( luaState );
	bindScene( luaState );
	bindMath( luaState );

	globals(luaState)["ResourceManager"] = engine->getResourceManager();
	globals(luaState)["Scene"] = engine->getSceneManager().get();
}

//-----------------------------------//

} } // end namespaces

#endif