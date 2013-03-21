/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Core/Reflection.h"
#include "Core/Event.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Different types of input devices.
 */

enum struct InputDeviceType
{
	Keyboard,
	Mouse,
	Joystick
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

	InputEvent( InputDeviceType );
	InputDeviceType deviceType;
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
	virtual const InputDeviceType getType() const = 0;
};

//-----------------------------------//

NAMESPACE_ENGINE_END