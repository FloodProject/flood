/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#include "Engine/Controllers/ScriptController.h"
#include "Engine/Physics/CharacterController.h"

#include "Script/Script.h"
#include "Script/ScriptManager.h"
#include "Resources/ResourceManager.h"

#include "Engine/Scene/Entity.h"

#include "Engine/Input/InputManager.h"
#include "Engine/Input/Keyboard.h"
#include "Engine/Input/Mouse.h"

#include "Engine/Headers.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ScriptController, Controller)
	FIELD_CLASS_PTR(7, Script, ScriptHandle, script, Handle)
REFLECT_CLASS_END()

//-----------------------------------//

ScriptController::ScriptController()
	: state(nullptr)
{
}

//-----------------------------------//

ScriptController::~ScriptController()
{
}

//-----------------------------------//

void ScriptController::_update( float delta )
{
	if( !state && script )
	{
		createState();
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

void ScriptController::onKeyPress( const KeyEvent& event )
{
	if( !enabled || !state )
		return;

	Engine* engine = GetEngine();
}

//-----------------------------------//

void ScriptController::onKeyRelease( const KeyEvent& event )
{
	if( !enabled || !state )
		return;

	Engine* engine = GetEngine();
}

//-----------------------------------//

void ScriptController::onMouseButtonPressed( const MouseButtonEvent& event )
{
	if( !enabled || !state )
		return;
}

//-----------------------------------//

void ScriptController::onMouseButtonReleased( const MouseButtonEvent& event )
{
	if( !enabled || !state )
		return;
}

//-----------------------------------//

NAMESPACE_ENGINE_END