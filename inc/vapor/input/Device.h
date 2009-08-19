/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/input/Event.h"

namespace vapor {
	namespace input {

/**
 * Different types of input devices.
 */

namespace Type
{
	enum Enum
	{
		Keyboard,
		Mouse,
		Joystick
	};
}

/**
 * Input device.
 */

class Device
{
public:

	// Processes an event.
	virtual void processEvent( const Event& event ) = 0;

	// Gets the type of this device.
	virtual Type::Enum& getType() = 0;
};

} } // end namespaces