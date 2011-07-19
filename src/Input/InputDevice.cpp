/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "input/Device.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(InputDeviceType)
	ENUM(Keyboard)
	ENUM(Mouse)
	ENUM(Joystick)
REFLECT_ENUM_END()

//-----------------------------------//

InputEvent::InputEvent( InputDeviceType::Enum type )
	: deviceType( type )
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END