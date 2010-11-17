/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Event.h"

namespace vapor {

//-----------------------------------//

/**
 * Different types of input devices.
 */

namespace InputDeviceType
{
	enum Enum
	{
		Keyboard,
		Mouse,
		Joystick
	};

	std::string getString( InputDeviceType::Enum );
}

//-----------------------------------//

/**
 * Base struct for input events.
 */

struct VAPOR_API InputEvent
{
	DECLARE_UNCOPYABLE(InputEvent)

public:

	InputEvent( InputDeviceType::Enum );
	InputDeviceType::Enum deviceType;
};

//-----------------------------------//

/**
 * Input device.
 */

class VAPOR_API InputDevice
{
	DECLARE_UNCOPYABLE(InputDevice)

public:

	InputDevice() {}
	virtual ~InputDevice() {}

	// Processes an event.
	virtual void processEvent( const InputEvent& event ) = 0;

	// Gets the type of this device.
	virtual const InputDeviceType::Enum getType() const = 0;
};

//-----------------------------------//

} // end namespace