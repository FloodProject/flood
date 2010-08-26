/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/input/Device.h"
#include "vapor/input/KeyboardEvents.h"

namespace vapor {

//-----------------------------------//

/**
 * Keyboard device.
 */

class VAPOR_API Keyboard : public InputDevice
{
public:	

	Keyboard();

	// Returns the current state of the keys.
	GETTER(KeyState, const std::vector<bool>&, keyState)

	// Checks if the given key is pressed.
	bool isKeyPressed( Keys::Enum keyCode ) const;

	// Resets the keys state.
	void resetKeys();

	//-----------------------------------//
	// Events
	//-----------------------------------//

	fd::delegate< void( const KeyEvent& ) > onKeyPress;
	fd::delegate< void( const KeyEvent& ) > onKeyRelease;

private:
	
	// Processes an event (only if it's a keyboard event).
	virtual void processEvent( const InputEvent& event );

	// Return this device as a keyboard.
	GETTER(Type, const InputDeviceType::Enum, InputDeviceType::Keyboard)

	// Handles a key press event.
	void keyPressed( const KeyEvent& keyEvent );
	
	// Handles a key release event.
	void keyReleased( const KeyEvent& keyEvent );
	
	// Current key state.
	std::vector<bool> keyState;
	
	// Last key pressed.
	Keys::Enum lastKey;
};

//-----------------------------------//

} // end namespace
