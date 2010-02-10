/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/input/Device.h"

namespace vapor { namespace input {

//-----------------------------------//

std::string DeviceType::getString( const DeviceType::Enum dt )
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