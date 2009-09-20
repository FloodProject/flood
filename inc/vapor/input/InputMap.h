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
	
};
struct Compare{
	bool operator()(joyId lhs, joyId rhs)
	{
		return lhs.id < rhs.id;
	}
};

//-----------------------------------//

class VAPOR_API InputMap : private boost::noncopyable
{
public:

	InputMap( const InputManager& manager );
	~InputMap();

	fd::delegate<void(void)> * registerAction(const std::string&, Keys::Enum);
	fd::delegate<void(void)> * registerAction(const std::string&, MouseButton::Enum);
	fd::delegate<void(void)> * registerAction(const std::string&, joyId);
	fd::delegate<void(void)> * getFunction(const std::string&);
	
	void onKeyPress(const KeyEvent& ke);
	void onMousePress(const MouseButtonEvent& mbe);
	void onJoyPress(const JoyButtonEvent& jbe);

private:

	std::map< Keys::Enum, std::string > keymap;
	std::map< MouseButton::Enum, std::string > mousemap;
	std::map< joyId, std::string, Compare> joystickmap;
	std::map< std::string, fd::delegate<void(void)> > inputMap; 
};

//-----------------------------------//

} } // end namespaces
