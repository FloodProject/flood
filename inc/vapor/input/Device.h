/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace input {

//-----------------------------------//

/**
 * Different types of input devices.
 */

namespace DeviceType
{
	enum Enum
	{
		Keyboard,
		Mouse,
		Joystick
	};

	std::string getString( const DeviceType::Enum dt );
}

//-----------------------------------//

/**
 * Base struct for input events.
 */

struct Event : private boost::noncopyable
{
	Event( DeviceType::Enum );

	DeviceType::Enum deviceType;
};

//-----------------------------------//

/**
 * Input device.
 */

class VAPOR_API Device : private boost::noncopyable
{
public:

	Device();
	virtual ~Device();

	// Processes an event.
	virtual void processEvent( const input::Event& event ) = 0;

	// Gets the type of this device.
	virtual const input::DeviceType::Enum getType() = 0;
};

//-----------------------------------//

} } // end namespaces