/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/input/Joystick.h"

namespace vapor { namespace input {

//-----------------------------------//

JoystickEvent::JoystickEvent( JoystickEventType::Enum eventType )
	: Event( DeviceType::Joystick ), eventType( eventType )
{

}

//-----------------------------------//

const input::DeviceType::Enum Joystick::getType()
{
	return input::DeviceType::Joystick; 
}

//-----------------------------------//

void Joystick::processEvent( const input::Event& event )
{
	if( event.deviceType != DeviceType::Joystick )
	{
		return;
	}

	const JoystickEvent& jevt = 
		static_cast< const JoystickEvent& > ( event );
	
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

	if ( !onJoystickMove.empty() )
	{
		onJoystickMove( jme );
	}
}

//-----------------------------------//

void Joystick::joyButtonPressed(const JoyButtonEvent& jbp)
{

	if ( !onJoystickButtonPress.empty() )
	{
		onJoystickButtonPress( jbp );
	}
}

//-----------------------------------//

void Joystick::joyButtonReleased(const JoyButtonEvent& jbe)
{

	if ( !onJoystickButtonRelease.empty() )
	{
		onJoystickButtonRelease( jbe );
	}
}

//-----------------------------------//

JoyMoveEvent::JoyMoveEvent(unsigned int JoystickId, JoystickAxis::Enum Axis,
		float Position): JoystickEvent(JoystickEventType::JoystickMove),
		JoystickId(JoystickId), Axis(Axis), Position(Position)
{

}

//-----------------------------------//

JoyButtonEvent::JoyButtonEvent(unsigned int JoystickId, unsigned int Button, 
		JoystickEventType::Enum eventType): JoystickEvent(eventType), 
		JoystickId(JoystickId), Button(Button)
{

}




//-----------------------------------//



} } // end namespaces