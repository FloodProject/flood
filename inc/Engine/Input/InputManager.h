/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

void API_INPUT InputInitialize();
void API_INPUT InputDeinitialize();

class InputDevice;
struct InputEvent;

class Keyboard;
class Mouse;
class Joystick;

class Window;

/**
 * Manages a set of input devices.
 */

class InputManager;
API_INPUT InputManager* GetInputManager();

class API_INPUT InputManager
{
	DECLARE_UNCOPYABLE(InputManager)

public:

	InputManager();
	virtual ~InputManager();

	/// Gets a keyboard device if it exists.
	virtual Keyboard* getKeyboard() = 0;
	
	/// Gets a mouse device if it exists.
	virtual Mouse* getMouse() = 0;
};

//-----------------------------------//

NAMESPACE_ENGINE_END