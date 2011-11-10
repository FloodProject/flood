/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Input/Keyboard.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Keyboard::Keyboard()
	: keyState(Keys::MAX, false)
{ }

//-----------------------------------//

bool Keyboard::isKeyPressed( Keys::Enum keycode ) const
{
	return keyState[keycode];
}

//-----------------------------------//

void Keyboard::resetKeys()
{
	std::fill(keyState.begin(), keyState.end(), false);
}

//-----------------------------------//

void Keyboard::processEvent( const InputEvent& event )
{
	if( event.deviceType != InputDeviceType::Keyboard )
		return;

	const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);
	
	switch(keyEvent.eventType)
	{
		case KeyboardEventType::KeyPressed:
		{
			keyPressed(keyEvent);
			break;
		}

		case KeyboardEventType::KeyReleased:
		{
			keyReleased(keyEvent);
			break;
		}

		case KeyboardEventType::KeyText:
		{
			onKeyText(keyEvent);
			break;
		}
	}
}

//-----------------------------------//

void Keyboard::keyPressed( const KeyEvent& keyEvent )
{
	// Ignore automatic key repeat.
	if( keyState[keyEvent.keyCode] )
		return;

	keyState[keyEvent.keyCode] = true;
	lastKey = keyEvent.keyCode;

	onKeyPress(keyEvent);
}

//-----------------------------------//

void Keyboard::keyReleased( const KeyEvent& keyEvent )
{
	keyState[keyEvent.keyCode] = false;

	onKeyRelease(keyEvent);
}

//-----------------------------------//

KeyEvent::KeyEvent(KeyboardEventType::Enum eventType)
	: InputEvent(InputDeviceType::Keyboard)
	, eventType(eventType)
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END