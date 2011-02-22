
/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/InputMap.h"

namespace vapor {

//-----------------------------------//

joyId::joyId(int id, int button)
	:id(id), button(button)
{

}

//-----------------------------------//

InputMap::InputMap(const InputManager& manager)
{
	Keyboard* kbd = manager.getKeyboard();
	Mouse* mouse = manager.getMouse();
	
	kbd->onKeyPress.Connect( this, &InputMap::onKeyPress );
	mouse->onMouseButtonPress.Connect( this, &InputMap::onMousePress );
}

//-----------------------------------//

InputMap::~InputMap()
{

}

//-----------------------------------//

Event0<>* InputMap::registerAction(const std::string& action, Keys::Enum keycode)
{
	keymap[keycode] = action;
	//return &(inputMap[action]);
	return nullptr;
}

//-----------------------------------//

Event0<>* InputMap::registerAction(const std::string& action, MouseButton::Enum button)
{
	mousemap[button] = action;
	//return &(inputMap[action]);
	return nullptr;
}

//-----------------------------------//

Event0<>* InputMap::registerAction(const std::string& action, joyId joy)
{
	joystickmap[joy] = action;
	//return &(inputMap[action]);
	return nullptr;
}

//-----------------------------------//

Event0<>* InputMap::getFunction(const std::string& action)
{
	//return &(inputMap[action]);
	return nullptr;
}

//-----------------------------------//

void InputMap::onKeyPress(const KeyEvent& ke)
{
	if(keymap.find(ke.keyCode) != keymap.end())
	{
		std::string action = keymap[ke.keyCode];
		//inputMap[action]();
	}
}

//-----------------------------------//

void InputMap::onMousePress(const MouseButtonEvent& mbe)
{
	if(mousemap.find(mbe.button) != mousemap.end())
	{
		std::string action = mousemap[mbe.button];
		//inputMap[action]();
	}
}

//-----------------------------------//

void InputMap::onJoyPress(const JoyButtonEvent& jbe)
{
	joyId joy((int)jbe.Button, (int)jbe.JoystickId); 
	if(joystickmap.find(joy) != joystickmap.end())
	{
		std::string action = joystickmap[joy];
		//inputMap[action]();
	}
}

//-----------------------------------//

} // end namespace