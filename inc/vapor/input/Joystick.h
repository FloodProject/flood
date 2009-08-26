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
friend class Joystick;
JoystickEvent( JoystickEventType::Enum eventType );

private:

JoystickEventType::Enum eventType;
};

struct JoyMoveEvent: public JoystickEvent
{
	JoyMoveEvent(unsigned int JoystickId, JoystickAxis::Enum Axis, float Position);
	
	unsigned int JoystickId;
	JoystickAxis::Enum	 Axis;
	float        Position;
};

//-----------------------------------//

struct JoyButtonEvent: public JoystickEvent
{	
	JoyButtonEvent(unsigned int JoystickId, unsigned int Button, JoystickEventType::Enum eventType);

	unsigned int JoystickId;
	unsigned int Button;
};


//-----------------------------------//

class Joystick : public Device
{
	Joystick();
	virtual ~Joystick();
	
	// Processes an event (only if it's a joystick event).
	virtual void processEvent( const input::Event& event );

	// Return this device as a joystick.
	virtual const input::DeviceType::Enum getType();

	//-----------------------------------//
	// Events
	//-----------------------------------//

	fd::delegate< void( const JoyMoveEvent& ) > onJoystickMove;
	
	fd::delegate< void( const JoyButtonEvent& ) > onJoystickButtonPress;
	fd::delegate< void( const JoyButtonEvent& ) > onJoystickButtonRelease;
	

private:

	// Occurs when a joystick button is pressed.
	void joyButtonPressed(const JoyButtonEvent& jevt);
	
	// Occurs when a joystick button is released.
	void joyButtonReleased(const JoyButtonEvent& jevt);
	
	// Occurs when the joystick is moved.
	void joyMoved(const JoyMoveEvent& jevt);
	
};

//-----------------------------------//

} } // end namespaces