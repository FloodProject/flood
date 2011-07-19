/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Reflection.h"
#include "Core/Event.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Different types of input devices.
 */

struct InputDeviceType
{
	enum Enum
	{
		Keyboard,
		Mouse,
		Joystick
	};
};

REFLECT_DECLARE_ENUM(InputDeviceType)

//-----------------------------------//

/**
 * Base struct for input events.
 */

struct API_INPUT InputEvent
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

class API_INPUT InputDevice
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

NAMESPACE_ENGINE_END