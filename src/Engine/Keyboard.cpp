/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/Keyboard.h"

namespace vapor {
	namespace input {

//-----------------------------------//

Keyboard::Keyboard()
: keyState(Keys::Pause, false), lastKey(Keys::Space)
{

}

//-----------------------------------//

Keyboard::~Keyboard()
{

}

//-----------------------------------//

const input::DeviceType::Enum Keyboard::getType()
{
	return input::DeviceType::Keyboard; 
}

//-----------------------------------//

void Keyboard::processEvent( const input::Event& event )
{
	if( event.deviceType != DeviceType::Keyboard )
	{
		return;
	}

	const KeyEvent& kevt = static_cast< const KeyEvent& > ( event );
	
	switch(kevt.eventType)
	{
		case KeyboardEventType::KeyPressed:
		{
			keyPressed( kevt );
			break;
		}

		case KeyboardEventType::KeyReleased:
		{
			keyReleased( kevt );
			break;
		}
	}
}

//-----------------------------------//

KeyEvent::KeyEvent(Keys::Enum keyCode, bool alt, bool shift, bool ctrl, 
					KeyboardEventType::Enum eventType)
	: Event( DeviceType::Keyboard ), keyCode( keyCode ), altPressed( alt ),
		shiftPressed( shift ), ctrlPressed( ctrl ), eventType( eventType )
{

}

//-----------------------------------//

bool Keyboard::isKeyPressed( Keys::Enum keycode )
{
	return keyState[keycode];
}

//-----------------------------------//

void Keyboard::keyPressed( const KeyEvent& keyEvent )
{
	keyState[keyEvent.keyCode] = true;
	lastKey = keyEvent.keyCode;

	if( !onKeyPress.empty() )
	{
		// fire the public event!
		onKeyPress( keyEvent );
	}
}

//-----------------------------------//

void Keyboard::keyReleased( const KeyEvent& keyEvent )
{
	keyState[keyEvent.keyCode] = false;
	//lastKey = keycode;

	if( !onKeyRelease.empty() )
	{
		// fire the public event!
		onKeyRelease( keyEvent );
	}
}	

//-----------------------------------//

//Keys::Enum Keyboard::getLastKeyPressed()
//{
//	return lastKey;
//}

//-----------------------------------//

//const KeyEvent& Keyboard::getLastKeyEvent()
//{
//	bool alt, shift, ctrl;
//
//	alt = keyState[Keys::LAlt] || keyState[Keys::RAlt];
//	shift = keyState[Keys::LShift] || keyState[Keys::RShift];
//	ctrl = keyState[Keys::LControl] || keyState[Keys::RControl];
//
//	KeyEvent keyinfo( lastKey, alt, shift, ctrl );
//
//	return *keyinfo;
//}

//-----------------------------------//

} } // end namespaces