/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Input/InputMap.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

InputMap::InputMap(const InputManager& input)
{
	Keyboard* keyboard = input.getKeyboard();
	keyboard->onKeyPress.Connect( this, &InputMap::onKeyPress );
	
	Mouse* mouse = input.getMouse();
	mouse->onMouseButtonPress.Connect( this, &InputMap::onMousePress );
}

//-----------------------------------//

InputMap::~InputMap()
{
	Keyboard* keyboard = GetInputManager()->getKeyboard();
	keyboard->onKeyPress.Disconnect( this, &InputMap::onKeyPress );
	
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