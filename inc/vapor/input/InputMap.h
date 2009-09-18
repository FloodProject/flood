/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/input/InputManager.h"

#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"
#include "vapor/input/Joystick.h"

namespace vapor {
	namespace input {

//-----------------------------------//

struct joyId
{
	joyId(int, int);
	int id;
	int button;
	int operator<(joyId joy1, joyId, joy2);
};

//-----------------------------------//

class InputMap
{
public:

	InputMap( const InputManager& manager );
	~InputMap();

	fd::delegate<void(void)>& registerAction(const std::string&, Keys::Enum);
	fd::delegate<void(void)>& registerAction(const std::string&, MouseButton::Enum);
	fd::delegate<void(void)>& registerAction(const std::string&, joyId);
	fd::delegate<void(void)>& getFunction(const std::string&);
	
	void onKeyPress(const KeyEvent& ke);
	void onMousePress(const MouseButtonEvent& mbe);
	void onJoyPress(const JoyButtonEvent& jbe);

private:

	std::map< Keys::Enum, std::string > keymap;
	std::map< MouseButton::Enum, std::string > mousemap;
	std::map< joyId, std::string > joystickmap;
	std::map< std::string, fd::delegate<void(void)> > inputMap; 
};

//-----------------------------------//

} } // end namespaces