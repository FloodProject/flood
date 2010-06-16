/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/Keyboard.h"

namespace vapor {

//-----------------------------------//

Keyboard::Keyboard()
	: keyState(1024, false),
	lastKey(Keys::Space)
// TODO: keyState size hack! previous solution, assigning it to Keys::Pause
// was causing a heap overflow, probably was off-by-1 error, but assigning
// a bigger size, like 1024 is probably safer...
{ }

//-----------------------------------//

void Keyboard::resetKeys()
{
	std::fill(keyState.begin(), keyState.end(), false);
}

//-----------------------------------//

void Keyboard::processEvent( const Event& event )
{
	if( event.deviceType != DeviceType::Keyboard )
		return;

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

void Keyboard::keyPressed( const KeyEvent& keyEvent )
{
	keyState[keyEvent.keyCode] = true;
	lastKey = keyEvent.keyCode;

	if( !onKeyPress.empty() )
	{
		onKeyPress( keyEvent );
	}
}

//-----------------------------------//

void Keyboard::keyReleased( const KeyEvent& keyEvent )
{
	keyState[keyEvent.keyCode] = false;

	if( !onKeyRelease.empty() )
	{
		onKeyRelease( keyEvent );
	}
}

//-----------------------------------//

KeyEvent::KeyEvent(Keys::Enum keyCode, 
					bool alt, bool shift, bool ctrl,
					KeyboardEventType::Enum eventType)
	: Event(DeviceType::Keyboard), keyCode(keyCode),
	altPressed(alt), shiftPressed(shift), ctrlPressed(ctrl),
	eventType( eventType )
{ }

//-----------------------------------//

bool Keyboard::isKeyPressed( Keys::Enum keycode ) const
{
	return keyState[keycode];
}

//-----------------------------------//

} // end namespace