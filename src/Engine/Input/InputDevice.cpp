/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Input/Device.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(InputDeviceType)
	ENUM(Keyboard)
	ENUM(Mouse)
	ENUM(Joystick)
REFLECT_ENUM_END()

//-----------------------------------//

InputEvent::InputEvent( InputDeviceType type )
	: deviceType( type )
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END