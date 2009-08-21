/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/Device.h"

namespace vapor {
	namespace input {

//-----------------------------------//

std::string DeviceType::getString( DeviceType::Enum dt )
{
	switch( dt )
	{
	case DeviceType::Keyboard:
		return "Keyboard";
	case DeviceType::Mouse:
		return "Mouse";
	case DeviceType::Joystick:
		return "Joystick";
	default:
		return "(Unknown)";
	}
}

//-----------------------------------//

Device::Device()
{

}

//-----------------------------------//

Device::~Device()
{

}

//-----------------------------------//

Event::Event( DeviceType::Enum type )
	: deviceType( type )
{

}


//-----------------------------------//

} } // end namespaces