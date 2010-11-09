/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/input/InputManager.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"
#include "vapor/input/Joystick.h"

namespace vapor {

//-----------------------------------//

struct joyId
{
	joyId(int, int);
	int id;
	int button;
	
};

//-----------------------------------//

struct Compare
{
	bool operator()(joyId lhs, joyId rhs) const
	{
		return lhs.id < rhs.id;
	}
};

//-----------------------------------//

class VAPOR_API InputMap
{
	DECLARE_UNCOPYABLE(InputMap);

public:

	InputMap( const InputManager& manager );
	~InputMap();

	Event0<>* registerAction(const std::string&, Keys::Enum);
	Event0<>* registerAction(const std::string&, MouseButton::Enum);
	Event0<>* registerAction(const std::string&, joyId);
	Event0<>* getFunction(const std::string&);
	
	void onKeyPress(const KeyEvent& ke);
	void onMousePress(const MouseButtonEvent& mbe);
	void onJoyPress(const JoyButtonEvent& jbe);

private:

	std::map< Keys::Enum, std::string > keymap;
	std::map< MouseButton::Enum, std::string > mousemap;
	std::map< joyId, std::string, Compare> joystickmap;
	std::map< std::string, Event0<>* > inputMap; 
};

//-----------------------------------//

} // end namespace
