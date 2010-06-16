/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/input/Device.h"

namespace vapor {

//-----------------------------------//

/**
 * Different types of joystick axis.
 */

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

/**
 * Different types of joystick events.
 */

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

/**
 * Generic event for joystick actions.
 */

struct JoystickEvent : public Event
{
	friend class Joystick;

	JoystickEvent( JoystickEventType::Enum eventType );

private:

	JoystickEventType::Enum eventType;
};

//-----------------------------------//

/**
 * Event that happens when a joystick is moved.
 */

struct JoyMoveEvent: public JoystickEvent
{
	JoyMoveEvent(uint id, JoystickAxis::Enum axis, float pos);
	
	uint JoystickId;
	JoystickAxis::Enum Axis;
	float Position;
};

//-----------------------------------//

/**
 * Event that happens when a joystick button is pressed or released.
 */

struct JoyButtonEvent: public JoystickEvent
{	
	JoyButtonEvent(uint JoystickId, uint Button, 
		JoystickEventType::Enum eventType);

	uint JoystickId;
	uint Button;
};

//-----------------------------------//

class VAPOR_API Joystick : public Device
{
	// Processes an event (only if it's a joystick event).
	virtual void processEvent( const Event& event );

	// Return this device as a joystick.
	virtual const DeviceType::Enum getType();

	//-----------------------------------//
	// Events
	//-----------------------------------//

	// Occurs when the joystick is moved.
	fd::delegate< void( const JoyMoveEvent& ) > onJoystickMove;
	
	// Occurs when a joystick button is pressed.
	fd::delegate< void( const JoyButtonEvent& ) > onJoystickButtonPress;
	
	// Occurs when a joystick button is released.
	fd::delegate< void( const JoyButtonEvent& ) > onJoystickButtonRelease;

private:

	// Private event handlers.
	void joyButtonPressed(const JoyButtonEvent& jevt);
	void joyButtonReleased(const JoyButtonEvent& jevt);
	void joyMoved(const JoyMoveEvent& jevt);
};

//-----------------------------------//

} // end namespace