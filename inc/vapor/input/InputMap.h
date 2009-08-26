

#pragma once

#include "vapor/Platform.h"
#include <boost/bimap.hpp>

namespace vapor {
	namespace input {

union Action 
{
	Keys::Enum key;
	MouseButton::Enum mouseButton; 
	joyId joyButton;
		
};

struct joyId
{
	int id;
	int button;
};

class InputMap
{

	typedef std::pair<DeviceType::Enum, Action> input;

public:
	InputMap(InputManager manager);
	~InputMap();
	void onKeyPress(const KeyEvent& ke);
	void onMousePress(const MouseButtonEvent& mbe);
	void onJoyPress(const JoyButtonEvent& jbe);

	void operator[](std::string);
	

	
private:

	boost::bimap<std::string, input> inputMap;
	
}



	}
}