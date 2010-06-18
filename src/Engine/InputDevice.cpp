/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/Device.h"

namespace vapor {

//-----------------------------------//

std::string InputDeviceType::getString( const InputDeviceType::Enum dt )
{
	switch( dt )
	{
	case InputDeviceType::Keyboard:
		return "Keyboard";
	case InputDeviceType::Mouse:
		return "Mouse";
	case InputDeviceType::Joystick:
		return "Joystick";
	default:
		return "(Unknown)";
	}
}

//-----------------------------------//

InputEvent::InputEvent( InputDeviceType::Enum type )
	: deviceType( type )
{ }

//-----------------------------------//

} // end namespace