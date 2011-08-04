/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_SCRIPTING_LUA

#include "Controllers/ScriptController.h"
#include "Physics/CharacterController.h"

#include "Script/Script.h"
#include "Script/ScriptManager.h"
#include "Resources/ResourceManager.h"

#include "Scene/Entity.h"

#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Engine/Headers.h"

#include <swigluarun.h>
#include <lua.hpp>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ScriptController, Controller)
	FIELD_CLASS_PTR(7, Script, ScriptHandle, script, Handle)
REFLECT_CLASS_END()

//-----------------------------------//

ScriptController::ScriptController()
	: state(nullptr)
{
	InputManager* im = GetInputManager();
	
	Keyboard* keyboard = im->getKeyboard();
	keyboard->onKeyPress.Connect( this, &ScriptController::onKeyPress );
	keyboard->onKeyRelease.Connect( this, &ScriptController::onKeyRelease );
	
	Mouse* mouse = im->getMouse();
	mouse->onMouseButtonPress.Connect( this, &ScriptController::onMouseButtonPressed );
	mouse->onMouseButtonRelease.Connect( this, &ScriptController::onMouseButtonReleased );
}

//-----------------------------------//

ScriptController::~ScriptController()
{
	InputManager* im = GetInputManager();
	
	Keyboard* keyboard = im->getKeyboard();
	keyboard->onKeyPress.Disconnect( this, &ScriptController::onKeyPress );
	keyboard->onKeyRelease.Disconnect( this, &ScriptController::onKeyRelease );
	
	Mouse* mouse = im->getMouse();
	mouse->onMouseButtonPress.Disconnect( this, &ScriptController::onMouseButtonPressed );
	mouse->onMouseButtonRelease.Disconnect( this, &ScriptController::onMouseButtonReleased );
}

//-----------------------------------//

void ScriptController::_update( float delta )
{
	if( !state && script )
	{
		createState();
		bindEntity(entity);
	}

	if( state )
		state->invoke("onUpdate", 0);
}

//-----------------------------------//

void ScriptController::createState()
{
	Engine* engine = GetEngine();	
	ScriptManager* scripts = engine->getScriptManager();

	state = scripts->createScriptInstance(script.Resolve());
}

//-----------------------------------//

#define BIND_COMPONENT(var, type)						\
	bindType( module, var,								\
		"vapor::"TOSTRING(type)" *",					\
		entity->getComponent<type>().get() );

void ScriptController::bindEntity(Entity* entity)
{
	Engine* engine = GetEngine();
	State* mainState = engine->getScriptManager()->getState();

	swig_module_info* module = SWIG_Lua_GetModule( mainState->getLuaState() );
	assert( module != nullptr );

	bindType(module, "entity", "vapor::Entity *", entity);
	
	BIND_COMPONENT("transform", Transform)
	BIND_COMPONENT("geometry", Geometry)
	BIND_COMPONENT("light", Light)
	BIND_COMPONENT("model", Model)
	BIND_COMPONENT("camera", Camera)

#ifdef VAPOR_PHYSICS_BULLET
	BIND_COMPONENT("characterController", CharacterController)
#endif
}

//-----------------------------------//

void ScriptController::bindType(swig_module_info* module,
								const char* name, const char* nameType, void* object)
{
	if( !object )
		return;

    swig_type_info* type = SWIG_TypeQueryModule(module, module, nameType);
	assert( type != nullptr );

	lua_State* L = state->getLuaState();
    SWIG_Lua_NewPointerObj(L, object, type, 0);

    lua_setglobal(L, name);
}

//-----------------------------------//

void ScriptController::onKeyPress( const KeyEvent& event )
{
	if( !enabled )
		return;

	if( !state )
		return;

	Engine* engine = GetEngine();
	State* mainState = engine->getScriptManager()->getState();

	swig_module_info* module = SWIG_Lua_GetModule( mainState->getLuaState() );
	assert( module != nullptr );

	swig_type_info* type = SWIG_TypeQueryModule(module, module, "vapor::KeyEvent *");
	assert( type != nullptr );

	lua_State* L = state->getLuaState();
    SWIG_Lua_NewPointerObj(L, (void*) &event, type, 0);

	state->invoke("onKeyPress", 1);
}

//-----------------------------------//

void ScriptController::onKeyRelease( const KeyEvent& event )
{
	if( !enabled )
		return;

	if( !state )
		return;

	Engine* engine = GetEngine();
	State* mainState = engine->getScriptManager()->getState();

	swig_module_info* module = SWIG_Lua_GetModule( mainState->getLuaState() );
	assert( module != nullptr );

	swig_type_info* type = SWIG_TypeQueryModule(module, module, "vapor::KeyEvent *");
	assert( type != nullptr );

	lua_State* L = state->getLuaState();
    SWIG_Lua_NewPointerObj(L, (void*) &event, type, 0);

	state->invoke("onKeyRelease", 1);
}

//-----------------------------------//

void ScriptController::onMouseButtonPressed( const MouseButtonEvent& event )
{
	if( !enabled )
		return;

	if( !state )
		return;
}

//-----------------------------------//

void ScriptController::onMouseButtonReleased( const MouseButtonEvent& event )
{
	if( !enabled )
		return;

	if( !state )
		return;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif