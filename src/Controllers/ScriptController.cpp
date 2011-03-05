/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SCRIPTING_LUA

#include "controllers/ScriptController.h"
#include "physics/CharacterController.h"

#include "script/Script.h"
#include "script/ScriptManager.h"
#include "resources/ResourceManager.h"

#include "scene/Entity.h"
#include "Engine.h"

#include "input/InputManager.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "Headers.h"

#include <swigluarun.h>
#include <lua.hpp>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(ScriptController, Controller)
	FIELD_CLASS_PTR(Script, ScriptPtr, script, RefPointer)
END_CLASS()

//-----------------------------------//

ScriptController::ScriptController()
	: state(nullptr)
{
	InputManager* im = GetEngine()->getInputManager();
	
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
	InputManager* im = GetEngine()->getInputManager();
	
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
		state->invoke("onUpdate");
}

//-----------------------------------//

void ScriptController::createState()
{
	Engine* engine = GetEngine();	
	ScriptManager* scripts = engine->getScriptManager();

	assert( script != nullptr );
	state = scripts->createScriptInstance(script);
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
	BIND_COMPONENT("characterController", CharacterController)
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

} // end namespace

#endif