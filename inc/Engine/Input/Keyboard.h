/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Engine/Input/Device.h"
#include "Engine/Input/KeyboardEvents.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Keyboard device.
 */

class API_ENGINE Keyboard : public InputDevice
{
public:

	Keyboard();

	// Return this device as a keyboard.
	GETTER(Type, const InputDeviceType, InputDeviceType::Keyboard)

	// Processes an event (only if it's a keyboard event).
	FLD_IGNORE virtual void processEvent(const InputEvent& event) override;

	// Checks if the given key is pressed.
	bool isKeyPressed( Keys keyCode ) const;

	// Resets the keys state.
	void resetKeys();

	// Returns the current state of the keys.
	GETTER(KeyState, const Vector<bool>&, keyState)

	// Event is sent when a key is pressed.
	Event1<const KeyEvent&> onKeyPress;

	// Event is sent when a key is released.
	Event1<const KeyEvent&> onKeyRelease;

	// Event is sent when text is input.
	Event1<const KeyEvent&> onKeyText;

private:

	// Handles a key press event.
	FLD_IGNORE void keyPressed( const KeyEvent& keyEvent );
	
	// Handles a key release event.
	FLD_IGNORE void keyReleased( const KeyEvent& keyEvent );

	// Current key state.
	Vector<bool> keyState;

	// Last key pressed.
	Keys lastKey;
};

//-----------------------------------//

NAMESPACE_ENGINE_END
