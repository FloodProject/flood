/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

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

	std::string getString( const InputDeviceType::Enum dt );
}

//-----------------------------------//

/**
 * Base struct for input events.
 */

struct VAPOR_API InputEvent : private boost::noncopyable
{
	InputEvent( InputDeviceType::Enum );

	InputDeviceType::Enum deviceType;
};

//-----------------------------------//

/**
 * Input device.
 */

class VAPOR_API InputDevice : private boost::noncopyable
{
public:

	virtual ~InputDevice() {}

	// Processes an event.
	virtual void processEvent( const InputEvent& event ) = 0;

	// Gets the type of this device.
	virtual const InputDeviceType::Enum getType() const = 0;
};

//-----------------------------------//

} // end namespace