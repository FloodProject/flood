/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Input/Joystick.h"

namespace vapor {

//-----------------------------------//

JoystickID::JoystickID(int id, int button)
	: id(id)
	, button(button)
{ }

//-----------------------------------//

JoystickEvent::JoystickEvent( JoystickEventType::Enum eventType )
	: InputEvent( InputDeviceType::Joystick )
	, eventType( eventType )
{ }

//-----------------------------------//

const InputDeviceType::Enum Joystick::getType()
{
	return InputDeviceType::Joystick; 
}

//-----------------------------------//

void Joystick::processEvent( const InputEvent& event )
{
	if( event.deviceType != InputDeviceType::Joystick )
		return;

	const JoystickEvent& jevt = static_cast< const JoystickEvent& > ( event );
	
	switch( jevt.eventType )
	{
	case JoystickEventType::JoystickPress:
	{
		const JoyButtonEvent& jbe = 
			static_cast< const JoyButtonEvent& > ( jevt );
		joyButtonPressed( jbe );
		break;
	}
	case JoystickEventType::JoystickRelease:
	{
		const JoyButtonEvent& jbe = 
			static_cast< const JoyButtonEvent& > ( jevt );
		joyButtonReleased( jbe );
		break;
	}
	case JoystickEventType::JoystickMove:
	{
		const JoyMoveEvent& jm = 
			static_cast< const JoyMoveEvent& > ( jevt );
		joyMoved( jm );
		break;
	}
	}
}
	
//-----------------------------------//

void Joystick::joyMoved(const JoyMoveEvent& jme)
{
	onJoystickMove( jme );
}

//-----------------------------------//

void Joystick::joyButtonPressed(const JoyButtonEvent& jbp)
{
	onJoystickButtonPress( jbp );
}

//-----------------------------------//

void Joystick::joyButtonReleased(const JoyButtonEvent& jbe)
{
	onJoystickButtonRelease( jbe );
}

//-----------------------------------//

JoyMoveEvent::JoyMoveEvent(uint32 id, JoystickAxis::Enum axis, float pos)
	: JoystickEvent(JoystickEventType::JoystickMove)
	, JoystickId(id)
	, Axis(axis)
	, Position(pos)
{ }

//-----------------------------------//

JoyButtonEvent::JoyButtonEvent(uint32 JoystickId, uint32 Button, JoystickEventType::Enum eventType)
	: JoystickEvent(eventType)
	, JoystickId(JoystickId)
	, Button(Button)
{ }

//-----------------------------------//

} // end namespace
