/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Input/InputMap.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

InputMap::InputMap(const InputManager& input)
{
	Keyboard* kbd = input.getKeyboard();
	kbd->onKeyPress.Connect( this, &InputMap::onKeyPress );
	
	Mouse* mouse = input.getMouse();	
	mouse->onMouseButtonPress.Connect( this, &InputMap::onMousePress );
}

//-----------------------------------//

InputMap::~InputMap()
{
	Keyboard* kbd = GetInputManager()->getKeyboard();
	kbd->onKeyPress.Disconnect( this, &InputMap::onKeyPress );
	
	Mouse* mouse = GetInputManager()->getMouse();	
	mouse->onMouseButtonPress.Disconnect( this, &InputMap::onMousePress );
}

//-----------------------------------//

void InputMap::onKeyPress(const KeyEvent& event)
{
}

//-----------------------------------//

void InputMap::onMousePress(const MouseButtonEvent& event)
{
}

//-----------------------------------//

void InputMap::onJoyPress(const JoyButtonEvent& event)
{
}

//-----------------------------------//

NAMESPACE_ENGINE_END