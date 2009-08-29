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

//#include <boost/bimap.hpp>

//-----------------------------------//

namespace vapor {
	namespace input {

//-----------------------------------//

union Action 
{
	Keys::Enum key;
	MouseButton::Enum mouseButton;

	//joyId joyButton;
};

//-----------------------------------//

//struct joyId
//{
//	int id;
//	int button;
//};

//-----------------------------------//

class InputMap
{
	typedef std::pair< DeviceType::Enum, Action > input;

public:

	InputMap( InputManager& manager );
	~InputMap();

	void onKeyPress( const KeyEvent& ke );
	void onMousePress( const MouseButtonEvent& mbe );
	void onJoyPress( const JoyButtonEvent& jbe );

	void operator[]( std::string action );

private:

	//boost::bimap< std::string, input > inputMap;	
};

//-----------------------------------//

} } // end namespaces