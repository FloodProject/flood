/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/controllers/ScriptController.h"
#include "vapor/physics/CharacterController.h"

#include "vapor/script/Script.h"
#include "vapor/script/ScriptManager.h"
#include "vapor/resources/ResourceManager.h"

#include "vapor/scene/Node.h"
#include "vapor/Engine.h"

#include "vapor/input/InputManager.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"

#include <swigluarun.h>
#include <lua.hpp>

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(ScriptController, Controller)
	FIELD_PRIMITIVE(ScriptController, string, scriptName)
	FIELD_CLASS_PTR(ScriptController, Script, script)
END_CLASS()

//-----------------------------------//

ScriptController::ScriptController()
	: state(nullptr)
{
	InputManager* im = Engine::getInstancePtr()->getInputManager();
	
	Keyboard* keyboard = im->getKeyboard();
	keyboard->onKeyPress += fd::bind( &ScriptController::onKeyPress, this );
	keyboard->onKeyRelease += fd::bind( &ScriptController::onKeyRelease, this );
	
	Mouse* mouse = im->getMouse();
	mouse->onMouseButtonPress += fd::bind( &ScriptController::onMouseButtonPressed, this );
	mouse->onMouseButtonRelease += fd::bind( &ScriptController::onMouseButtonReleased, this );
}

//-----------------------------------//

ScriptController::~ScriptController()
{
	InputManager* im = Engine::getInstancePtr()->getInputManager();
	
	Keyboard* keyboard = im->getKeyboard();
	keyboard->onKeyPress -= fd::bind( &ScriptController::onKeyPress, this );
	keyboard->onKeyRelease -= fd::bind( &ScriptController::onKeyRelease, this );
	
	Mouse* mouse = im->getMouse();
	mouse->onMouseButtonPress -= fd::bind( &ScriptController::onMouseButtonPressed, this );
	mouse->onMouseButtonRelease -= fd::bind( &ScriptController::onMouseButtonReleased, this );
}

//-----------------------------------//

void ScriptController::_update( double delta )
{
	if( !script && !scriptName.empty() )
		script = getScript();

	if( !state && script )
	{
		createState();
		bindNode();
	}

	if( state )
		state->invoke("onUpdate");
}

//-----------------------------------//

void ScriptController::createState()
{
	Engine* engine = Engine::getInstancePtr();	
	ScriptManager* scripts = engine->getScriptManager();

	assert( script != nullptr );
	state = scripts->createScriptInstance(script);
}

//-----------------------------------//

#define BIND_COMPONENT(var, type)						\
	bindType(module, var, "vapor::"TOSTRING(type)" *",	\
	node->getComponent<Component>(TOSTRING(type)).get());

void ScriptController::bindNode()
{
	const NodePtr& node = getNode();
	assert( node != nullptr );

	Engine* engine = Engine::getInstancePtr();
	State* mainState = engine->getScriptManager()->getState();

	swig_module_info* module = SWIG_Lua_GetModule( mainState->getLuaState() );
	assert( module != nullptr );

	bindType(module, "node", "vapor::Node *", node.get());
	
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

ScriptPtr ScriptController::getScript()
{
	Engine* engine = Engine::getInstancePtr();

	ResourceManager* resources = engine->getResourceManager();
	return resources->loadResource<Script>(scriptName);
}

//-----------------------------------//

void ScriptController::onKeyPress( const KeyEvent& event )
{
	if( !enabled )
		return;

	if( !state )
		return;

	Engine* engine = Engine::getInstancePtr();
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

	Engine* engine = Engine::getInstancePtr();
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