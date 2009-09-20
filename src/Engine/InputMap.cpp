
/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/InputMap.h"

namespace vapor {
	namespace input {


joyId::joyId(int id, int button)
	:id(id), button(button)
{

}



InputMap::InputMap(const InputManager& manager)
{

	Keyboard* kbd = manager.getKeyboard();
	Mouse* mouse = manager.getMouse();
	
	kbd->onKeyPress.bind( &InputMap::onKeyPress, this );
	mouse->onMouseButtonPress.bind( &InputMap::onMousePress, this );
}

InputMap::~InputMap()
{

}

fd::delegate<void(void)> * InputMap::registerAction(const std::string& action, Keys::Enum keycode)
{
	keymap[keycode] = action;
	return &(inputMap[action]);
}

fd::delegate<void(void)> * InputMap::registerAction(const std::string& action, MouseButton::Enum button)
{
	mousemap[button] = action;
	return &(inputMap[action]);
	
}

fd::delegate<void(void)> * InputMap::registerAction(const std::string& action, joyId joy)
{
	joystickmap[joy] = action;
	return &(inputMap[action]);
}

fd::delegate<void(void)> * InputMap::getFunction(const std::string& action)
{
		return &(inputMap[action]);
}

void InputMap::onKeyPress(const KeyEvent& ke)
{
	if(keymap.find(ke.keyCode) != keymap.end())
	{
		std::string action = keymap[ke.keyCode];
		if(!(inputMap[action]).empty())
			(inputMap[action])();
	}
	
}

void InputMap::onMousePress(const MouseButtonEvent& mbe)
{
	if(mousemap.find(mbe.button) != mousemap.end())
	{
		std::string action = mousemap[mbe.button];
		if(!(inputMap[action]).empty())
			(inputMap[action])();
	}

}

void InputMap::onJoyPress(const JoyButtonEvent& jbe)
{
	joyId joy((int)jbe.Button, (int)jbe.JoystickId); 
	if(joystickmap.find(joy) != joystickmap.end())
	{
		std::string action = joystickmap[joy];
		if(!(inputMap[action]).empty())
			(inputMap[action])();
	}
}








	}
}
