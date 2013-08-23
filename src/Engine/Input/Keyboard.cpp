/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Input/Keyboard.h"
#include "Core/Array.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Keyboard::Keyboard()
{
	keyState.resize((size_t)Keys::MAX);
	for(auto ks : keyState)
		ks = false;
}

//-----------------------------------//

bool Keyboard::isKeyPressed( Keys keycode ) const
{
	return keyState[(int)keycode];
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
	} }
}

//-----------------------------------//

void Keyboard::keyPressed( const KeyEvent& keyEvent )
{
	// Ignore automatic key repeat.
	if( keyState[(int)keyEvent.keyCode] )
		return;

	keyState[(int)keyEvent.keyCode] = true;
	lastKey = keyEvent.keyCode;

	onKeyPress(keyEvent);
}

//-----------------------------------//

void Keyboard::keyReleased( const KeyEvent& keyEvent )
{
	keyState[(int)keyEvent.keyCode] = false;

	onKeyRelease(keyEvent);
}

//-----------------------------------//

KeyEvent::KeyEvent(KeyboardEventType eventType)
	: InputEvent(InputDeviceType::Keyboard)
	, eventType(eventType)
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END