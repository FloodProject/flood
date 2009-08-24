/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/input/Device.h"

namespace vapor {
	namespace input {

//-----------------------------------//

namespace JoystickAxis
{
	enum Enum
	{
		AxisX,
		AxisY,
		AxisZ,
		AxisR,
		AxisU,
		AxisV,
		AxisPOV,
	};
}

//-----------------------------------//

namespace JoystickEventType
{
	enum Enum
	{
		JoystickPress,
		JoystickRelease,
		JoystickMove
	};
}

//-----------------------------------//

struct JoystickEvent : public input::Event
{
friend class Mouse;

JoystickEvent( JoystickEventType::Enum eventType );

private:

JoystickEventType::Enum eventType;
};

struct JoyMoveEvent: public JoystickEvent
{
	JoyMoveEvent(unsigned int JoystickId, JoystickAxis::Enum Axis,
		float Position): JoystickEvent(JoystickEventType::JoystickMove),
		JoystickId(JoystickId), Axis(Axis), Position(Position){}

	unsigned int JoystickId;
	JoystickAxis::Enum	 Axis;
	float        Position;
};

//-----------------------------------//

struct JoyButtonEvent: public JoystickEvent
{	
	JoyButtonEvent(unsigned int JoystickId, unsigned int Button, 
		JoystickEventType::Enum eventType): JoystickEvent(eventType),
		JoystickId(JoystickId), Button(Button){}

	unsigned int JoystickId;
	unsigned int Button;
};

//-----------------------------------//

class Joystick : public Device
{
	friend class InputManager;
	//TODO:: write class
	
};

//-----------------------------------//

} } // end namespaces