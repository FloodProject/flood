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

std::string InputDeviceType::getString( InputDeviceType::Enum type )
{
	switch(type)
	{
	case InputDeviceType::Keyboard:
		return "Keyboard";
	case InputDeviceType::Mouse:
		return "Mouse";
	case InputDeviceType::Joystick:
		return "Joystick";
	default:
		assert( false && "This should not be reached" );
	}
}

//-----------------------------------//

InputEvent::InputEvent( InputDeviceType::Enum type )
	: deviceType( type )
{ }

//-----------------------------------//

} // end namespace